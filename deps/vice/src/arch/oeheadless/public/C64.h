#import <Foundation/Foundation.h>
#import "oecommon.h"

NS_ASSUME_NONNULL_BEGIN

@protocol C64Delegate;

typedef NS_ENUM(NSUInteger, C64ImageType)
{
    C64ImageTypeUnknown,
    C64ImageTypeDisk,
    C64ImageTypeTape,
    C64ImageTypeCartridge,
    C64ImageTypePRG,
};

typedef NS_ENUM(NSUInteger, C64Model)
{
    C64ModelPAL     = 0,
    C64ModelCPAL    = 1,
    C64ModelNTSC    = 3,
    C64ModelCNTSC   = 4,
};

OE_EXPORTED_CLASS
@interface C64: NSObject

@property (class, nonatomic, readonly) C64 *shared;

@property (nonatomic, readonly) id<C64Delegate> delegate;
- (void)initializeWithDelegate:(id<C64Delegate>)delegate;

#pragma mark - configuration

@property (nonatomic) C64Model model;
@property (nonatomic, copy) NSString *bootPath;

#pragma mark - image management

- (C64ImageType)imageTypeAtURL:(NSURL *)url;
- (BOOL)attachImageAtURL:(NSURL *)url error:(NSError **)error;
- (BOOL)autostartImageAtURL:(NSURL *)url error:(NSError **)error;

#pragma mark - save / load save state

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName error:(NSError **)error;
- (BOOL)saveStateFromFileAtPath:(NSString *)fileName error:(NSError **)error;

#pragma mark - serialization

/*!
 * Returns the number of bytes required to store a save state
 */
- (NSUInteger)stateSize;
- (NSData *)serializeState;
- (BOOL)deserializeState:(NSData *)data;

#pragma mark - execution

- (BOOL)execute;
- (void)setBuffer:(void *)buffer size:(NSSize)size;

@end

NS_ASSUME_NONNULL_END