/*
 Copyright (c) 2013, OpenEmu Team


 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions are met:
     * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.
     * Redistributions in binary form must reproduce the above copyright
       notice, this list of conditions and the following disclaimer in the
       documentation and/or other materials provided with the distribution.
     * Neither the name of the OpenEmu Team nor the
       names of its contributors may be used to endorse or promote products
       derived from this software without specific prior written permission.

 THIS SOFTWARE IS PROVIDED BY OpenEmu Team ''AS IS'' AND ANY
 EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 DISCLAIMED. IN NO EVENT SHALL OpenEmu Team BE LIABLE FOR ANY
 DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#import "VICEGameCore.h"
#import "OEComputerSystemResponderClient.h"
#import <OpenEmuBase/OERingBuffer.h>
#import <OpenGL/gl.h>

#include "autostart.h"
#include "interrupt.h"
#include "log.h"
#include "machine.h"
#include "main.h"
//#include "maincpu.h"
#include "resources.h"

@interface VICEGameCore () <OEComputerSystemResponderClient>

@end

@implementation VICEGameCore

#pragma mark Execution

- (BOOL)loadFileAtPath:(NSString *)path
{
    // enter VICE main program and main loop
    int argc = 1;
    char **argv = (char **)malloc(sizeof(char *) * argc);
    main_program(argc, argv);
    
    // Set True Drive Emulation (TDE) for compatibility mode
    //resources_set_int("DriveTrueEmulation", 1);
    
    if (autostart_autodetect([path UTF8String], NULL, 0, AUTOSTART_MODE_RUN) < 0) {
        //ui_error(_("Unknown image type"));
        //log_warning (LOG_DEFAULT, "autostart_autodetect failed for image : '%s'", current_rom);
        return NO;
    }

    return YES;
}

- (void)executeFrame
{
    [self executeFrameSkippingFrame:NO];
}

- (void)executeFrameSkippingFrame:(BOOL)skip
{
    
}

- (void)resetEmulation
{
    machine_trigger_reset(MACHINE_RESET_MODE_HARD);
    //machine_trigger_reset(MACHINE_RESET_MODE_SOFT);
}

- (void)stopEmulation
{
    //main_exit();
    machine_shutdown();
}

#pragma mark Video

- (OEIntSize)bufferSize
{
    return OEIntSizeMake(320, 200);
}

- (const void *)videoBuffer
{
    return NULL;
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

- (NSTimeInterval)frameInterval
{
    return 60;
}

#pragma mark Audio

- (NSUInteger)channelCount
{
    return 1;
}

- (double)audioSampleRate
{
    return 48000;
}

#pragma mark Save States

- (BOOL)saveStateToFileAtPath:(NSString *)fileName
{
    //machine_write_snapshot
    //interrupt_maincpu_trigger_trap(saveSnapshotTrap, NULL);
    return NO;
}

- (BOOL)loadStateFromFileAtPath:(NSString *)fileName
{
    //machine_read_snapshot
    //interrupt_maincpu_trigger_trap(loadSnapshotTrap, NULL);
    return NO;
}

#pragma mark Input

- (void)leftMouseUp
{}

- (void)leftMouseDown
{}

- (void)rightMouseUp
{}

- (void)rightMouseDown
{}

@end
