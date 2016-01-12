//
//  soundioscoreaudio.c
//  C64C
//
//  Created by Stuart Carnie on 11/26/11.
//  Copyright (c) 2011 __MyCompanyName__. All rights reserved.
//

#include "vice.h"

#include <AudioToolbox/AudioToolbox.h>
#include <CoreAudio/CoreAudio.h>
#include <libkern/OSAtomic.h>

#include "lib.h"
#include "log.h"
#include "sound.h"

/* type for atomic increments */
typedef volatile int32_t atomic_int_t;

/* the cyclic buffer containing m fragments */
static SWORD *soundbuffer;

/* silence fragment */
static SWORD *silence;

/* current read position: no. of fragment in soundbuffer */
static unsigned int read_position;

/* the next position to write: no. of fragment in soundbuffer */
static unsigned int write_position;

/* frames in fragment  */
static unsigned int frames_in_fragment;

/* Size of fragment (bytes).  */
static unsigned int bytes_in_fragment;

/* Size of fragment (SWORDs) */
static unsigned int swords_in_fragment;

/* total number of fragments */
static unsigned int fragment_count;

/* current number of fragments in buffer */
static atomic_int_t fragments_in_queue;

/* number of interleaved channels (mono SID=1, stereo SID=2) */
static int in_channels;

/* samples left in current fragment */
static unsigned int frames_left_in_fragment;

/* bytes per input frame */
static unsigned int in_frame_byte_size;

#include <AudioUnit/AudioUnit.h>

/* ----- Audio Converter ------------------------------------------------ */

static AudioConverterRef converter = 0;

static OSStatus converter_input(AudioConverterRef inAudioConverter,
                                UInt32 * ioNumberDataPackets,
                                AudioBufferList * ioData,
                                AudioStreamPacketDescription** outDataPacketDescription,
                                void * inUserData)
{
    UInt32 num_frames = *ioNumberDataPackets;
    
    SWORD *buffer;
    if (fragments_in_queue) {
        /* too many -> crop to available in current fragment */
        if(num_frames > frames_left_in_fragment) {
            num_frames = frames_left_in_fragment;
        }
        
        /* calc position in sound buffer */
        int sample_offset_in_fragment = frames_in_fragment - frames_left_in_fragment;
        buffer = soundbuffer + swords_in_fragment * read_position + sample_offset_in_fragment * in_channels;
        
        /* update the samples left in the current fragment */
        frames_left_in_fragment -= num_frames;
        
        /* fetch next fragment */
        if(frames_left_in_fragment == 0) {
            read_position = (read_position + 1) % fragment_count;
            OSAtomicDecrement32(&fragments_in_queue);
            frames_left_in_fragment = frames_in_fragment;
        }
    } else {
        if(num_frames > frames_in_fragment) {
            num_frames = frames_in_fragment;
        }
        
        /* output silence */
        buffer = silence;
    }
    
    /* prepare return buffer */
    ioData->mBuffers[0].mDataByteSize = num_frames * in_frame_byte_size;
    ioData->mBuffers[0].mData = buffer;
    *ioNumberDataPackets = num_frames;
    
    return noErr;
}

static int converter_open(AudioStreamBasicDescription *in,
                          AudioStreamBasicDescription *out)
{
    OSStatus err;
    
    /* need to to sample rate conversion? */
    if (out->mSampleRate != in->mSampleRate) {
        log_warning(LOG_DEFAULT, "sound (coreaudio_init): sampling rate conversion %dHz->%dHz",
                    (int)in->mSampleRate, (int)out->mSampleRate);
    }
    
    /* create a new audio converter */
    err = AudioConverterNew(in, out, &converter);
    if (err != noErr) {
        log_error(LOG_DEFAULT,
                  "sound (coreaudio_init): could not create AudioConverter: err=%d", (int)err);
        return -1;
    }
    
    /* duplicate mono stream to all output channels */
    if (in->mChannelsPerFrame == 1 && out->mChannelsPerFrame > 1) {
        Boolean writable;
        UInt32 size;
        err = AudioConverterGetPropertyInfo(converter, kAudioConverterChannelMap, &size, &writable);
        if (err == noErr && writable) {
            SInt32 * channel_map = lib_malloc(size);
            if (channel_map) {
                memset(channel_map, 0, size);
                AudioConverterSetProperty(converter, kAudioConverterChannelMap, size, channel_map);
                lib_free(channel_map);
            }
        }
    }
    
    return 0;
}

static void converter_close(void)
{
    if (converter) {
        AudioConverterDispose(converter);
        converter = NULL;
    }
}

static AudioUnit outputUnit;

static OSStatus	audio_render(void *inRefCon,
                             AudioUnitRenderActionFlags  *ioActionFlags,
                             const AudioTimeStamp        *inTimeStamp,
                             UInt32                      inBusNumber,
                             UInt32                      inNumberFrames,
                             AudioBufferList             *ioData)
{
    UInt32 numFrames = inNumberFrames;
    return AudioConverterFillComplexBuffer(converter,
                                           converter_input,
                                           NULL,
                                           &numFrames,
                                           ioData,
                                           NULL);
}

#define kOutputBus 0
#define kInputBus 1

static AudioUnit outputUnit;

static int audio_open(AudioStreamBasicDescription *in)
{
    OSStatus status;
    AudioComponentDescription desc;
    AudioStreamBasicDescription out;
    UInt32 size;
    
    /* find the default audio component */
    desc.componentType = kAudioUnitType_Output;
#if !TARGET_OS_IPHONE
    desc.componentSubType = kAudioUnitSubType_DefaultOutput;
#else
    desc.componentSubType = kAudioUnitSubType_RemoteIO;
#endif
    desc.componentManufacturer = kAudioUnitManufacturer_Apple;
    desc.componentFlags = 0;
    desc.componentFlagsMask = 0;
    
    AudioComponent comp = AudioComponentFindNext(NULL, &desc);
    if (comp == NULL) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): can't find next component");
        return -1;
    }
    
    /* open audio component */
    status = AudioComponentInstanceNew(comp, &outputUnit);
    if (status) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): error opening output unit");
        return -1;
    }
    
#if TARGET_OS_IPHONE
    // Enable IO for playback
    UInt32 flag = 1;
    status = AudioUnitSetProperty(outputUnit,
                                  kAudioOutputUnitProperty_EnableIO,
                                  kAudioUnitScope_Output,
                                  kOutputBus,
                                  &flag,
                                  sizeof(flag));
    if (status) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): error initializing audio unit for playback");
        return -1;
    }
#endif
    
    // Apply format
    status = AudioUnitSetProperty(outputUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  kOutputBus,
                                  in,
                                  sizeof(AudioStreamBasicDescription));
    if (status) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): error setting stream format for output");
        //return -1;
    }
    
    /* Set up a callback function to generate output to the output unit */
    AURenderCallbackStruct input;
    input.inputProc = audio_render;
    input.inputProcRefCon = NULL;
    status = AudioUnitSetProperty(outputUnit,
                                  kAudioUnitProperty_SetRenderCallback,
                                  kAudioUnitScope_Global,
                                  0,
                                  &input,
                                  sizeof(input));
    if (status) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): error setting render callback");
        return -1;
    }
    
    /* Get output properties */
    size = sizeof(AudioStreamBasicDescription);
    status = AudioUnitGetProperty(outputUnit,
                                  kAudioUnitProperty_StreamFormat,
                                  kAudioUnitScope_Input,
                                  kOutputBus,
                                  &out,
                                  &size);
    if (status) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): error setting desired input format");
        return -1;
    }
    
    /* Init unit */
    status = AudioUnitInitialize(outputUnit);
    if (status) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_init): error initializing audio unit");
        return -1;
    }
    
    /* open converter */
    return converter_open(in, &out);
}

static void audio_close(void)
{
    OSStatus err;
    
    converter_close();
    
    /* Uninit unit */
    err = AudioUnitUninitialize(outputUnit);
    if (err) {
        log_error(LOG_DEFAULT,
                  "sound (audiounit_close): error uninitializing audio unit");
    }
}

static int audio_start(void)
{
    OSStatus err = AudioOutputUnitStart(outputUnit);
    if(err)
        return -1;
    else
        return 0;
}

static int audio_stop(void)
{
    OSStatus err = AudioOutputUnitStop(outputUnit);
    if(err)
        return -1;
    else
        return 0;
}


/* ---------- coreaudio VICE API ------------------------------------------ */

static int coreaudio_resume(void);

static int coreaudio_init(const char *param, int *speed,
                          int *fragsize, int *fragnr, int *channels)
{
    AudioStreamBasicDescription in;
    int result;
    
    /* store fragment parameters */
    fragment_count     = *fragnr;
    frames_in_fragment = *fragsize;
    in_channels        = *channels;
    
    /* the size of a fragment in bytes and SWORDs */
    swords_in_fragment = frames_in_fragment * in_channels;
    bytes_in_fragment  = swords_in_fragment * sizeof(SWORD);
    
    /* the size of a sample */
    in_frame_byte_size = sizeof(SWORD) * in_channels;
    
    /* allocate sound buffers */
    soundbuffer = lib_calloc(fragment_count, bytes_in_fragment);
    silence = lib_calloc(1, bytes_in_fragment);
    
    /* define desired input format */
    in.mChannelsPerFrame = *channels;
    in.mSampleRate = (float)*speed;
    in.mFormatID = kAudioFormatLinearPCM;
    in.mFormatFlags = kLinearPCMFormatFlagIsSignedInteger | kLinearPCMFormatFlagIsPacked;
    in.mBytesPerFrame = sizeof(SWORD) * *channels;
    in.mBytesPerPacket = in.mBytesPerFrame;
    in.mFramesPerPacket = 1;
    in.mBitsPerChannel = 8 * sizeof(SWORD);
    in.mReserved = 0;
    
    /* setup audio device */
    result = audio_open(&in);
    if(result < 0)
        return result;
    
    coreaudio_resume();
    
    return 0;
}

static int coreaudio_write(SWORD *pbuf, size_t nr)
{
    int i, count;
    
    /* number of fragments */
    count = nr / swords_in_fragment;
    
    for (i = 0; i < count; i++)
    {
        if (fragments_in_queue == fragment_count) {
            log_warning(LOG_DEFAULT, "sound (audiounit): buffer overrun");
            return -1;
        }
        
        memcpy(soundbuffer + swords_in_fragment * write_position,
               pbuf + i * swords_in_fragment,
               bytes_in_fragment);
        
        write_position = (write_position + 1) % fragment_count;
        
        OSAtomicIncrement32(&fragments_in_queue);
    }
    
    return 0;
}

static int coreaudio_bufferspace(void)
{
    return (fragment_count - fragments_in_queue) * frames_in_fragment;
}

static void coreaudio_close(void)
{
    audio_close();
    
    lib_free(soundbuffer);
    lib_free(silence);
}

static int coreaudio_suspend(void)
{
    int result = audio_stop();
    if(result < 0) {
        log_error(LOG_DEFAULT, "sound (audiounit_suspend): could not stop audio");
    }
    return result;
}

static int coreaudio_resume(void)
{
    int result;
    
    /* reset buffers before resume */
    read_position = 0;
    write_position = 0;
    fragments_in_queue = 0;
    frames_left_in_fragment = frames_in_fragment;
    
    result = audio_start();
    if(result < 0) {
        log_error(LOG_DEFAULT, "sound (audiounit_resume): could not start audio");
    }
    return result;
}

static sound_device_t audiounit_device =
{
    "audiounit",
    coreaudio_init,
    coreaudio_write,
    NULL,
    NULL,
    coreaudio_bufferspace,
    coreaudio_close,
    coreaudio_suspend,
    coreaudio_resume,
    1,
    2
};

int sound_init_audiounit_device(void) {
    return sound_register_device(&audiounit_device);
}