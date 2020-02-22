#import "public/ResourceManager.h"

@implementation ResourceManager

+ (ResourceManager *)shared {
    static ResourceManager *shared;
    
    static dispatch_once_t once;
    dispatch_once(&once, ^{
        shared = [ResourceManager new];
    });
    
    return shared;
}

@end