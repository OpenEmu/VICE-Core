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
#import <Carbon/Carbon.h>
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
#import "cartridge.h"
#import "tape.h"

#import <setjmp.h>

static __unsafe_unretained ViceGameCore* core;

uint32_t* OEvideobuffer;
int vid_width = 384, vid_height = 272;
jmp_buf emu_exit_jmp;
const NSEventModifierFlags OENSEventModifierFlagFunctionKey = 1 << 24;
double pixelAspectRatio;

static dispatch_semaphore_t sem_Core_pause, sem_CPU_pause, sem_vSync_hold;
static bool running, pausestart, CPU_paused, vSync_held, BootComplete;
const char archdep_boot_path(void);

@interface ViceGameCore() <OEC64SystemResponderClient>
{
    NSThread *thread;
    BOOL _isJoystickPortSwapped;
    bool RunStopLock;
}

- (void)initializeEmulator;
- (void)emulatorThread;

@end

@implementation ViceGameCore

- (id)init
{
    if((self = [super init])) {
        core = self;

        OEvideobuffer = (uint32_t*)malloc(1024 * 1024 * 4);
        memset(OEvideobuffer, 0, 1024 * 1024 * 4);

        running = false;
        CPU_paused  = false;

        sem_vSync_hold   = dispatch_semaphore_create(0);
        sem_CPU_pause   = dispatch_semaphore_create(0);
        sem_Core_pause  = dispatch_semaphore_create(0);
    }

    return self;
}

#pragma emulation

- (void)stopEmulation {
    running = false;

    emu_resume(); // ensure vSync hold and CPU pause are cleared

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

    autostart_autodetect(path.fileSystemRepresentation, NULL, 0, AUTOSTART_MODE_RUN);

    running = true;

    return YES;
}

- (void)insertFileAtURL:(NSURL *)url completionHandler:(void(^)(BOOL success, NSError *error))block
{
    NSString *fileExtension = url.pathExtension.lowercaseString;
    NSArray *tape = @[@"tap", @"t64"];
    NSArray *cart = @[@"crt", @"bin"];
    NSArray *disk = @[@"d64", @"d67", @"d71", @"d80", @"d81", @"d82", @"g41", @"g64", @"g71", @"p64", @"x64", @"d1m", @"d2m", @"d4m"];

    if([tape containsObject:fileExtension])
    {
        tape_image_detach(1);
        tape_image_attach(1, url.fileSystemRepresentation);
    }
    else if([cart containsObject:fileExtension])
    {
        cartridge_detach_image(-1);
        cartridge_attach_image(CARTRIDGE_CRT, url.fileSystemRepresentation);
    }
    else if([disk containsObject:fileExtension])
    {
        file_system_detach_disk(8);
        //file_system_detach_disk(9);
        //file_system_detach_disk(10);
        //file_system_detach_disk(11);
        file_system_attach_disk(8, url.fileSystemRepresentation);
    }

    block(YES, nil);
}

- (void)resetEmulation
{
    machine_trigger_reset(MACHINE_RESET_MODE_HARD);
}

- (void)executeFrame
{
    if(!BootComplete) {
        //The emulator is finally in a fully initalized state
        BootComplete = true;
    }

    if (vSync_held){
        dispatch_semaphore_signal(sem_vSync_hold);
    }
}

#pragma mark game save state

static void emu_pause_trap(uint16_t a, void * b)
{
    // This routine is called by the emulated CPU interrupt on the Emulated CPU thread
    //    it will pause the CPU until the core thread releases it

    if (!running) {
        return;
    }

    // There is a slight chance that vSync was called between the CPU interrupt
    //    trap set and the interrupt trigger so check for vSync
    //    and release the hold before continuing
    if (vSync_held) {
        // If the CPU is in a vSyn hold, relases it now
        dispatch_semaphore_signal(sem_vSync_hold);

        // wait until vSync is released
        dispatch_semaphore_wait(sem_CPU_pause, DISPATCH_TIME_FOREVER);
    }

    CPU_paused = true;
    vsync_suspend_speed_eval();

    // Signal the core to continue
    dispatch_semaphore_signal(sem_CPU_pause);

    // Pause the Emulated CPU thread and wait
    dispatch_semaphore_wait(sem_Core_pause, DISPATCH_TIME_FOREVER);

    //The CPU thread has been released
    CPU_paused = false;
}

static void emu_pause()
{
    // Set the pausestart state
    pausestart = true;

    if (vSync_held) {
        // If the CPU is in a vSyn hold, relases it now
        dispatch_semaphore_signal(sem_vSync_hold);

        // wait until vSync is released
        dispatch_semaphore_wait(sem_CPU_pause, DISPATCH_TIME_FOREVER);
    }

    if (!CPU_paused && running) {
        // Trigger the CPU to pause for the core
        interrupt_maincpu_trigger_trap(emu_pause_trap, NULL);

        //Wait until the CPU has been paused by the interrupt call
        dispatch_semaphore_wait(sem_CPU_pause, DISPATCH_TIME_FOREVER);
    }
}

static void emu_resume()
{
    if (vSync_held) {
        // If the CPU is in a vSyn hold, relases it now
        dispatch_semaphore_signal(sem_vSync_hold);
    }

    if (CPU_paused) {
        // Release the CPU pause
        dispatch_semaphore_signal(sem_Core_pause);
    }

    // Exit pausestart state
    pausestart = false;
}

- (void)saveStateToFileAtPath:(NSString *)fileName completionHandler:(void(^)(BOOL success, NSError *error))block
{
    BOOL res = TRUE;

    emu_pause();

    if (machine_write_snapshot(fileName.fileSystemRepresentation, 1, 1, 0) < 0) {
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

    if (!BootComplete) {
        autostart_snapshot(fileName.fileSystemRepresentation, NULL);

        //Autostart functions use warp mode.  but autoload-sanpshot does not turn it off
        //   we need to make sure it is turned off or bad things end up happening
        resources_set_int("WarpMode", 0);
    } else {
        if (machine_read_snapshot(fileName.fileSystemRepresentation, 0) < 0) {
            //load snap failed
            res = FALSE;
        }
    }

    emu_resume();

    block (res, nil);
}

#pragma mark Video

static void vSync_hold_trap(uint16_t a, void * b)
{
    // This routine is called by the emulated CPU interrupt on the Emulated CPU thread
    //    it will hold the CPU until OE executes the next frame for vSync

    // There is a slight chance that pause was called between the CPU interrupt
    //    trap set and the interrput trigger so check for pausestart and return if it was set
    if (!running || pausestart) {
        return;
    }

    vSync_held = true;
    vsync_suspend_speed_eval();

    // Hold the Emulated CPU Thread for vSync
    dispatch_semaphore_wait(sem_vSync_hold, DISPATCH_TIME_FOREVER);

    // vSync hold has been released
    if (pausestart)
    {
        // The system is getting ready to pause,
        //   and the core thread is waiting, release the core thread
        //   and give a little time for Core thread to resume
        dispatch_semaphore_signal(sem_CPU_pause);
        usleep(20);
    }
    vSync_held= false;
}

- (const void *)videoBuffer
{
    return OEvideobuffer;
}

- (OEIntSize)bufferSize
{
    return OEIntSizeMake(384, 272);
}

- (OEIntSize)aspectSize
{
    return OEIntSizeMake(vid_width * pixelAspectRatio, vid_height);
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

- (NSUInteger)audioBitDepth
{
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
    if(keyCode == kVK_Function)
        return;

    switch (keyCode)
    {
        case kVK_F1:
        case kVK_F2:
        case kVK_F3:
        case kVK_F4:
        case kVK_F5:
        case kVK_F6:
        case kVK_F7:
        case kVK_F8:
        case kVK_F9:
        case kVK_F10:
        case kVK_F11:
        case kVK_F12:
            if (!(flags & OENSEventModifierFlagFunctionKey))
                return;
    }

    NSLog(@"keyDown: code=%03d, flags=%08x", keyCode, (uint32_t)flags);
    
    // Set the RunStopLock flag if Arrow up is pressed
    if (keyCode == kVK_UpArrow) RunStopLock = true;
    
    keyboard_key_pressed(keyCode);
}

- (oneway void)keyUp:(unsigned short)keyCode characters:(NSString *)characters charactersIgnoringModifiers:(NSString *)charactersIgnoringModifiers flags:(NSEventModifierFlags)flags
{
    if(keyCode == kVK_Function)
        return;

    NSLog(@"keyUp: code=%03d, flags=%08x", keyCode, (uint32_t)flags);
    
    // UnSet RunStopLock flag if Arrow up is released
    if (keyCode == 126) RunStopLock = false;
    
    keyboard_key_released(keyCode);
}

static uint8_t joystick_bits[] = {
    [OEC64JoystickUp]    = 0x01,
    [OEC64JoystickDown]  = 0x02,
    [OEC64JoystickLeft]  = 0x04,
    [OEC64JoystickRight] = 0x08,
    [OEC64ButtonFire]    = 0x10
};

- (oneway void)swapJoysticks
{
    // do port swap
    _isJoystickPortSwapped = !_isJoystickPortSwapped;
}

- (oneway void)didPushC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    // Avoid a RunStop Lock if the Arrow key is pressed, and up is pushed on a Joystick
    if (RunStopLock && button == OEC64JoystickUp) keyboard_key_released(kVK_UpArrow);

    if (button == OEC64ButtonJump) button = OEC64JoystickUp;

    // Default to Port 2 for Player 1, since most C64 games use this
    int port;
    if (player == 1)
        port = _isJoystickPortSwapped ? 1 : 2;
    else
        port = _isJoystickPortSwapped ? 2 : 1;

    joystick_set_value_or(port, joystick_bits[button]);
}

- (oneway void)didReleaseC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    if (button == OEC64ButtonJump) button = OEC64JoystickUp;

    // Default to Port 2 for Player 1, since most C64 games use this
    int port;
    if (player == 1)
        port = _isJoystickPortSwapped ? 1 : 2;
    else
        port = _isJoystickPortSwapped ? 2 : 1;

    uint8_t val = 0xff & ~joystick_bits[button];
    joystick_set_value_and(port, val);
}

#pragma mark sounddev

static int openemu_init(const char *param, int *speed, int *fragsize, int *fragnr, int *channels)
{
    return 0;
}

static int openemu_write(int16_t *pbuf, size_t nr)
{
    [[core audioBufferAtIndex:0] write:pbuf maxLength:nr * 2];
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
{
    //This is called after every frame
    int warp;

    // Get warp state of Emulator
    resources_get_int("WarpMode", &warp);

    //If the eumaltor is not running, is in warp mode, or starting pause, or not fully booted:
    //   we don't want to set a vsyn trap
    if (!warp && !pausestart && BootComplete && running) {
        if ([core rate] == 0.0f ) {
            interrupt_maincpu_trigger_trap(vSync_hold_trap, NULL);
        }
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
