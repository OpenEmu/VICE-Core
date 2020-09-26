#import <Foundation/Foundation.h>
#import "api.h"

NS_ASSUME_NONNULL_BEGIN

@protocol C128Delegate;

typedef NS_ENUM(NSUInteger, C128ImageType)
{
    C128ImageTypeUnknown,
    C128ImageTypeDisk,
    C128ImageTypeTape,
    C128ImageTypeCartridge,
    C128ImageTypePRG,
};

typedef NS_ENUM(NSUInteger, C128Model)
{
    C128ModelPAL     = 0,
    C128ModelDPAL    = 1,
    C128ModelDCRPAL  = 2,
    C128ModelNTSC    = 3,
    C128ModelDNTSC   = 4,
    C128ModelDCRNTSC = 5,
};

OE_EXPORTED_CLASS
@interface C128: NSObject

@property (class, nonatomic, readonly) C128 *shared;

@property (nonatomic, assign, nullable) id<C128Delegate> delegate;
- (void)initializeWithBootPath:(NSString *)bootPath systemPathList:(NSArray<NSString *>*)pathList;

#pragma mark - configuration

@property (nonatomic) C128Model model;
@property (nonatomic, readonly) NSString *bootPath;
@property (nonatomic, readonly) NSArray<NSString *> *sysfilePathList;

#pragma mark - video

@property (nonatomic, readonly) double videoFrequency;
@property (nonatomic, readonly) NSRect videoFrame;

#pragma mark - audio

@property (nonatomic, readonly) int audioFrequency;
@property (nonatomic, readonly) int audioChannels;

#pragma mark - image management

- (C128ImageType)imageTypeAtURL:(NSURL *)url;
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
