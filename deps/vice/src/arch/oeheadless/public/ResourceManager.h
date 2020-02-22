#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@class Resource;

@interface ResourceManager: NSObject

@property (class, readonly) ResourceManager *shared;

@end

NS_ASSUME_NONNULL_END