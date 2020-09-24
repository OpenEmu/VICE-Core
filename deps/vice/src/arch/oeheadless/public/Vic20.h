#import <Foundation/Foundation.h>
#import "api.h"

NS_ASSUME_NONNULL_BEGIN

@protocol Vic20Delegate;

typedef NS_ENUM(NSUInteger, Vic20ImageType)
{
    Vic20ImageTypeUnknown,
    Vic20ImageTypeDisk,
    Vic20ImageTypeTape,
    Vic20ImageTypeCartridge,
    Vic20ImageTypePRG,
};

typedef NS_ENUM(NSUInteger, Vic20Model)
{
    Vic20ModelPAL      = 0,
    Vic20ModelNTSC     = 1,
    Vic20ModelSuperVIC = 2,
};

OE_EXPORTED_CLASS
@interface Vic20: NSObject

@property (class, nonatomic, readonly) Vic20 *shared;

@property (nonatomic, assign, nullable) id<Vic20Delegate> delegate;
- (void)initializeWithBootPath:(NSString *)bootPath systemPathList:(NSArray<NSString *>*)pathList;

#pragma mark - configuration

@property (nonatomic) Vic20Model model;
@property (nonatomic, readonly) NSString *bootPath;
@property (nonatomic, readonly) NSArray<NSString *> *sysfilePathList;

#pragma mark - video

@property (nonatomic, readonly) double videoFrequency;
@property (nonatomic, readonly) NSRect videoFrame;

#pragma mark - audio

@property (nonatomic, readonly) int audioFrequency;
@property (nonatomic, readonly) int audioChannels;

#pragma mark - image management

- (Vic20ImageType)imageTypeAtURL:(NSURL *)url;
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
