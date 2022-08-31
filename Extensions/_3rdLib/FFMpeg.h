#pragma once

// Ref: https://github.com/leandromoreira/ffmpeg-libav-tutorial

#pragma warning(disable : 4819)
#pragma warning(disable : 4996)

#pragma comment(lib,"avcodec.lib")
#pragma comment(lib,"avformat.lib")
#pragma comment(lib,"avutil.lib")
#pragma comment(lib,"swscale.lib")

#include <string>
#include <functional>

#include "WindowsCommon.h"

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/imgutils.h>
#include <libswscale/swscale.h>
#include <libswresample/swresample.h>
}

class FFMpeg;

#include <SDL.h>
#include <SDL_thread.h>

// SDL
constexpr auto SDL_AUDIO_BUFFER_SIZE = 1024;
constexpr auto MAX_AUDIO_FRAME_SIZE = 192000;

constexpr auto MAX_AUDIOQ_SIZE = (5 * 16 * 1024);
constexpr auto MAX_VIDEOQ_SIZE = (5 * 256 * 1024);

constexpr auto SDL_EXCEPTION_AUDIO = 0;

static int quit = 0;

//���ݰ�����(����)�ṹ��
typedef struct PacketQueue {
	AVPacketList* first_pkt, * last_pkt;	//������β�ڵ�ָ��
	int nb_packets;							//���г���
	int size;								//����������ݵĻ��泤�ȣ�size=packet->size
	SDL_mutex* qlock;						//���л�������������������
	SDL_cond* qready;						//���о�����������
} PacketQueue;

static PacketQueue videoQueue;
static PacketQueue audioQueue;

//���г�ʼ������
inline void packet_queue_init(PacketQueue* q) {
	memset(q, 0, sizeof(PacketQueue));//ȫ���ʼ�����нṹ�����

	q->qlock = SDL_CreateMutex();//��������������
	q->qready = SDL_CreateCond();//����������������
}

//������л��棬�ͷŶ��������ж�̬������ڴ�
inline void packet_queue_flush(PacketQueue* q) {
	AVPacketList* pkt = nullptr;//���е�ǰ�ڵ�
	AVPacketList* pkttmp = nullptr;//��ʱ�ڵ�

	SDL_LockMutex(q->qlock);//����������

	for (pkt = q->first_pkt; pkt != NULL; pkt = pkttmp) {//�����������нڵ�
		pkttmp = pkt->next;//����ͷ�ڵ����
		av_packet_unref(&pkt->pkt);//��ǰ�ڵ����ü���-1
		av_freep(&pkt);//�ͷŵ�ǰ�ڵ㻺��
	}

	q->last_pkt = NULL;//����β�ڵ�ָ������
	q->first_pkt = NULL;//����ͷ�ڵ�ָ������
	q->nb_packets = 0;//���г�������
	q->size = 0;//���б������ݵĻ��泤������

	SDL_UnlockMutex(q->qlock);//����������
}

//������в������ݰ�
inline int packet_queue_put(PacketQueue* q, AVPacket* pkt) {
	/*-------׼������(����)�ڵ����------*/
	AVPacketList* pktlist;//��������ڵ����ָ��
	pktlist = (AVPacketList*)av_malloc(sizeof(AVPacketList));//�ڶ��ϴ�������ڵ����

	if (!pktlist) {//�������ڵ�����Ƿ񴴽��ɹ�
		return -1;
	}

	pktlist->pkt = *pkt;//���������ݰ���ֵ���½�����ڵ�����е����ݰ�����
	pktlist->next = NULL;//������ָ��Ϊ��
	//	if (av_packet_ref(pkt, pkt)<0) {//����pkt�������ݵ����ü���(��������е�pkt���½�����ڵ��е�pkt����ͬһ����ռ�)
	//		return -1;
	//	}
	/*---------���½��ڵ�������-------*/
	SDL_LockMutex(q->qlock);//���л�����������������������

	if (!q->last_pkt) {//������β�ڵ��Ƿ����(�������Ƿ�Ϊ��)
		q->first_pkt = pktlist;//��������(����β��)���򽫵�ǰ�ڵ�������Ϊ�׽ڵ�
	}
	else {
		q->last_pkt->next = pktlist;//���Ѵ���β�ڵ㣬�򽫵�ǰ�ڵ�ҵ�β�ڵ�ĺ��ָ���ϣ�����Ϊ�µ�β�ڵ�
	}
	q->last_pkt = pktlist;//����ǰ�ڵ���Ϊ�µ�β�ڵ�
	q->nb_packets++;//���г���+1
	q->size += pktlist->pkt.size;//���¶��б������ݵĻ��泤��

	SDL_CondSignal(q->qready);//���ȴ��̷߳�����Ϣ��֪ͨ�����Ѿ���
	SDL_UnlockMutex(q->qlock);//�ͷŻ�����

	return 0;
}

//�Ӷ�������ȡ���ݰ���������ȡ�����ݰ�������
inline int packet_queue_get(PacketQueue* q, AVPacket* pkt, int block) {
	AVPacketList* pktlist;//��ʱ����ڵ����ָ��
	int ret = 0;//�������

	SDL_LockMutex(q->qlock);//���л�����������������������

	for (;;) {
		if (quit) {
			ret = -1;

			break;
		}

		pktlist = q->first_pkt;//���ݽ������׸����ݰ�ָ��

		if (pktlist) {//������ݰ��Ƿ�Ϊ��(�����Ƿ�������)
			q->first_pkt = pktlist->next;//�����׽ڵ�ָ�����

			if (!q->first_pkt) {//����׽ڵ�ĺ�̽ڵ��Ƿ����
				q->last_pkt = NULL;//�������ڣ���β�ڵ�ָ���ÿ�
			}

			q->nb_packets--;//���г���-1
			q->size -= pktlist->pkt.size;//���¶��б������ݵĻ��泤��
			*pkt = pktlist->pkt;//�������׽ڵ����ݷ���

			av_free(pktlist);//�����ʱ�ڵ�����(����׽ڵ����ݣ��׽ڵ������)

			ret = 1;//�����ɹ�

			break;
		}
		else if (!block) {
			ret = 0;

			break;
		}
		else {
			//���д���δ����״̬����ʱͨ��SDL_CondWait�����ȴ�qready�����źţ�����ʱ�Ի���������
			/*---------------------
			 * �ȴ����о����ź�qready�����Ի�������ʱ����
			 * ��ʱ�̴߳�������״̬�������ڵȴ������������߳��б���
			 * ʹ�ø��߳�ֻ���ٽ�����Դ������ű����ѣ����������̱߳�Ƶ���л�
			 * �ú�������ʱ���������ٴα���ס����ִ�к�������
			 --------------------*/
			SDL_CondWait(q->qready, q->qlock);//��ʱ���������������Լ��������ȴ��ٽ�����Դ����(�ȴ�SDL_CondSignal�����ٽ�����Դ�������ź�)
		}
	}

	SDL_UnlockMutex(q->qlock);//�ͷŻ�����

	return ret;
}

/*------Audio Callback-------
 * ��Ƶ����ص�������sdlͨ���ûص�������������pcm����������������,
 * sdlͨ��һ�λ�׼��һ�黺��pcm���ݣ�ͨ���ûص���������������������Ƶpts���β���pcm����
 * �����뻺���pcm������ɲ��ź�������һ���µ�pcm��������(ÿ����Ƶ�������Ϊ��ʱ��sdl�͵��ô˺��������Ƶ������棬��������������)
 * When we begin playing audio, SDL will continually call this callback function
 * and ask it to fill the audio buffer with a certain number of bytes
 * The audio function callback takes the following parameters:
 * stream: A pointer to the audio buffer to be filled�������Ƶ���ݵ���������
 * len: The length (in bytes) of the audio buffer,���泤��wanted_spec.samples=SDL_AUDIO_BUFFER_SIZE(1024)
 --------------------------*/
inline void audio_callback(void* userdata, Uint8* stream, int len);

// Exceptions
constexpr auto FFMpegException_InitFailed = -1;

// pData, width, height
using rawDataCallBack = std::function<void(const unsigned char*, const int, const int)>;

using Uint8 = unsigned char;

class FFMpeg {
private:
	bool bLoop = true;

	AVFormatContext* pFormatContext = nullptr;

	const AVCodec* pVCodec = NULL;
	AVCodecParameters* pVCodecParameters = NULL;

	const AVCodec* pACodec = NULL;
	AVCodecParameters* pACodecParameters = NULL;

	int video_stream_index = -1;
	int audio_stream_index = -1;

	AVCodecContext* pVCodecContext = nullptr;
	AVCodecContext* pACodecContext = nullptr;

	AVFrame* pFrame = nullptr;

	AVPacket* pVPacket = nullptr;
	AVPacket* pAPacket = nullptr;
	AVPacket* pFlushPacket = nullptr;

	uint8_t* audio_buf = nullptr;
	//unsigned int audio_buf_size;
	//unsigned int audio_buf_index;

	//uint8_t* audio_pkt_data;
	//int audio_pkt_size;
	//int audio_hw_buf_size;

	SDL_AudioSpec spec;
	SDL_AudioSpec wanted_spec;

	SwsContext* swsContext = nullptr;

	AVStream* pViedoStream = nullptr;
	AVStream* pAudioStream = nullptr;

	AVDictionary* pVOptionsDict = nullptr;
	AVDictionary* pAOptionsDict = nullptr;

	AVRational rational = { 0 };
	double decimalRational = 0;

	int64_t totalFrame = 0;
	double timePerFrameInMs = 0;

	double totalTime = 0;
	double totalTimeInMs = 0;

	int64_t globalPts = 0;
	double videoClock = 0;

	inline void init_general() {
		if (avformat_find_stream_info(pFormatContext, NULL) < 0) {
			throw FFMpegException_InitFailed;
		}

		for (unsigned int i = 0; i < pFormatContext->nb_streams; i++) {
			AVCodecParameters* pLocalCodecParameters = NULL;
			pLocalCodecParameters = pFormatContext->streams [i]->codecpar;

			const AVCodec* pLocalCodec = avcodec_find_decoder(pLocalCodecParameters->codec_id);

			if (pLocalCodec == NULL) {
				continue;
			}

			if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_VIDEO) {
				if (video_stream_index == -1) {
					video_stream_index = i;
					pVCodec = pLocalCodec;
					pVCodecParameters = pLocalCodecParameters;
				}
			}

			if (pLocalCodecParameters->codec_type == AVMEDIA_TYPE_AUDIO) {
				if (audio_stream_index == -1) {
					audio_stream_index = i;
					pACodec = pLocalCodec;
					pACodecParameters = pLocalCodecParameters;
				}
			}
		}

		pVCodecContext = avcodec_alloc_context3(pVCodec);
		if (!pVCodecContext) {
			throw FFMpegException_InitFailed;
		}

		if (avcodec_parameters_to_context(pVCodecContext, pVCodecParameters) < 0) {
			throw FFMpegException_InitFailed;
		}

		if (avcodec_open2(pVCodecContext, pVCodec, NULL) < 0) {
			throw FFMpegException_InitFailed;
		}

		packet_queue_init(&videoQueue);

		swsContext = sws_getContext(pVCodecContext->width, pVCodecContext->height, pVCodecContext->pix_fmt,
			pVCodecContext->width, pVCodecContext->height, AV_PIX_FMT_BGR24
			, NULL, NULL, NULL, NULL);

		pACodecContext = avcodec_alloc_context3(pACodec);
		if (!pACodecContext) {
			throw FFMpegException_InitFailed;
		}

		if (avcodec_parameters_to_context(pACodecContext, pACodecParameters) < 0) {
			throw FFMpegException_InitFailed;
		}

		if (avcodec_open2(pACodecContext, pACodec, NULL) < 0) {
			throw FFMpegException_InitFailed;
		}

		packet_queue_init(&audioQueue);

		pFrame = av_frame_alloc();
		if (!pFrame) {
			throw FFMpegException_InitFailed;
		}

		pVPacket = av_packet_alloc();
		if (!pVPacket) {
			throw FFMpegException_InitFailed;
		}

		pAPacket = av_packet_alloc();
		if (!pAPacket) {
			throw FFMpegException_InitFailed;
		}

		pFlushPacket = av_packet_alloc();
		if (!pFlushPacket) {
			throw FFMpegException_InitFailed;
		}

		pFlushPacket->data = (unsigned char*)"FLUSH";

		pViedoStream = pFormatContext->streams [video_stream_index];
		pAudioStream = pFormatContext->streams [audio_stream_index];

		rational = pViedoStream->time_base;
		decimalRational = (double)rational.num / rational.den;

		totalFrame = pViedoStream->duration;
		timePerFrameInMs = decimalRational * 1000;

		totalTime = totalFrame * decimalRational;
		totalTimeInMs = totalTime * 1000;

		audio_buf = new uint8_t [(MAX_AUDIO_FRAME_SIZE * 3) / 2];

		wanted_spec.freq = pACodecContext->sample_rate;//����Ƶ�� DSP frequency -- samples per second
		wanted_spec.format = AUDIO_S16SYS;//������ʽ Audio data format
		wanted_spec.channels = pACodecContext->channels;//������ Number of channels: 1 mono, 2 stereo
		wanted_spec.silence = 0;//�����ʱ�Ƿ���
		wanted_spec.samples = SDL_AUDIO_BUFFER_SIZE;//Ĭ��ÿ�ζ���Ƶ����Ĵ�С���Ƽ�ֵΪ 512~8192��ffplayʹ�õ���1024 specifies a unit of audio data refers to the size of the audio buffer in sample frames

		wanted_spec.callback = audio_callback;//����ȡ��Ƶ���ݵĻص��ӿں��� the function to call when the audio device needs more data
		//wanted_spec.userdata = (void*)pACodecContext;//�����û�����
		wanted_spec.userdata = (void*)this;//�����û�����

		if (SDL_OpenAudio(&wanted_spec, &spec) < 0) {
			auto error = SDL_GetError();

			throw SDL_EXCEPTION_AUDIO;
		}

		SDL_PauseAudio(0);
	}

	inline int read_frame() {
		int response = 0;

		while (true) {
			if (audioQueue.size > MAX_AUDIOQ_SIZE
				|| videoQueue.size > MAX_VIDEOQ_SIZE) {
				SDL_Delay(10);

				return 0;
			}

			auto readResult = av_read_frame(pFormatContext, pVPacket);

			if (readResult < 0) {
				SDL_Delay(100);

				return 0;
			}

			bool bValid = false;

			// if it's the video stream
			if (pVPacket->stream_index == video_stream_index) {
				packet_queue_put(&videoQueue, pVPacket);
				bValid = true;
			}

			//if it's the audio stream
			if (pVPacket->stream_index == audio_stream_index) {
				packet_queue_put(&audioQueue, pVPacket);
				bValid = true;
			}

			if (!bValid) {
				av_packet_unref(pVPacket);
			}
		}

		return 0;
	}

	int decode_vpacket(AVPacket* pVPacket, AVCodecContext* pVCodecContext, AVFrame* pFrame, rawDataCallBack callBack) {
		// Supply raw packet data as input to a decoder
		// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga58bc4bf1e0ac59e27362597e467efff3
		int response = avcodec_send_packet(pVCodecContext, pVPacket);

		if (response < 0) {
			return response;
		}


		while (response >= 0) {
			// Return decoded output data (into a frame) from a decoder
			// https://ffmpeg.org/doxygen/trunk/group__lavc__decoding.html#ga11e6542c4e66d3028668788a1a74217c
			response = avcodec_receive_frame(pVCodecContext, pFrame);

			if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
				break;
			}
			else if (response < 0) {
				av_frame_unref(pFrame);

				return response;
			}

			if (response >= 0) {
				// Check if the frame is a planar YUV 4:2:0, 12bpp
				// That is the format of the provided .mp4 file
				// https://zhuanlan.zhihu.com/p/53305541
				if (pFrame->format != AV_PIX_FMT_BGR24) {
					int linesize [8] = { pFrame->linesize [0] * 3 };
					int num_bytes = av_image_get_buffer_size(AV_PIX_FMT_BGR24, pFrame->width, pFrame->height, 1);

					//auto p_global_bgr_buffer = (uint8_t*)malloc(num_bytes * sizeof(uint8_t));
					auto p_global_bgr_buffer = new uint8_t [num_bytes];
					uint8_t* bgr_buffer [8] = { p_global_bgr_buffer };

					sws_scale(swsContext, pFrame->data, pFrame->linesize, 0, pFrame->height, bgr_buffer, linesize);

					//bgr_buffer[0] is the BGR raw data
					callBack(bgr_buffer [0], pFrame->width, pFrame->height);

					delete[] p_global_bgr_buffer;
				}
				else {
					// call callback
					callBack(pFrame->data [0], pFrame->width, pFrame->height);
				}

				av_frame_unref(pFrame);
			}
		}

		return 0;
	}

	//https://github.com/brookicv/FFMPEG-study/blob/master/FFmpeg-playAudio.cpp
	int decode_apacket() {
		AVFrame* pFrame = av_frame_alloc();
		int data_size = 0;
		AVPacket pkt = { 0 };

		SwrContext* swr_ctx = nullptr;

		if (!packet_queue_get(&audioQueue, &pkt, true)) {
			return -1;
		}

		if (quit) {
			return -1;
		}

		int response = avcodec_send_packet(pACodecContext, &pkt);
		if (response < 0 && response != AVERROR(EAGAIN) && response != AVERROR_EOF) {
			return -1;
		}

		response = avcodec_receive_frame(pACodecContext, pFrame);
		if (response < 0 && response != AVERROR_EOF) {
			//av_frame_unref(pFrame);

			return -1;
		}

		// ����ͨ������channel_layout
		if (pFrame->channels > 0 && pFrame->channel_layout == 0) {
			pFrame->channel_layout = av_get_default_channel_layout(pFrame->channels);
		}
		else if (pFrame->channels == 0 && pFrame->channel_layout > 0) {
			pFrame->channels = av_get_channel_layout_nb_channels(pFrame->channel_layout);
		}

		AVSampleFormat dst_format = AV_SAMPLE_FMT_S16;//av_get_packed_sample_fmt((AVSampleFormat)frame->format);
		Uint64 dst_layout = av_get_default_channel_layout(pFrame->channels);

		// ����ת������
		swr_ctx = swr_alloc_set_opts(nullptr, dst_layout, dst_format, pFrame->sample_rate,
			pFrame->channel_layout, (AVSampleFormat)pFrame->format, pFrame->sample_rate, 0, nullptr);

		if (!swr_ctx || swr_init(swr_ctx) < 0) {
			return -1;
		}

		// ����ת�����sample���� a * b / c
		int dst_nb_samples = (int)av_rescale_rnd(swr_get_delay(swr_ctx, pFrame->sample_rate) + pFrame->nb_samples, pFrame->sample_rate, pFrame->sample_rate, AVRounding(1));
		// ת��������ֵΪת�����sample����
		int nb = swr_convert(swr_ctx, &audio_buf, dst_nb_samples, (const uint8_t**)pFrame->data, pFrame->nb_samples);
		data_size = pFrame->channels * nb * av_get_bytes_per_sample(dst_format);

		av_frame_free(&pFrame);
		swr_free(&swr_ctx);

		return data_size;
	}

	/*---------------------------
	 * �����ڲ���Ƶ���ż�ʱ��(��¼��Ƶ�Ѿ���ʱ��(video_clock)��
	 * @is��ȫ��״̬������
	 * @src_frame����ǰ(�����)(�����µ�)ͼ��֡����
	 * @pts����ǰͼ��֡����ʾʱ���
	 ---------------------------*/
	inline double synchronize_video(AVFrame* src_frame, double pts) {
		/*----------�����ʾʱ���----------*/
		if (pts != 0) {//�����ʾʱ����Ƿ���Ч
			// If we have pts, set video clock to it.
			videoClock = pts;//����ʾʱ��������Ѳ���ʱ��
		}
		else {//����ȡ������ʾʱ���
			// If we aren't given a pts, set it to the clock.
			pts = videoClock;//���Ѳ���ʱ�������ʾʱ���
		}
		/*--------������Ƶ�Ѿ���ʱ��--------*/
			// Update the video clock������֡Ҫ�ظ���ʾ(ȡ����repeat_pict)����ȫ����Ƶ����ʱ��video_clockӦ�����ظ���ʾ������*֡��
		double frame_delay = av_q2d(pVCodecContext->time_base);//��֡��ʾ�꽫Ҫ���ѵ�ʱ��
		// If we are repeating a frame, adjust clock accordingly,�������ظ�֡�������������ŵ�ǰ����֡ͼ��䰲����Ⱦ�ظ�֡
		frame_delay += src_frame->repeat_pict * (frame_delay * 0.5);//������Ⱦ�ظ�֡��ʱֵ(����������ʱֵ)
		videoClock += frame_delay;//������Ƶ����ʱ��

		return pts;//��ʱ���ص�ֵ��Ϊ��һ֡��Ҫ��ʼ��ʾ��ʱ���
	}

public:
	//Load from file
	FFMpeg(const std::wstring& filePath) {
		pFormatContext = avformat_alloc_context();
		if (!pFormatContext) {
			throw FFMpegException_InitFailed;
		}

		if (avformat_open_input(&pFormatContext, ConvertWStrToStr(filePath).c_str(), NULL, NULL) != 0) {
			throw FFMpegException_InitFailed;
		}

		init_general();
	}

	//Load from memory
	//https://blog.csdn.net/leixiaohua1020/article/details/12980423
	FFMpeg(unsigned char* pBuffer, size_t bfSz) {
		pFormatContext = avformat_alloc_context();
		if (!pFormatContext) {
			throw FFMpegException_InitFailed;
		}

		auto pAvio = avio_alloc_context(pBuffer, bfSz, 0, NULL
			, [] (void* opaque, uint8_t* buf, int bufsize)->int {
				return bufsize;
			}
		, NULL, NULL);

		if (!pAvio) {
			throw FFMpegException_InitFailed;
		}

		pFormatContext->pb = pAvio;

		if (avformat_open_input(&pFormatContext, NULL, NULL, NULL) != 0) {
			throw FFMpegException_InitFailed;
		}

		init_general();
	}

	~FFMpeg() {
		avformat_close_input(&pFormatContext);
		av_packet_free(&pVPacket);
		av_packet_free(&pAPacket);
		av_packet_free(&pFlushPacket);
		av_frame_free(&pFrame);
		avcodec_close(pVCodecContext);
		avcodec_free_context(&pVCodecContext);
		avcodec_close(pACodecContext);
		avcodec_free_context(&pACodecContext);

		sws_freeContext(swsContext);

		delete[] audio_buf;

		quit = 1;

		SDL_CondSignal(audioQueue.qready);
		SDL_CondSignal(videoQueue.qready);
	}

	inline int decode_videoFrame(rawDataCallBack callBack) {
		int response = 0;
		double pts = 0;

		if (packet_queue_get(&videoQueue, pVPacket, 1) < 0) {
			return -1;
		}

		if (pVPacket->data == pFlushPacket->data) {
			avcodec_flush_buffers(pVCodecContext);
			return 0;
		}

		globalPts = pVPacket->pts;

		response = decode_vpacket(pVPacket, pVCodecContext, pFrame, callBack);

		av_packet_unref(pVPacket);

		return response;
	}

	inline int decode_audioFrame(Uint8* stream, int len) {
		//ÿ��д��stream�����ݳ���
		int wt_stream_len = 0;
		//ÿ���������ݳ���
		int audio_size = 0;

		//static uint8_t audio_buf[(MAX_AUDIO_FRAME_SIZE * 3) / 2];//�������һ��packet��Ķ�֡ԭʼ��Ƶ����
		static unsigned int audio_buf_size = 0;//�����Ķ�֡��Ƶ���ݳ���
		static unsigned int audio_buf_index = 0;//�ۼ�д��stream�ĳ���

		SDL_memset(stream, 0, len);

		//�����Ƶ�����ʣ�೤��
		while (len > 0) {
			//����Ƿ���Ҫִ�н������
			if (audio_buf_index >= audio_buf_size) {
				// We have already sent all our data; get more
				// �ӻ����������ȡ���ݰ������룬�����ؽ��������ݳ��ȣ�audio_buf�����п��ܰ�����֡��������Ƶ����
				audio_size = decode_apacket();

				if (quit) {
					return -1;
				}

				if (audio_size < 0) {//����������Ƿ�ɹ�
					// If error, output silence.
					audio_buf_size = 1024; // arbitrary?
					memset(audio_buf, 0, audio_buf_size);//ȫ�����û�����
				}
				else {
					//TODO
					audio_buf_size = audio_size;//����packet�а�����ԭʼ��Ƶ���ݳ���(��֡)
				}
				audio_buf_index = 0;//��ʼ���ۼ�д�뻺�泤��
			}

			wt_stream_len = audio_buf_size - audio_buf_index;//������뻺��ʣ�೤��

			if (wt_stream_len > len) {//���ÿ��д�뻺������ݳ����Ƿ񳬹�ָ������(1024)
				wt_stream_len = len;//ָ�����ȴӽ���Ļ�����ȡ����
			}
			//ÿ�δӽ���Ļ�����������ָ�����ȳ�ȡ���ݲ�д��stream���ݸ�����
			memcpy(stream, (uint8_t*)audio_buf + audio_buf_index, wt_stream_len);
			//SDL_MixAudio(stream, audio_buf + audio_buf_index, len, SDL_MIX_MAXVOLUME);

			len -= wt_stream_len;//���½�����Ƶ�����ʣ�೤��
			stream += wt_stream_len;//���»���д��λ��
			audio_buf_index += wt_stream_len;//�����ۼ�д�뻺�����ݳ���
		}

		return 0;
	}

	inline int64_t get_timePerFrame() {
		return (int64_t)(timePerFrameInMs);
	}

	//inline int set_videoPosition(size_t ms = 0, bool bInit = false) {
	inline int set_videoPosition(size_t ms = 0, bool bInit = true) {
		auto protectedTimeInMs = min(totalTimeInMs, max(0, ms));
		auto protectedTime = protectedTimeInMs / 1000;
		auto protectedFrame = protectedTime / decimalRational;

		//auto flags = AVSEEK_FLAG_BACKWARD | AVSEEK_FLAG_FRAME;
		auto flags = AVSEEK_FLAG_FRAME;

		if (!(av_seek_frame(pFormatContext, video_stream_index
			, (int64_t)(protectedFrame)
			, flags) >= 0)) {
			return -1;
		}
		else {
			if (video_stream_index != -1) {
				if (!bInit) {
					packet_queue_flush(&videoQueue);
					packet_queue_put(&videoQueue, pFlushPacket);
				}
			}
			if (audio_stream_index != -1) {
				if (!bInit) {
					packet_queue_flush(&audioQueue);
					packet_queue_put(&audioQueue, pFlushPacket);
				}
			}
		}

		return 0;
	}

	inline int get_videoFrame(size_t ms, rawDataCallBack callBack) {
		int response = 0;
		int how_many_packets_to_process = 0;

		response = set_videoPosition(ms);

		response = read_frame();
		response = decode_videoFrame(callBack);

		return response;
	}

	inline int get_nextFrame(rawDataCallBack callBack) {
		int response = 0;
		int how_many_packets_to_process = 0;

		response = read_frame();
		response = decode_videoFrame(callBack);
		//response = decode_audioFrame();

		globalPts;

		if (bLoop && (int64_t)(pVCodecContext->frame_number + 1) == totalFrame) {
			response = set_videoPosition();

			if (response < 0) { return response; }

			pVCodecContext->frame_number = 0;
			pACodecContext->frame_number = 0;
		}

		return response;
	}
};

inline void audio_callback(void* userdata, Uint8* stream, int len) {
	FFMpeg* pFFMpeg = (FFMpeg*)userdata;
	pFFMpeg->decode_audioFrame(stream, len);
}