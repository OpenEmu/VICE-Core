#import "api.h"
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

int main(int argc, char **argv)
{
    C64 *c64 = C64.shared;
    NSString *bootPath = @SOURCE_ROOT;
    NSArray<NSString *> *paths = @[
            [NSString pathWithComponents:@[bootPath, @"data", @"C64"]],
            [NSString pathWithComponents:@[bootPath, @"data", @"DRIVES"]],
    ];
    [c64 initializeWithBootPath:bootPath systemPathList:paths];
    
    printf("frequency %f\n", c64.videoFrequency);
    c64.model = C64ModelCNTSC;
    printf("frequency %f\n", c64.videoFrequency);
    
    [c64 execute];
    NSData *data = [c64 serializeState];
    [c64 execute];
    [c64 deserializeState:data];
    [c64 execute];
    
    resource_enumerate(&callback, nil);
    
    return 0;
}