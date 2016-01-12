//
//  ViceGameCore.m
//  C64C
//
//  Created by Stuart Carnie on 1/11/16.
//  Copyright © 2016 Stuart Carnie. All rights reserved.
//

#import "ViceGameCore.h"
#import "OEC64SystemResponderClient.h"
#import <OpenGL/gl.h>

@interface ViceGameCore() <OEC64SystemResponderClient>
{
    uint32_t* videoBuffer;
}
@end

@implementation ViceGameCore

- (id)init
{
    if((self = [super init]))
    {
        if(videoBuffer)
            free(videoBuffer);
        videoBuffer = (uint32_t*)malloc(160 * 256 * 4);
    }
    
    return self;
}

#pragma emulation

- (void)resetEmulation
{
    
}

- (void)executeFrame
{
    
}

- (BOOL)loadFileAtPath:(NSString *)path
{
    return YES;
}

#pragma mark Video

- (const void *)videoBuffer
{
    return videoBuffer;
}

- (OEIntSize)bufferSize
{
    return OEIntSizeMake(160, 256);
}

- (GLenum)pixelFormat
{
    return GL_BGRA;
}

- (GLenum)pixelType
{
    return GL_UNSIGNED_INT_8_8_8_8_REV;
}

- (GLenum)internalPixelFormat
{
    return GL_RGB8;
}

- (double)audioSampleRate
{
    return 31400;
}

- (NSUInteger)channelCount
{
    return 2;
}

#pragma responder

- (oneway void)mouseMovedAtPoint:(OEIntPoint)point
{
    
}

- (oneway void)leftMouseDownAtPoint:(OEIntPoint)point
{
    
}
- (oneway void)leftMouseUp
{
    
}
- (oneway void)rightMouseDownAtPoint:(OEIntPoint)point
{
    
}
- (oneway void)rightMouseUp
{
    
}
- (oneway void)keyDown:(unsigned short)keyCode characters:(NSString *)characters charactersIgnoringModifiers:(NSString *)charactersIgnoringModifiers flags:(NSEventModifierFlags)flags
{
    
}
- (oneway void)keyUp:(unsigned short)keyCode characters:(NSString *)characters charactersIgnoringModifiers:(NSString *)charactersIgnoringModifiers flags:(NSEventModifierFlags)flags
{
    
}
- (oneway void)didPushC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    
}
- (oneway void)didReleaseC64Button:(OEC64Button)button forPlayer:(NSUInteger)player
{
    
}


@end
