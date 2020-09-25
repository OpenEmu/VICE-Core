#import "api.h"
#import "Vic20_archdep.h"
#import "Vic20.h"
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

@interface Delegate: NSObject<Vic20Delegate>
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
    Vic20 *vic20 = Vic20.shared;
    NSString *bootPath = @SOURCE_ROOT;
    NSArray<NSString *> *paths = @[
            [NSString pathWithComponents:@[bootPath, @"data", @"VIC20"]],
            [NSString pathWithComponents:@[bootPath, @"data", @"DRIVES"]],
    ];
    Delegate *delegate = [Delegate new];
    vic20.delegate = delegate;
    [vic20 initializeWithBootPath:bootPath systemPathList:paths];
    
    printf("frequency %f\n", vic20.videoFrequency);
    vic20.model = Vic20ModelPAL;
    printf("frequency %f\n", vic20.videoFrequency);
    vic20.model = Vic20ModelNTSC;
    printf("frequency %f\n", vic20.videoFrequency);
    
    delegate.written = 0;
    int frames = 0;
    for (int j=0; j<10000; j++) {
        [vic20 execute];
        frames+=1;
        if (frames >= vic20.videoFrequency) {
            frames = 0;
            printf("frames=%03d, written=%d\n", j, delegate.written);
            delegate.written = 0;
        }
    }
    
    
    
    // resource_enumerate(&callback, nil);
    
    return 0;
}
