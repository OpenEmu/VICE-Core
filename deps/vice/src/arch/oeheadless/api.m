#import "public/api.h"

@implementation C64

+ (C64 *)shared
{
    static C64 *shared;

    static dispatch_once_t once;
    dispatch_once(&once, ^{
        shared = [C64 new];
    });

    return shared;
}

@end