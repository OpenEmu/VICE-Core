#import <Foundation/Foundation.h>

@protocol C64Delegate <NSObject>

@property (nonatomic, readonly) NSString *bootPath;
@property (nonatomic, readonly) NSArray<NSString *> *sysfilePathList;

- (void)initSoundSpeed:(int *)speed fragSize:(int *)fragSize fragNumber:(int *)fragNumber channels:(int *)channels;
- (void)updateAudioBuffer:(int16_t const *)buffer samples:(NSInteger)samples;

@end