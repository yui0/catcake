/*
    Copyright ©2013 Yuichiro Nakada

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in
    all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
    THE SOFTWARE.
*/


#ifdef CK_ANDROID

#include "opensl_io.h"

#include "ck_low_level_api.h"

#include "ck_mem_all.h"

//#include <pthread.h>
/*#include <jni.h>
#include <android/log.h>
#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "catcake_sound", __VA_ARGS__))*/
#define LOGI(...)

static bool s_is_snd_dev_open = false;
volatile bool s_is_playing = false;
static u8 s_channel_num;
static u16 s_sample_rate;
static u16 s_snd_mix_buf_msec;
static u32 s_snd_mix_buf_size;
static u32 s_snd_mix_buf_sample_num;

ckLowLevelAPI::SoundMixFunction s_snd_mix_func;
void* s_snd_mix_buf[2] = { NULL, NULL };

//pthread_mutex_t s_snd_mix_mutex = PTHREAD_RECURSIVE_MUTEX_INITIALIZER_NP;
pthread_t s_snd_play_thread;

OPENSL_STREAM  *sl;


void* soundPlayThread(void*)
{
	while (s_is_playing) {
		if ((*s_snd_mix_func)(s_snd_mix_buf[0])) {
			android_AudioOut(sl, (short*)s_snd_mix_buf[0], s_snd_mix_buf_size/2);
		}
		if ((*s_snd_mix_func)(s_snd_mix_buf[1])) {
			android_AudioOut(sl, (short*)s_snd_mix_buf[1], s_snd_mix_buf_size/2);
		}
		/*if ((*s_snd_mix_func)(s_snd_mix_buf[0])) {
			if ((*s_snd_mix_func)(s_snd_mix_buf[1])) {
				// stereo
				android_AudioOut2(sl, (short*)s_snd_mix_buf[0], (short*)s_snd_mix_buf[1], s_snd_mix_buf_size/2);
			} else {
				// monoral
				android_AudioOut(sl, (short*)s_snd_mix_buf[0], s_snd_mix_buf_size/2);
			}
		}*/
		//pthread_yield();
	}
	pthread_exit(NULL);
}


bool ckLowLevelAPI::isSoundDeviceOpen()
{
    // TODO

    return s_is_snd_dev_open;
}


bool ckLowLevelAPI::openSoundDevice(u8 channel_num, u16 sample_rate, u16 snd_mix_buf_msec, SoundMixFunction snd_mix_func)
{
    LOGI("openSoundDevice");
    if (isSoundDeviceOpen())
    {
        closeSoundDevice();
    }

    s_snd_mix_func =  snd_mix_func;

    s_channel_num = channel_num;
    s_sample_rate = sample_rate;
    s_snd_mix_buf_msec = snd_mix_buf_msec;
    s_snd_mix_buf_size = channel_num * sample_rate * 2 * snd_mix_buf_msec / 1000;
    s_snd_mix_buf_sample_num = sample_rate * snd_mix_buf_msec / 1000;
    //s_bits_per_sample = 16;

    s_snd_mix_buf_size = 2 * channel_num * s_snd_mix_buf_sample_num;
    for (s32 i = 0; i < 2; i++)
    {
        s_snd_mix_buf[i] = ckMalloc(s_snd_mix_buf_size);
        ckMemMgr::memset(s_snd_mix_buf[i], 0, s_snd_mix_buf_size);
    }

    // TODO
    sl = android_OpenAudioDevice(sample_rate, 1, channel_num, s_snd_mix_buf_size);
    if (sl) {
	if (pthread_create(&s_snd_play_thread, NULL, soundPlayThread, NULL) != 0)
	{
	    s_is_playing = false;
	    closeSoundDevice();
	    LOGI("openSoundDevice failed!");
	    return false;
	}

	LOGI("openSoundDevice success!");
	s_is_playing = true;
	s_is_snd_dev_open = true;
	return true;
    }

    LOGI("openSoundDevice failed!");
    return false; // TODO
}


void ckLowLevelAPI::closeSoundDevice()
{
    // TODO
    if (s_is_playing)
    {
        s_is_playing = false;
        pthread_join(s_snd_play_thread, NULL);
    }

    for (s32 i = 0; i < 2; i++)
    {
        if (s_snd_mix_buf[i])
        {
            ckFree(s_snd_mix_buf[i]);
            s_snd_mix_buf[i] = NULL;
        }
    }

    android_CloseAudioDevice(sl);

    //pthread_mutex_destroy(&s_snd_mix_mutex);

    s_is_snd_dev_open = false;
}


u8 ckLowLevelAPI::getSoundDeviceChannelNum()
{
    return s_channel_num;
}


u16 ckLowLevelAPI::getSoundDeviceSampleRate()
{
    return s_sample_rate;
}


u16 ckLowLevelAPI::getSoundMixBufferMsec()
{
    return s_snd_mix_buf_msec;
}


u32 ckLowLevelAPI::getSoundMixBufferSize()
{
    return s_snd_mix_buf_size;
}


u32 ckLowLevelAPI::getSoundMixBufferSampleNum()
{
    return s_snd_mix_buf_sample_num;
}


void ckLowLevelAPI::lockSoundMixMutex()
{
    // TODO
}


void ckLowLevelAPI::unlockSoundMixMutex()
{
    // TODO
}


#endif // CK_ANDROID
