#import "api.h"
#import "C64dtv_archdep.h"
#import "C64dtv.h"
#import "resources.h"

int callback(int i, resource_ram_t *item, void *context)
{
    printf("%-30s: ", item->name);
    switch (item->type) {
        case RES_STRING: {
            char const *v = *(char const **)item->value_ptr;
            printf("%s", (v == NULL || strlen(v) == 0) ? "[EMPTY]" : v);
            break;
        }
        
        case RES_INTEGER:
            printf("%d", *(int *)item->value_ptr);
            break;
        
        default:
            printf("NONE");
    }
    printf("\n");
    return 0;
}

@interface Delegate: NSObject<C64dtvDelegate>
@property (nonatomic, readwrite) int written;
@end

@implementation Delegate

- (void)canvasWillResizeWidth:(NSUInteger)width height:(NSUInteger)height {

}


-(void)updateAudioBuffer:(int16_t const *)buffer samples:(NSInteger)samples {
    _written += samples;
}

@end

int main(int argc, char **argv)
{
    C64dtv *c64 = C64dtv.shared;
    NSString *bootPath = @SOURCE_ROOT;
    NSArray<NSString *> *paths = @[
            [NSString pathWithComponents:@[bootPath, @"data", @"C64"]],
            [NSString pathWithComponents:@[bootPath, @"data", @"DRIVES"]],
    ];
    Delegate *delegate = [Delegate new];
    c64.delegate = delegate;
    [c64 initializeWithBootPath:bootPath systemPathList:paths];
    
    c64.model = DTVModelV2PAL;
    printf("frequency %f\n", c64.videoFrequency);
    c64.model =  DTVModelV2NTSC;
    printf("frequency %f\n", c64.videoFrequency);
    c64.model = DTVModelV3PAL;
    printf("frequency %f\n", c64.videoFrequency);
    c64.model = DTVModelV3NTSC;
    printf("frequency %f\n", c64.videoFrequency);
    c64.model = DTVModelHummerNTSC;
    printf("frequency %f\n", c64.videoFrequency);
    
    delegate.written = 0;
    int frames = 0;
    for (int j=0; j<10000; j++) {
        [c64 execute];
        frames+=1;
        if (frames >= c64.videoFrequency) {
            frames = 0;
            printf("frames=%03d, written=%d\n", j, delegate.written);
            delegate.written = 0;
        }
    }
    
    
    
    // resource_enumerate(&callback, nil);
    
    return 0;
}
