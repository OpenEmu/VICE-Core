#import <Foundation/Foundation.h>
#import "api.h"

NS_ASSUME_NONNULL_BEGIN

@protocol C64dtvDelegate;

typedef NS_ENUM(NSUInteger, C64dtvImageType)
{
    C64dtvImageTypeUnknown,
    C64dtvImageTypeDisk,
    C64dtvImageTypeTape,
    C64dtvImageTypeCartridge,
    C64dtvImageTypePRG,
};

typedef NS_ENUM(NSUInteger, C64dtvModel)
{
    DTVModelV2PAL      = 0,
    DTVModelV2NTSC     = 1,
    DTVModelV3PAL      = 2,
    DTVModelV3NTSC     = 3,
    DTVModelHummerNTSC = 4

};

OE_EXPORTED_CLASS
@interface C64dtv: NSObject

@property (class, nonatomic, readonly) C64dtv *shared;

@property (nonatomic, assign, nullable) id<C64dtvDelegate> delegate;
- (void)initializeWithBootPath:(NSString *)bootPath systemPathList:(NSArray<NSString *>*)pathList;

#pragma mark - configuration

@property (nonatomic) C64dtvModel model;
@property (nonatomic, readonly) NSString *bootPath;
@property (nonatomic, readonly) NSArray<NSString *> *sysfilePathList;

#pragma mark - video

@property (nonatomic, readonly) double videoFrequency;
@property (nonatomic, readonly) NSRect videoFrame;

#pragma mark - audio

@property (nonatomic, readonly) int audioFrequency;
@property (nonatomic, readonly) int audioChannels;

#pragma mark - image management

- (C64dtvImageType)imageTypeAtURL:(NSURL *)url;
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
