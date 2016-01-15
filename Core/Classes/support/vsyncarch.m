/*
 Copyright (c) 2016, OpenEmu Team
 
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


#import "videoarch.h"
#import <Foundation/Foundation.h>
#import <CoreVideo/CVHostTime.h>

#include <sys/time.h>

static unsigned long hostToUsFactor = 1000UL;

/* Number of timer units per second. */
signed long vsyncarch_frequency(void)
{
    hostToUsFactor = (unsigned long)(CVGetHostClockFrequency() / 1000000UL);

    /* Microseconds resolution. */
    return 1000000L;
}

/* Get time in timer units. */
unsigned long vsyncarch_gettime(void)
{
    return (unsigned long)(CVGetCurrentHostTime() / hostToUsFactor);
}

#pragma mark stubs

void vsyncarch_init(void)
{
}

void vsyncarch_display_speed(double speed, double frame_rate, int warp_enabled)
{
}

void vsyncarch_sleep(signed long delay)
{
    usleep((useconds_t)delay);
}