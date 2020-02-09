#import "public/api.h"
#import "C64+Private.h"
#import "interrupt.h"
#import "machine.h"
#import "snapshot.h"
#import "resources.h"
#import "maincpu.h"
#import "autostart.h"
#import "c64model.h"
#import "main.h"
#import "video.h"
#import "keyboard.h"
#import "joystick.h"

// sanity checks
OE_STATIC_ASSERT(KeyboardModLShift == (KeyboardMod)KBD_MOD_LSHIFT);
OE_STATIC_ASSERT(KeyboardModRALT == (KeyboardMod)KBD_MOD_RALT);
OE_STATIC_ASSERT(C64ModelPAL == (C64Model)C64MODEL_C64_PAL);
OE_STATIC_ASSERT(C64ModelCPAL == (C64Model)C64MODEL_C64C_PAL);
OE_STATIC_ASSERT(C64ModelNTSC == (C64Model)C64MODEL_C64_NTSC);
OE_STATIC_ASSERT(C64ModelCNTSC == (C64Model)C64MODEL_C64C_NTSC);

@implementation C64 {
    BOOL _initialized;
    video_canvas_t *_canvas;
    void * _buffer;
    NSSize _bufferSize;
}

- (instancetype)init {
    if ((self = [super init])) {
        _initialized = NO;
    }
    return self;
}

static snapshot_stream_t *snapshot_stream = NULL;
static int load_trap_happened = 0;
static int save_trap_happened = 0;

static void save_trap(uint16_t addr, void *success)
{
    static int save_disks;
    static int drive_type;
    resources_get_int("Drive8Type", &drive_type);
    save_disks = (drive_type < 1550) ? 1 : 0;
    
    /* params: stream, save_roms, save_disks, event_mode */
    if (machine_write_snapshot_to_stream(snapshot_stream, 0, save_disks, 0) >= 0)
        *((int *)success) = 1;
    else
        *((int *)success) = 0;
    save_trap_happened = 1;
}

static void load_trap(uint16_t addr, void *success)
{
    /* params: stream, event_mode */
    if (machine_read_snapshot_from_stream(snapshot_stream, 0) >= 0)
        *((int *)success) = 1;
    else
        *((int *)success) = 0;
    load_trap_happened = 1;
}

+ (C64 *)shared {
    static C64 *shared;
    
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        shared = [C64 new];
    });
    
    return shared;
}

#pragma mark - private

- (void)updateCanvasFromBuffer {
    if (_canvas == nil) {
        return;
    }
    
    _canvas->screen.pixels = _buffer;
    _canvas->screen.bpp    = 32;
    _canvas->screen.pitch  = (unsigned int)_bufferSize.width * 4;
}

- (void)createCanvas:(video_canvas_t *)canvas width:(unsigned int *)width height:(unsigned int *)height {
    _canvas = canvas;
    
    canvas->videoconfig->rendermode = VIDEO_RENDER_RGB_1X1;
    canvas->created = 1;
    canvas->initialized = 1;
    [self updateCanvasFromBuffer];
}

#pragma mark - initialization

- (void)initializeWithDelegate:(id <C64Delegate>)delegate {
    if (_initialized) {
        assert(false);
    }
    
    _delegate = delegate;
    main_program(0, NULL);
    _initialized = YES;
}

#pragma mark - configuration

- (void)setModel:(C64Model)model {
    if (c64model_get() == model) {
        return;
    }
    [self willChangeValueForKey:@"model"];
    c64model_set(model);
    [self didChangeValueForKey:@"model"];
}

- (C64Model)model {
    return (C64Model)c64model_get();
}

#pragma mark - image management

- (C64ImageType)imageTypeAtURL:(NSURL *)url {
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
        return C64ImageTypeTape;
    } else if ([cart containsObject:fileExtension]) {
        return C64ImageTypeCartridge;
    } else if ([disk containsObject:fileExtension]) {
        return C64ImageTypeDisk;
    }
    
    return C64ImageTypeUnknown;
}

- (BOOL)attachImageAtURL:(NSURL *)url error:(NSError **)error {
    return NO;
}

- (BOOL)autostartImageAtURL:(NSURL *)url error:(NSError **)error {
    int res = autostart_autodetect(url.fileSystemRepresentation, NULL, 0, AUTOSTART_MODE_RUN);
    return res == 0;
}

#pragma mark - save / load state

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName error:(NSError **)error {
    return NO;
}

- (BOOL)saveStateFromFileAtPath:(NSString *)fileName error:(NSError **)error {
    return NO;
}

#pragma mark - serialization

- (NSUInteger)stateSize {
    NSUInteger stateSize = 0;
    
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
    
    return stateSize;
}

- (NSData *)serializeState {
    return nil;
}

- (BOOL)deserializeState:(NSData *)data {
    return NO;
}

#pragma mark - execution

- (BOOL)execute {
    maincpu_headless_mainloop();
    return YES;
}

- (void)resetMachineSoft {
    machine_trigger_reset(MACHINE_RESET_MODE_SOFT);
}

- (void)resetMachineHard {
    machine_trigger_reset(MACHINE_RESET_MODE_HARD);
}

- (void)setBuffer:(void *)buffer size:(NSSize)size {
    _buffer     = buffer;
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