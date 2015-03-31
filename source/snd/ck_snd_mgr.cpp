/*
    Copyright (c) 2007-2010 Takashi Kitao

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


#include "ck_snd_all.h"

#include "ck_math_all.h"
#include "ck_sys_all.h"
#include "ck_task_all.h"
#include "ck_res_all.h"
#include "ck_low_level_api.h"
#include "ck_private_macro.h"


ckSndMgr* ckSndMgr::m_instance = NULL;


CK_DEFINE_MANAGER_IS_CREATED(ckSndMgr)


CK_DEFINE_MANAGER_CREATE(ckSndMgr, AfterRes, BeforeRes)


CK_DEFINE_MANAGER_DESTROY(ckSndMgr, BeforeRes)


bool ckSndMgr::isSoundDeviceOpen()
{
    instance();

    return ckLowLevelAPI::isSoundDeviceOpen();
}


bool ckSndMgr::openSoundDevice(ChannelNum channel_num, SampleRate sample_rate, u16 snd_mix_buf_msec)
{
    instance();

    if (snd_mix_buf_msec == 0)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (ckLowLevelAPI::openSoundDevice(channel_num, sample_rate, snd_mix_buf_msec, soundMixFunction))
    {
        return true;
    }
    else
    {
        ckLowLevelAPI::printf("*** can't open sound device ***");

        return false;
    }
}


void ckSndMgr::closeSoundDevice()
{
    instance();

    if (ckLowLevelAPI::isSoundDeviceOpen())
    {
        ckLowLevelAPI::closeSoundDevice();
    }
}


ckSndMgr::ChannelNum ckSndMgr::getSoundDeviceChannelNum()
{
    instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return CHANNEL_NUM_MONO;
    }

    return static_cast<ChannelNum>(ckLowLevelAPI::getSoundDeviceChannelNum());
}


ckSndMgr::SampleRate ckSndMgr::getSoundDeviceSampleRate()
{
    instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return SAMPLE_RATE_11KHZ;
    }

    return static_cast<SampleRate>(ckLowLevelAPI::getSoundDeviceSampleRate());
}


u16 ckSndMgr::getSoundMixBufferMsec()
{
    instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    return ckLowLevelAPI::getSoundMixBufferMsec();
}


ckSndMgr::ChannelNum ckSndMgr::getChannelNum(ckID snd_id)
{
    instance();

    if (snd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(snd_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV" && res.getExtension() != "MP3")
    {
        ckThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->channel_num.getType();
}


ckSndMgr::SampleRate ckSndMgr::getSampleRate(ckID snd_id)
{
    instance();

    if (snd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(snd_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV" && res.getExtension() != "MP3")
    {
        ckThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->sample_rate.getType();
}


u32 ckSndMgr::getSampleNum(ckID snd_id)
{
    instance();

    if (snd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(snd_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (res.getExtension() != "WAV" && res.getExtension() != "MP3")
    {
        ckThrow(ExceptionInvalidType);
    }

    return res.getExInfo<SoundInfo>()->sample_num;
}


u8 ckSndMgr::getTrackVolume(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    u8 trk_vol;

    ckLowLevelAPI::lockSoundMixMutex();
    {
        trk_vol = static_cast<u8>(ins->m_trk_info[trk_no].trk_vol);
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    return trk_vol;
}


void ckSndMgr::setTrackVolume(u8 trk_no, u8 trk_vol)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::lockSoundMixMutex();
    {
        ins->m_trk_info[trk_no].trk_vol = static_cast<u8>(trk_vol);
        ins->m_trk_info[trk_no].fade_frame_count = 0;
    }
    ckLowLevelAPI::unlockSoundMixMutex();
}


void ckSndMgr::fadeTrackVolume(u8 trk_no, u8 to_trk_vol, r32 delta_per_frame)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM || delta_per_frame < ckMath::EPSILON)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::lockSoundMixMutex();
    {
        TrackInfo* trk_info = &ins->m_trk_info[trk_no];
        u32 fade_frame_count = static_cast<u32>(ckMath::abs<r32>(to_trk_vol - trk_info->trk_vol) / delta_per_frame);

        if (fade_frame_count > 0)
        {
            trk_info->fade_to_trk_vol = to_trk_vol;
            trk_info->fade_frame_count = fade_frame_count;
            trk_info->fade_delta_per_frame = (to_trk_vol > trk_info->trk_vol) ? delta_per_frame : -delta_per_frame;
        }
        else
        {
            trk_info->trk_vol = to_trk_vol;
        }
    }
    ckLowLevelAPI::unlockSoundMixMutex();
}


bool ckSndMgr::isPlaying(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return false;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    bool is_playing;

    ckLowLevelAPI::lockSoundMixMutex();
    {
        is_playing = (ins->m_trk_info[trk_no].trk_state != STATE_STOP);
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    return is_playing;
}


void ckSndMgr::play(u8 trk_no, ckID snd_id, u8 snd_vol, bool is_play_loop, u32 play_pos)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM || snd_id == ckID::ZERO)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckRes res;

    ckTry
    {
        res = ckResMgr::getResource(snd_id);
    }
    ckCatch(ckResMgr::ExceptionNotFound)
    {
        ckThrow(ExceptionNotFound);
    }

    if (res.getExtension() != "WAV" && res.getExtension() != "MP3")
    {
        ckThrow(ExceptionInvalidType);
    }

    SoundInfo* snd_info = res.getExInfo<SoundInfo>();

    if (play_pos >= snd_info->sample_num)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    TrackInfo* trk_info = &ins->m_trk_info[trk_no];

    ckLowLevelAPI::lockSoundMixMutex();
    {
        trk_info->trk_state = is_play_loop ? STATE_PLAY_LOOP : STATE_PLAY_ONESHOT;
        trk_info->snd_id = snd_id;
        trk_info->snd_vol = snd_vol;
        trk_info->snd_info = snd_info;
        trk_info->play_pos = play_pos;
    }
    ckLowLevelAPI::unlockSoundMixMutex();
}


void ckSndMgr::stop(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    ckLowLevelAPI::lockSoundMixMutex();
    {
        ins->m_trk_info[trk_no].trk_state = STATE_STOP;
    }
    ckLowLevelAPI::unlockSoundMixMutex();
}


ckID ckSndMgr::getPlayingSoundID(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return ckID::ZERO;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        ckThrow(ExceptionInvalidCall);
    }

    ckID snd_id;

    ckLowLevelAPI::lockSoundMixMutex();
    {
        snd_id = ins->m_trk_info[trk_no].snd_id;
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    return snd_id;
}


u8 ckSndMgr::getPlayingSoundVolume(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        ckThrow(ExceptionInvalidCall);
    }

    u8 snd_vol;

    ckLowLevelAPI::lockSoundMixMutex();
    {
        snd_vol = ins->m_trk_info[trk_no].snd_vol;
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    return snd_vol;
}


bool ckSndMgr::isPlayingLoop(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return false;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        ckThrow(ExceptionInvalidCall);
    }

    bool is_playing_loop;

    ckLowLevelAPI::lockSoundMixMutex();
    {
        is_playing_loop = (ins->m_trk_info[trk_no].trk_state == STATE_PLAY_LOOP);
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    return is_playing_loop;
}


u32 ckSndMgr::getPlayingPosition(u8 trk_no)
{
    ckSndMgr* ins = instance();

    if (!ckLowLevelAPI::isSoundDeviceOpen())
    {
        return 0;
    }

    if (trk_no >= TRACK_NUM)
    {
        ckThrow(ExceptionInvalidArgument);
    }

    if (!isPlaying(trk_no))
    {
        ckThrow(ExceptionInvalidCall);
    }

    u32 play_pos;

    ckLowLevelAPI::lockSoundMixMutex();
    {
        play_pos = ins->m_trk_info[trk_no].play_pos;
    }
    ckLowLevelAPI::unlockSoundMixMutex();

    return play_pos;
}


class ckSndDaemon : public ckTask
{
public:
    ckSndDaemon() : ckTask(ORDER_PLUS_8_FOR_SYSTEM) {}

    virtual void onUpdate()
    {
        ckSndMgr* ins = ckSndMgr::instance();

        for (u32 i = 0; i < ckSndMgr::TRACK_NUM; i++)
        {
            ckSndMgr::TrackInfo* trk_info = &ins->m_trk_info[i];

            if (trk_info->fade_frame_count > 0)
            {
                trk_info->trk_vol = ckMath::clamp(trk_info->trk_vol + trk_info->fade_delta_per_frame, 0.0f, 255.0f);
                trk_info->fade_frame_count--;

                if (trk_info->fade_frame_count == 0)
                {
                    trk_info->trk_vol = trk_info->fade_to_trk_vol;
                }
            }
        }
    }
};


ckSndMgr::ckSndMgr()
{
    for (u32 i = 0; i < TRACK_NUM; i++)
    {
        TrackInfo* trk_info = &m_trk_info[i];

        trk_info->trk_state = STATE_STOP;
        trk_info->trk_vol = 192.0f;
        trk_info->fade_frame_count = 0;
    }

    m_snd_daemon = ckNewTask(ckSndDaemon);

    ckResMgr::addType("WAV", waveInitializer, waveFinalizer);
    ckResMgr::addType("MP3", mp3Initializer, mp3Finalizer);
}


ckSndMgr::~ckSndMgr()
{
    if (ckTaskMgr::isCreated())
    {
        ckDeleteTask(m_snd_daemon);
    }

    ckLowLevelAPI::closeSoundDevice();

    for (const ckRes* res = ckResMgr::getFirstResourceN(); res; res = ckResMgr::getNextResourceN(res->getID()))
    {
        if (res->getExtension() == "WAV")
        {
            waveFinalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
        if (res->getExtension() == "MP3")
        {
            mp3Finalizer(res->getID(), res->getExtension(), res->getData<void>(), res->getDataSize(), res->getExInfo<void>());
        }
    }

    ckResMgr::removeType("MP3");
    ckResMgr::removeType("WAV");
}


CK_DEFINE_OPERATOR_EQUAL(ckSndMgr)


CK_DEFINE_MANAGER_INSTANCE(ckSndMgr)


void ckSndMgr::waveInitializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
    const u8* buf = static_cast<const u8*>(data);

    if (buf[0] != 'R' || buf[1] != 'I' || buf[2] != 'F' || buf[3] != 'F' || //
        buf[8] != 'W' || buf[9] != 'A' || buf[10] != 'V' || buf[11] != 'E')
    {
        ckThrow(ExceptionWaveInitializerFailed);
    }

    buf += 12;

    bool is_fmt_chunk = false;
    bool is_data_chunk = false;
    const u8* end = buf + data_size - 1;
    SoundInfo* snd_info = ckNew(SoundInfo);
    u16 bits_per_sample = 0;

    while (!is_fmt_chunk || !is_data_chunk)
    {
        u32 chunk_size = ckReadLittleEndian(buf + 4, u32);

        if (buf + 8 + chunk_size > end)
        {
            ckDelete(snd_info, SoundInfo);
            ckThrow(ExceptionWaveInitializerFailed);
        }

        if (buf[0] == 'f' && buf[1] == 'm' && buf[2] == 't' && buf[3] == ' ')
        {
            if (ckReadLittleEndian(buf + 8, u16) != 1)
            {
                ckDelete(snd_info, SoundInfo);
                ckThrow(ExceptionWaveInitializerFailed);
            }

            is_fmt_chunk = true;

            snd_info->channel_num = static_cast<ChannelNum>(ckReadLittleEndian(buf + 10, u16));
            snd_info->sample_rate = static_cast<SampleRate>(ckReadLittleEndian(buf + 12, u32));
            bits_per_sample = ckReadLittleEndian(buf + 22, u16);
        }
        else if (buf[0] == 'd' && buf[1] == 'a' && buf[2] == 't' && buf[3] == 'a')
        {
            is_data_chunk = true;

            snd_info->data = buf + 8;
            snd_info->data_size = chunk_size;
        }

        buf += 8 + chunk_size;
    }

    if ((snd_info->channel_num != CHANNEL_NUM_MONO && snd_info->channel_num != CHANNEL_NUM_STEREO) || //
        (snd_info->sample_rate != SAMPLE_RATE_11KHZ && snd_info->sample_rate != SAMPLE_RATE_22KHZ && snd_info->sample_rate != SAMPLE_RATE_44KHZ) || //
        bits_per_sample != 16 || snd_info->data_size == 0)
    {
        ckThrow(ExceptionWaveInitializerFailed);
    }

    snd_info->sample_num = snd_info->data_size / (snd_info->channel_num.getValue() * 2);

    *exinfo = snd_info;
}


void ckSndMgr::waveFinalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
    ckDelete(exinfo, SoundInfo);
}


//#include <stdio.h>
#include <mad.h>
struct mp3_info {
	unsigned char const *start;
	unsigned long length;
	struct mad_pcm *pcm;
	short *sample, *sample_pos;
};

static enum mad_flow input(void *data, struct mad_stream *stream)
{
	struct mp3_info *buffer = (struct mp3_info *)data;

	if (!buffer->length) return MAD_FLOW_STOP;

	mad_stream_buffer(stream, buffer->start, buffer->length);

	buffer->length = 0;

	return MAD_FLOW_CONTINUE;
}

static inline signed int scale(mad_fixed_t sample)
{
	/* round */
	sample += (1L << (MAD_F_FRACBITS - 16));

	/* clip */
	if (sample >= MAD_F_ONE)
		sample = MAD_F_ONE - 1;
	else if (sample < -MAD_F_ONE)
		sample = -MAD_F_ONE;

	/* quantize */
	return sample >> (MAD_F_FRACBITS + 1 - 16);
}

static enum mad_flow output(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
	unsigned int nchannels, nsamples;
	mad_fixed_t const *left_ch, *right_ch;

	//printf("output %d\n", pcm->length);
	struct mp3_info *m = (struct mp3_info *)data;
	m->pcm = pcm;

	/* pcm->samplerate contains the sampling frequency */
	nchannels = pcm->channels;
	nsamples  = pcm->length;
	left_ch   = pcm->samples[0];
	right_ch  = pcm->samples[1];

	while (nsamples--) {
		//signed int sample;

		/* output sample(s) in 16-bit signed little-endian PCM */
		/*sample = scale(*left_ch++);
		putchar((sample >> 0) & 0xff);
		putchar((sample >> 8) & 0xff);*/
		*m->sample_pos++ = scale(*left_ch++);

		if (nchannels == 2) {
			/*sample = scale(*right_ch++);
			putchar((sample >> 0) & 0xff);
			putchar((sample >> 8) & 0xff);*/
			*m->sample_pos++ = scale(*right_ch++);
		}
	}

	return MAD_FLOW_CONTINUE;
}

static enum mad_flow error(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
	/*struct mp3_info *buffer = (struct mp3_info *)data;

	fprintf(stderr, "decoding error 0x%04x (%s) at byte offset %u\n",
		stream->error, mad_stream_errorstr(stream),
		stream->this_frame - buffer->start);*/

	/* return MAD_FLOW_BREAK here to stop decoding (and propagate an error) */
	return MAD_FLOW_CONTINUE;
}

#define ckRealloc(ptr, size) ckMemMgr::reallocForSystem(ptr, size, 0, __FILE__)
void ckSndMgr::mp3Initializer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void** exinfo)
{
	struct mp3_info m;
	struct mad_decoder decoder;
	//int result;

	m.start  = (unsigned char const *)data;
	m.length = data_size;
	//m.snd_info = ckNew(SoundInfo);
	m.sample = m.sample_pos = (short*)ckMalloc(1024*1024*10*2);	// 20MB

	mad_decoder_init(&decoder, &m,
		input, 0 /* header */, 0 /* filter */, output,
		error, 0 /* message */);

	/*result = */mad_decoder_run(&decoder, MAD_DECODER_MODE_SYNC);

	mad_decoder_finish(&decoder);

	m.pcm->length = m.sample_pos - m.sample;
	//printf("size %d\n", m.sample_pos - m.sample);

	SoundInfo* snd_info = ckNew(SoundInfo);
	snd_info->channel_num = static_cast<ChannelNum>(m.pcm->channels);
	snd_info->sample_rate = static_cast<SampleRate>(m.pcm->samplerate);
	//snd_info->data = ckMalloc(m.pcm->length * 2);
	//snd_info->data_size = m.pcm->length;
	snd_info->data_size = (m.sample_pos - m.sample) *2 /* *2 */;
	//snd_info->data = m.sample;
	snd_info->data = ckRealloc(m.sample, snd_info->data_size);
	snd_info->sample_num = snd_info->data_size / (snd_info->channel_num.getValue() * 2);

	/*if ((snd_info->channel_num != CHANNEL_NUM_MONO && snd_info->channel_num != CHANNEL_NUM_STEREO) || //
		(snd_info->sample_rate != SAMPLE_RATE_11KHZ && snd_info->sample_rate != SAMPLE_RATE_22KHZ && snd_info->sample_rate != SAMPLE_RATE_44KHZ) || //
		bits_per_sample != 16 || snd_info->data_size == 0)
	{
		ckThrow(ExceptionWaveInitializerFailed);
	}*/

	*exinfo = snd_info;
}


void ckSndMgr::mp3Finalizer(ckID id, ckStr<char, 3> ext, const void* data, u32 data_size, void* exinfo)
{
	SoundInfo *snd_info = static_cast<SoundInfo*>(exinfo);
	ckFree((void*)snd_info->data);
	ckDelete(exinfo, SoundInfo);
}
