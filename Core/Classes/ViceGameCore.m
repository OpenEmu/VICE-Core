/*
 Copyright (c) 2016, OpenEmu Team

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
 * Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.
 * Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.
 * Neither the name of the OpenEmu Team nor the
 names of its contributors may be used to endorse or promote products
 derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */


#import "ViceGameCore.h"
#import "OEC64SystemResponderClient.h"
#import <OpenGL/gl.h>
#import "main.h"
#import "archdep+private.h"
#import "sound.h"
#import "attach.h"
#import "autostart.h"
#import "autostart-prg.h"
#import "resources.h"
#import "machine.h"
#import "main_program.h"
#import "maincpu.h"
#import "interrupt.h"
#import "kbdbuf.h"
#import "joystick.h"
#import "keyboard.h"
#import "vsync.h"

#import <setjmp.h>

static __unsafe_unretained ViceGameCore* core;

uint32_t* OEvideobuffer;
int vid_width = 512, vid_height = 512;
jmp_buf emu_exit_jmp;

static dispatch_semaphore_t sem_pause, sem_paused, vsync_hold;
static bool running, inpause, paused, vSync;


@interface ViceGameCore() <OEC64SystemResponderClient>
{
    NSThread *thread;
}

- (void)initializeEmulator;
- (void)emulatorThread;

@end

@implementation ViceGameCore

- (id)init
{
    if((self = [super init]))
    {
        core = self;

        OEvideobuffer = (uint32_t*)malloc(1024 * 1024 * 4);
        memset(OEvideobuffer, 0, 1024*1024*4);

        running = false;
        paused  = false;

        vsync_hold = dispatch_semaphore_create(0);
        sem_paused = dispatch_semaphore_create(0);
        sem_pause  = dispatch_semaphore_create(0);
    }

    return self;
}

#pragma emulation

- (void)stopEmulation {
    running = false;
    emu_resume(); // ensure it is running before we wait for it

    [thread cancel];
    while (![thread isFinished]) {
        [NSThread sleepForTimeInterval:0.050];
    }

    thread = nil;
    core   = nil;

    if(OEvideobuffer) {
        free(OEvideobuffer);
    }

    [super stopEmulation];
}

- (BOOL)loadFileAtPath:(NSString *)path error:(NSError **)error
{

    [self initializeEmulator];

    autostart_autodetect([path cStringUsingEncoding:NSASCIIStringEncoding], NULL, 0, AUTOSTART_MODE_RUN);

    running = true;

    return YES;
}

- (void)resetEmulation
{
    machine_trigger_reset(MACHINE_RESET_MODE_HARD);
}

//- (void)setPauseEmulation:(BOOL)flag
//{
//
//    if (flag){
//        emu_pause();
//    }else{
//        emu_resume();
//    }
//
//    [super setPauseEmulation:flag];
//}

- (void)executeFrame
{
    if (vSync){
        dispatch_semaphore_signal(vsync_hold);
    }
}

#pragma mark game save state

static void vsync_pause_trap(uint16_t a, void * b)
{   // This routine is called by the emulated CPU interupt on the Emulated CPU thread
    //    it will hold the CPU until OE executes the next frame for vSync
    //    there is a slight chance that inpuase was called between the CPU interupt
    //    trap set and the interput trigger so check for inpause and return if it was set

    if (!running || inpause) {
        return;
    }

    vsync_suspend_speed_eval();
    vSync = true;

    // Hold the Emulated CPU Thread for vSync
    dispatch_semaphore_wait(vsync_hold, DISPATCH_TIME_FOREVER);

    // vSync hold has been released
    if (inpause)
    {   // If the system is getting ready to pause, vSync hold was released
        //   and the Core thread is waiting, release the core thread
        //   If we got here, the vSync hold was set before inpause started
        dispatch_semaphore_signal(sem_paused);
        usleep(20);
    }
    vSync= false;
}


static void emu_pause_trap(uint16_t a, void * b)
{   // This routine is called by the emulated CPU interupt on the Emulated CPU thread
    //    it will pause the CPU until the core thread releases it
    //    there is a slight chance that vSync was called between the CPU interupt
    //    trap set and the interupt trigger so check for vSync and return if it was set

    if (!running || vSync) {
        return;
    }
    vsync_suspend_speed_eval();
    paused = true;

    // Trigger the OE Core to continue
    dispatch_semaphore_signal(sem_paused);

    // Pause the Emulated CPU thread
    dispatch_semaphore_wait(sem_pause, DISPATCH_TIME_FOREVER);
    paused = false;
}

static void emu_pause()
{
    // Set the inpause state
    inpause = true;

    if (vSync){
        // If the CPU is in a vSyn hold, relases it now
        dispatch_semaphore_signal(vsync_hold);
        dispatch_semaphore_wait(sem_paused, DISPATCH_TIME_FOREVER);
    }

    if (!paused && running) {
        // Trigger the CPU to pause for the core
        interrupt_maincpu_trigger_trap(emu_pause_trap, NULL);
        dispatch_semaphore_wait(sem_paused, DISPATCH_TIME_FOREVER);
    }
}

static void emu_resume()
{
    if (vSync){
        // Release the vSync hold  this will only get called
        //  when the core is shutting down
        dispatch_semaphore_signal(vsync_hold);
    }

    if (paused) {
        // Release the CPU pause
        dispatch_semaphore_signal(sem_pause);
    }

    // Exit In Pause State
    inpause = false;
}

- (void)saveStateToFileAtPath:(NSString *)fileName completionHandler:(void(^)(BOOL success, NSError *error))block
{
    emu_pause();

    BOOL res = TRUE;
    if (machine_write_snapshot([fileName cStringUsingEncoding:NSASCIIStringEncoding], 1, 1, 0) < 0) {
        //save snap failed
        res = FALSE;
    }

    emu_resume();

    block(res, nil);
}

- (void)loadStateFromFileAtPath:(NSString *)fileName completionHandler:(void(^)(BOOL success, NSError *error))block
{
    BOOL res = TRUE;

    emu_pause();

    if (machine_read_snapshot([fileName cStringUsingEncoding:NSASCIIStringEncoding], 0) < 0) {
        //load snap failed
        res = FALSE;
    }

    emu_resume();

    block (res, nil);
}


#pragma mark Video

- (const void *)videoBuffer
{
    return OEvideobuffer;
}

- (OEIntSize)bufferSize
{
    return OEIntSizeMake(vid_width, vid_height);
}

- (OEIntRect)screenRect
{
    return OEIntRectMake(0, 0,  vid_width, vid_height);
}

- (GLenum)pixelFormat
{
    return GL_BGRA;
}

- (GLenum)pixelType
{
    return GL_UNSIGNED_INT_8_8_8_8_REV;
}

- (GLenum)internalPixelFormat
{
    return GL_RGB8;
}

- (double)audioSampleRate
{
    return 44100;
}

- (NSUInteger)channelCount
{
    return 1;
}

- (NSUInteger)audioBitDepth {
    return 16;
}

#pragma responder

- (oneway void)mouseMovedAtPoint:(OEIntPoint)point
{
}

- (oneway void)leftMouseDownAtPoint:(OEIntPoint)point
{
}

- (oneway void)leftMouseUp
{
}

- (oneway void)rightMouseDownAtPoint:(OEIntPoint)point
{
}

- (oneway void)rightMouseUp
{
}

- (oneway void)keyDown:(unsigned short)keyCode characters:(NSString *)characters charactersIgnoringModifiers:(NSString *)charactersIgnoringModifiers flags:(NSEventModifierFlags)flags
{
    NSLog(@"keyDown: code=%03d, flags=%08x", keyCode, (uint32_t)flags);
    keyboard_key_pressed(keyCode);
}

- (oneway void)keyUp:(unsigned short)keyCode characters:(NSString *)characters charactersIgnoringModifiers:(NSString *)charactersIgnoringModifiers flags:(NSEventModifierFlags)flags
{
    NSLog(@"keyUp: code=%03d, flags=%08x", keyCode, (uint32_t)flags);
    keyboard_key_released(keyCode);
}


static uint8_t joystick_bits[] = {
    [OEC64JoystickUp]    = 0x01,
    [OEC64JoystickDown]  = 0x02,
    [OEC64JoystickLeft]  = 0x04,
    [OEC64JoystickRight] = 0x08,
    [OEC64ButtonFire]    = 0x10
};

const size_t joystick_len = sizeof(joystick_bits) / sizeof(joystick_bits[0]);

- (oneway void)didPushC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    if (button < joystick_len) {
        joystick_set_value_or((uint32_t)player, joystick_bits[button]);
    }
}

- (oneway void)didReleaseC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    if (button < joystick_len) {
        uint8_t val = 0xff & ~joystick_bits[button];
        joystick_set_value_and((uint32_t)player, val);
    }
}

#pragma mark sounddev

static int openemu_init(const char *param, int *speed, int *fragsize, int *fragnr, int *channels)
{
    return 0;
}

static int openemu_write(SWORD *pbuf, size_t nr)
{
    [[core ringBufferAtIndex:0] write:pbuf maxLength:nr * 2];
    return 0;
}

static sound_device_t openemu_device =
{
    "openemu",
    openemu_init,
    openemu_write,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    0,
    2
};

int sound_init_openemu_device(void) {
    return sound_register_device(&openemu_device);
}

#pragma mark private

- (void)initializeEmulator
{
    static bool initialized = false;

    if (!initialized) {
        archdep_set_boot_path([[[self owner] bundle] resourcePath]);
        main_init();
        initialized = true;
    }

    thread = [[NSThread alloc] initWithTarget:self selector:@selector(emulatorThread) object:nil];
    thread.name = @"C64";
    [thread start];
}

static void check_cancelled()
{
    if ([[NSThread currentThread] isCancelled]) {
        longjmp(emu_exit_jmp, 1);
    }
}

void vsyncarch_presync(void)
{
    check_cancelled();
    kbdbuf_flush();
}

void vsyncarch_postsync(void)
{   //This is called after every frame
    //  three things must be ensured before the CPU is paused for vSync
    //  1: The core must be running
    //  2: The core rate must be 0
    //  3: The core must not be initating a CPU pause


    if ([core rate] == 0.0f && running && !inpause) {
        interrupt_maincpu_trigger_trap(vsync_pause_trap, NULL);
    }
}

- (void)emulatorThread
{
    @autoreleasepool {
        if (setjmp(emu_exit_jmp) == 0) {
            maincpu_mainloop();
        }

        NSLog(@"returning from emulator thread");
    }
}

@end
