
#import "public/api.h"
#import "Vic20_archdep.h"
#import "Vic20+Private.h"
#import "interrupt.h"
#import "machine.h"
#import "snapshot.h"
#import "resources.h"
#import "maincpu.h"
#import "autostart.h"
#import "vic20model.h"
#import "main.h"
#import "video.h"
#import "keyboard.h"
#import "joystick.h"
#import "vsync.h"


// sanity checks
OE_STATIC_ASSERT(KeyboardModLShift == (KeyboardMod)KBD_MOD_LSHIFT);
OE_STATIC_ASSERT(KeyboardModRALT == (KeyboardMod)KBD_MOD_RALT);
OE_STATIC_ASSERT(Vic20ModelPAL == (Vic20Model)VIC20MODEL_VIC20_PAL);
OE_STATIC_ASSERT(Vic20ModelNTSC == (Vic20Model)VIC20MODEL_VIC20_NTSC);
OE_STATIC_ASSERT(Vic20ModelSuperVIC == (Vic20Model)VIC20MODEL_VIC21);

OE_EXPORTED_CLASS
@interface C64: NSObject
@end
@implementation C64 {}
@end

OE_EXPORTED_CLASS
@interface C128: NSObject
@end
@implementation C128 {}
@end

OE_EXPORTED_CLASS
@interface C64dtv: NSObject
@end
@implementation C64dtv {}
@end


@implementation Vic20 {
    BOOL _initialized;
    video_canvas_t *_canvas;
    void *_buffer;
    NSSize _bufferSize;
    size_t _stateSize;
    unsigned long _ticksPerFrame;
}

- (instancetype)init {
    if ((self = [super init])) {
        _initialized = NO;
    }
    return self;
}

+ (Vic20 *)shared {
    static Vic20 *shared;
    
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        shared = [Vic20 new];
    });
    
    return shared;
}

#pragma mark - private

- (void)updateCanvasFromBuffer {
    if (_canvas == nil) {
        return;
    }
    
    _canvas->screen.pixels = _buffer;
    _canvas->screen.bpp = 32;
    _canvas->screen.pitch = (unsigned int)_bufferSize.width * 4;
}

- (void)createCanvas:(video_canvas_t *)canvas width:(unsigned int *)width height:(unsigned int *)height {
    _canvas = canvas;
    
    canvas->videoconfig->rendermode = VIDEO_RENDER_RGB_1X1;
    canvas->created = 1;
    canvas->initialized = 1;
    [self updateCanvasFromBuffer];
}

- (void)canvasDidResize:(video_canvas_t *)canvas {
    [_delegate canvasWillResizeWidth:canvas->draw_buffer->canvas_width height:canvas->draw_buffer->canvas_height];
}

- (void)initSoundSpeed:(int *)speed fragSize:(int *)fragSize fragNumber:(int *)fragNumber channels:(int *)channels {
    _audioFrequency = *speed;
    _audioChannels = *channels;
}

#pragma mark - video

- (double)videoFrequency {
    return vsync_get_refresh_frequency();
}

- (NSRect)videoFrame {
    if (_canvas) {
        return NSMakeRect(0, 0, _canvas->draw_buffer->canvas_width, _canvas->draw_buffer->canvas_height);
    }
    // default to PAL
    return NSMakeRect(0, 0, 384, 272);
}

#pragma mark - initialization

- (void)initializeWithBootPath:(NSString *)bootPath systemPathList:(NSArray<NSString *> *)pathList {
    if (_initialized) {
        assert(false);
    }
    
    _bootPath = [bootPath copy];
    _sysfilePathList = [pathList copy];
    
    main_program(0, NULL);
    [self updateTicksPerFrame];
    _initialized = YES;
}

#pragma mark - configuration

- (void)updateTicksPerFrame {
    _ticksPerFrame = (unsigned long)(1e6 / vsync_get_refresh_frequency());
}

- (void)setModel:(Vic20Model)model {
    if (vic20model_get() == model) {
        return;
    }
    [self willChangeValueForKey:@"model"];
    vic20model_set(model);
    [self updateTicksPerFrame];
    [self didChangeValueForKey:@"model"];
}

- (Vic20Model)model {
    return (Vic20Model)vic20model_get();
}

#pragma mark - image management

- (Vic20ImageType)imageTypeAtURL:(NSURL *)url {
    static NSSet<NSString *> *tape;
    static NSSet<NSString *> *cart;
    static NSSet<NSString *> *disk;
    
    static dispatch_once_t init;
    dispatch_once(&init, ^{
        tape = [NSSet setWithArray:@[@"tap", @"t64"]];
        cart = [NSSet setWithArray:@[@"crt", @"bin"]];
        disk = [NSSet setWithArray:@[@"d64", @"d67", @"d71", @"d80", @"d81", @"d82", @"g41", @"g64", @"g71", @"p64",
                                     @"x64", @"d1m", @"d2m", @"d4m"]];
    });
    
    NSString *fileExtension = url.pathExtension.lowercaseString;
    if ([tape containsObject:fileExtension]) {
        return Vic20ImageTypeTape;
    } else if ([cart containsObject:fileExtension]) {
        return Vic20ImageTypeCartridge;
    } else if ([disk containsObject:fileExtension]) {
        return Vic20ImageTypeDisk;
    }
    
    return Vic20ImageTypeUnknown;
}

- (BOOL)attachImageAtURL:(NSURL *)url error:(NSError **)error {
    return NO;
}

- (BOOL)autostartImageAtURL:(NSURL *)url error:(NSError **)error {
    int res = autostart_autodetect(url.fileSystemRepresentation, NULL, 0, AUTOSTART_MODE_RUN);
    return res == 0;
}

#pragma mark - save / load state

typedef void (^trapBlock)(void);

static void trap_handler(uint16_t addr, void *success)
{
    void (^handler)(void);
    handler = (__bridge trapBlock)(success);
    handler();
}

- (void)executeTrap:(trapBlock)block {
    interrupt_maincpu_trigger_trap(trap_handler, (__bridge void *)block);
    maincpu_headless_mainloop(MACHINE_EVENT_TRAP);
}

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName error:(NSError **)error {
    __block int res = 0;
    [self executeTrap:^{
        res = machine_read_snapshot(fileName.UTF8String, 0);
    }];
    return res >= 0;
}

- (BOOL)saveStateFromFileAtPath:(NSString *)fileName error:(NSError **)error {
    __block int res = 0;
    [self executeTrap:^{
        res = machine_write_snapshot(fileName.UTF8String, 0, 1, 0);
    }];
    
    return res >= 0;
}

#pragma mark - serialization

- (NSUInteger)stateSize {
    __block NSUInteger stateSize = 0;
    
    [self executeTrap:^{
        int drive_type;
        resources_get_int("Drive8Type", &drive_type);
        int save_disks = (drive_type < 1550) ? 1 : 0;
    
        snapshot_stream_t *snapshot_stream = snapshot_memory_write_fopen(NULL, 0);
        int res = machine_write_snapshot_to_stream(snapshot_stream, 0, save_disks, 0);
        if (res >= 0) {
            snapshot_fseek(snapshot_stream, 0, SEEK_END);
            stateSize = (NSUInteger)snapshot_ftell(snapshot_stream);
        }
        snapshot_fclose(snapshot_stream);
    }];
    
    return stateSize;
}

- (NSData *)serializeState {
    if (_stateSize == 0) {
        _stateSize = self.stateSize;
    }
    
    __block void *buf = nil;
    __block NSUInteger stateSize = 0;
    __block int res = 0;
    
    [self executeTrap:^{
        int drive_type;
        resources_get_int("Drive8Type", &drive_type);
        int save_disks = (drive_type < 1550) ? 1 : 0;
    
        buf = malloc(_stateSize);
        snapshot_stream_t *snapshot_stream = snapshot_memory_write_fopen(buf, _stateSize);
        res = machine_write_snapshot_to_stream(snapshot_stream, 0, save_disks, 0);
        snapshot_fseek(snapshot_stream, 0, SEEK_END);
        stateSize = (NSUInteger)snapshot_ftell(snapshot_stream);
        snapshot_fclose(snapshot_stream);
    }];
    
    NSData *data = [NSData dataWithBytesNoCopy:buf length:stateSize freeWhenDone:YES];
    if (res >= 0) {
        return data;
    }
    
    return nil;
}

- (BOOL)deserializeState:(NSData *)data {
    __block int res = 0;
    
    [self executeTrap:^{
        resources_set_int("WarpMode", 0);
        snapshot_stream_t *snapshot_stream = snapshot_memory_read_fopen(data.bytes, data.length);
        res = machine_read_snapshot_from_stream(snapshot_stream, 0);
        snapshot_fclose(snapshot_stream);
    }];
    
    return res >= 0;
}

#pragma mark - execution

extern unsigned long vsyncarch_ticks;

- (BOOL)execute {
    maincpu_headless_mainloop(MACHINE_EVENT_FRAME);
    vsyncarch_ticks += _ticksPerFrame;
    return YES;
}

- (void)resetMachineSoft {
    machine_trigger_reset(MACHINE_RESET_MODE_SOFT);
}

- (void)resetMachineHard {
    machine_trigger_reset(MACHINE_RESET_MODE_HARD);
}

- (void)setBuffer:(void *)buffer size:(NSSize)size {
    _buffer = buffer;
    _bufferSize = size;
    [self updateCanvasFromBuffer];
}

#pragma mark - input

- (void)keyboardKeyDown:(int)keycode mod:(KeyboardMod)mod {
    keyboard_key_pressed(keycode, mod);
}

- (void)keyboardKeyUp:(int)keycode mod:(KeyboardMod)mod {
    keyboard_key_released(keycode, mod);
}

- (void)keyboardKeyClear {
    keyboard_key_clear();
}

- (void)joystickOrValue:(uint8_t)value forPort:(unsigned int)port {
    joystick_set_value_or(port, value);
}

- (void)joystickAndValue:(uint8_t)value forPort:(unsigned int)port {
    joystick_set_value_and(port, value);
}

- (void)joystickClearValueForPort:(unsigned int)port {
    joystick_clear(port);
}

- (void)joystickClearAll {
    joystick_clear_all();
}


@end


