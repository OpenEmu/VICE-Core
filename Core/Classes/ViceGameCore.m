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

#import "api.h"

#import "C64.h"
#import "C64_archdep.h"
#import "C128.h"
#import "C128_archdep.h"
#import "Vic20.h"
#import "Vic20_archdep.h"

const NSEventModifierFlags OENSEventModifierFlagFunctionKey = 1 << 24;

@interface ViceGameCore() <OEC64SystemResponderClient, C64Delegate>
{
    NSArray             *_availableDisplayModes;
    BOOL                _isJoystickPortSwapped;
    BOOL                _runstopLock;
    C64                 *_c64;
    NSArray<NSString *> *_sysfilePathList;
    uint32_t            *_buffer;
    OEIntSize           _bufferSize;
    OEIntRect           _screenRect;
    NSTimeInterval      _frameInterval;
}

- (void)initializeEmulator;

@end

@implementation ViceGameCore

- (id)init
{
    if((self = [super init])) {
        _c64            = C64.shared;
        _c64.delegate   = self;
        _bufferSize     = OEIntSizeMake(384, 272);
        NSRect f = _c64.videoFrame;
        _screenRect     = OEIntRectMake(f.origin.x, f.origin.y, f.size.width, f.size.height);
        _frameInterval  = _c64.videoFrequency;
    }

    return self;
}

#pragma mark - C64Delegate

- (void)updateAudioBuffer:(const int16_t *)buffer samples:(NSInteger)samples {
    [[self audioBufferAtIndex:0] write:buffer maxLength:samples * 2];
}

- (void)canvasWillResizeWidth:(NSUInteger)width height:(NSUInteger)height {
    NSRect f        = _c64.videoFrame;
    _screenRect     = OEIntRectMake(f.origin.x, f.origin.y, f.size.width, f.size.height);
    _frameInterval  = _c64.videoFrequency;
}

#pragma mark - emulation

- (void)setupEmulation {
    _c64.delegate = self;
    [super setupEmulation];
}

- (void)stopEmulation {
    [_c64 resetMachineHard];
    _c64.delegate = nil;
    [super stopEmulation];
}

- (BOOL)loadFileAtPath:(NSString *)path error:(NSError **)error
{
    [self initializeEmulator];
    
    return [_c64 autostartImageAtURL:[NSURL fileURLWithPath:path] error:error];
}

- (void)insertFileAtURL:(NSURL *)url completionHandler:(void(^)(BOOL success, NSError *error))block
{
    C64ImageType type = [_c64 imageTypeAtURL:url];
    
    block(YES, nil);
}

- (void)resetEmulation
{
    [_c64 resetMachineHard];
}

- (void)executeFrame
{
    [_c64 execute];
}

- (void)saveStateToFileAtPath:(NSString *)fileName completionHandler:(void(^)(BOOL success, NSError *error))block
{
    NSError *err = nil;
    BOOL res = [_c64 saveStateFromFileAtPath:fileName error:&err];
    block(res, err);
}

- (void)loadStateFromFileAtPath:(NSString *)fileName completionHandler:(void(^)(BOOL success, NSError *error))block
{
    NSError *err = nil;
    BOOL res = [_c64 loadStateFromFileAtPath:fileName error:&err];
    block(res, err);
}

- (NSTimeInterval)frameInterval {
    return _frameInterval;
}

- (const void*)getVideoBufferWithHint:(void *)hint {
    _buffer = hint;
    [_c64 setBuffer:hint size:NSSizeFromOEIntSize(_bufferSize)];
    return hint;
}

- (OEIntSize)bufferSize
{
    return _bufferSize;
}

- (OEIntSize)aspectSize
{
    // TODO
    return OEIntSizeMake(4, 3);
}

- (OEIntRect)screenRect
{
    return _screenRect;
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
    return _c64.audioFrequency;
}

- (NSUInteger)channelCount
{
    return _c64.audioChannels;
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

KeyboardMod keyCodeToMod( unsigned short keyCode) {
    switch (keyCode) {
        case kVK_Shift:
            return KeyboardModLShift;
        case kVK_RightShift:
            return KeyboardModRShift;
        case kVK_Control:
            return KeyboardModLCTRL;
        case kVK_RightControl:
            return KeyboardModRCTRL;
        case kVK_Option:
            return KeyboardModLALT;
        case kVK_RightOption:
            return KeyboardModRALT;
        default:
            return KeyboardModNone;
    }
}

KeyboardMod flagsToMod(NSEventModifierFlags flags) {
    KeyboardMod res = KeyboardModNone;
    if (flags & NX_DEVICELSHIFTKEYMASK) {
        res |= KeyboardModLShift;
    }
    if (flags & NX_DEVICERSHIFTKEYMASK) {
        res |= KeyboardModRShift;
    }
    
    if (flags & NX_DEVICELCTLKEYMASK) {
        res |= KeyboardModLCTRL;
    }
    if (flags & NX_DEVICERCTLKEYMASK) {
        res |= KeyboardModRCTRL;
    }
    
    if (flags & NX_DEVICELALTKEYMASK) {
        res |= KeyboardModLALT;
    }
    if (flags & NX_DEVICERALTKEYMASK) {
        res |= KeyboardModRALT;
    }

    return res;
}

- (oneway void)keyDown:(NSUInteger)keyCode
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

    // TODO: Fix flags, which will be sent as virtual key codes
    KeyboardMod mod = flagsToMod(0);

    NSLog(@"keyDown: code=%03d, flags=%08x, mod=%08lx", keyCode, (uint32_t)flags, mod);
    
    // Set the RunStopLock flag if Arrow up is pressed
    // if (keyCode == kVK_UpArrow) RunStopLock = true;
    [_c64 keyboardKeyDown:keyCode mod:mod];
}

- (oneway void)keyUp:(NSUInteger)keyCode
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

    // TODO: Fix flags, which will be sent as virtual key codes
    KeyboardMod mod = flagsToMod(0);
    
    NSLog(@"keyUp: code=%03d, flags=%08x, mod=%08lx", keyCode, (uint32_t)flags, mod);
    
    // UnSet RunStopLock flag if Arrow up is released
    // if (keyCode == 126) RunStopLock = false;
    
    [_c64 keyboardKeyUp:keyCode mod:mod];
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
//    // Avoid a RunStop Lock if the Arrow key is pressed, and up is pushed on a Joystick
//    if (RunStopLock && button == OEC64JoystickUp) keyboard_key_released(kVK_UpArrow);
//
    if (button == OEC64ButtonJump) button = OEC64JoystickUp;
//
    // Default to Port 2 for Player 1, since most C64 games use this
    int port;
    if (player == 1)
        port = _isJoystickPortSwapped ? 1 : 2;
    else
        port = _isJoystickPortSwapped ? 2 : 1;

    [_c64 joystickOrValue:joystick_bits[button] forPort:port];
}

- (oneway void)didReleaseC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    if (button == OEC64ButtonJump) button = OEC64JoystickUp;
//
    // Default to Port 2 for Player 1, since most C64 games use this
    int port;
    if (player == 1)
        port = _isJoystickPortSwapped ? 1 : 2;
    else
        port = _isJoystickPortSwapped ? 2 : 1;

    uint8_t val = 0xff & ~joystick_bits[button];
    [_c64 joystickAndValue:val forPort:port];
}

- (NSArray <NSDictionary <NSString *, id> *> *)displayModes
{
    if (_availableDisplayModes.count == 0)
    {
        _availableDisplayModes = [NSArray array];

        NSArray <NSDictionary <NSString *, id> *> *availableModesWithDefault =
        @[
          @{ OEGameCoreDisplayModeNameKey        : @"NTSC",
             OEGameCoreDisplayModePrefKeyNameKey : @"format",
             OEGameCoreDisplayModeStateKey       : @NO
             },
          @{ OEGameCoreDisplayModeNameKey        : @"PAL",
             OEGameCoreDisplayModePrefKeyNameKey : @"format",
             OEGameCoreDisplayModeStateKey       : @NO
             },
          ];

        _availableDisplayModes = availableModesWithDefault;
    }

    return _availableDisplayModes;
}

- (void)changeDisplayWithMode:(NSString *)displayMode
{
    if (_availableDisplayModes.count == 0)
        [self displayModes];

    NSMutableArray *tempModesArray = [NSMutableArray array];
    for(NSDictionary *modeDict in _availableDisplayModes)
    {
        NSString *mode = modeDict[OEGameCoreDisplayModeNameKey];
        NSString *pref = modeDict[OEGameCoreDisplayModePrefKeyNameKey];
        BOOL state = NO;

        if ([mode isEqualToString:displayMode])
            state = YES;

        [tempModesArray addObject:@{OEGameCoreDisplayModeNameKey : mode, OEGameCoreDisplayModeStateKey : @(state), OEGameCoreDisplayModePrefKeyNameKey : pref}];
    }

    if ([displayMode isEqualToString:@"NTSC"]) {
        _c64.model = C64ModelNTSC;
    } else {
        _c64.model = C64ModelPAL;
    }
    
    _availableDisplayModes = tempModesArray;
}


#pragma mark private

- (void)initializeEmulator
{
    static bool initialized = false;

    if (!initialized) {
        NSString *bootPath = self.owner.bundle.resourcePath;
#define MACOSX_ROMDIR @"ROM"
        NSArray<NSString *> *pathList = @[
            [NSString pathWithComponents:@[bootPath, MACOSX_ROMDIR, @"C64"]],
            [NSString pathWithComponents:@[bootPath, MACOSX_ROMDIR, @"DRIVES"]],
        ];
        [_c64 initializeWithBootPath:bootPath systemPathList:pathList];
        _c64.model     = C64ModelPAL;
        _frameInterval = _c64.videoFrequency;
        
        // Use the last selected display mode or default to the appropriate for the user system locale
        if (self.displayModeInfo == nil)
        {
            if ([self.systemRegion isEqualToString: @"North America"] || [self.systemRegion isEqualToString: @"Japan"])
                [self changeDisplayWithMode:@"NTSC"];
            else
                [self changeDisplayWithMode:@"PAL"];
        }
        else
        {
            NSString *lastSelectedModeToSet = self.displayModeInfo[@"format"];
            [self changeDisplayWithMode:lastSelectedModeToSet];

        }
        initialized = true;
    }
}

@end
