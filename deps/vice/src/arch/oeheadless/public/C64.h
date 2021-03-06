#import <Foundation/Foundation.h>
#import "api.h"

NS_ASSUME_NONNULL_BEGIN

@protocol C64Delegate;

typedef NS_ENUM(NSUInteger, C64ImageType)
{
    C64ImageTypeUnknown,
    C64ImageTypeDisk,
    C64ImageTypeTape,
    C64ImageTypeCartridge,
    C64ImageTypePRG,
};

typedef NS_ENUM(NSUInteger, C64Model)
{
    C64ModelPAL     = 0,
    C64ModelCPAL    = 1,
    C64ModelNTSC    = 3,
    C64ModelCNTSC   = 4,
};

OE_EXPORTED_CLASS
@interface C64: NSObject

@property (class, nonatomic, readonly) C64 *shared;

@property (nonatomic, assign, nullable) id<C64Delegate> delegate;
- (void)initializeWithBootPath:(NSString *)bootPath systemPathList:(NSArray<NSString *>*)pathList;

#pragma mark - configuration

@property (nonatomic) C64Model model;
@property (nonatomic, readonly) NSString *bootPath;
@property (nonatomic, readonly) NSArray<NSString *> *sysfilePathList;

#pragma mark - video

@property (nonatomic, readonly) double videoFrequency;
@property (nonatomic, readonly) NSRect videoFrame;

#pragma mark - audio

@property (nonatomic, readonly) int audioFrequency;
@property (nonatomic, readonly) int audioChannels;

#pragma mark - image management

- (C64ImageType)imageTypeAtURL:(NSURL *)url;
- (BOOL)attachImageAtURL:(NSURL *)url error:(NSError **)error;
- (BOOL)autostartImageAtURL:(NSURL *)url error:(NSError **)error;

#pragma mark - save / load save state

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName error:(NSError **)error;
- (BOOL)saveStateFromFileAtPath:(NSString *)fileName error:(NSError **)error;

#pragma mark - execution

- (BOOL)execute;
- (void)resetMachineSoft;
- (void)resetMachineHard;
- (void)setBuffer:(void *)buffer size:(NSSize)size;

#pragma mark - input

- (void)keyboardKeyDown:(int)keycode mod:(KeyboardMod)mod;
- (void)keyboardKeyUp:(int)keycode mod:(KeyboardMod)mod;
- (void)keyboardKeyClear;

- (void)joystickOrValue:(uint8_t)value forPort:(unsigned int)port;
- (void)joystickAndValue:(uint8_t)value forPort:(unsigned int)port;
- (void)joystickClearValueForPort:(unsigned int)port;
- (void)joystickClearAll;

@end

NS_ASSUME_NONNULL_END
