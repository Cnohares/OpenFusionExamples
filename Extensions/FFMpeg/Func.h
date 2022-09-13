#pragma once

#include <functional>

inline void UpdateScale(LPRDATA rdPtr, int width, int height) {
	rdPtr->rc.rcScaleX = ((float)rdPtr->swidth) / width;
	rdPtr->rc.rcScaleY = ((float)rdPtr->sheight) / height;
}

inline void ReDisplay(LPRDATA rdPtr) {
	if (rdPtr->pMemSf != nullptr && rdPtr->pMemSf->IsValid()) {
		//callRunTimeFunction(rdPtr, RFUNCTION_REDRAW, 0, 0);
		rdPtr->rc.rcChanged = true;

		//rdPtr->rHo.hoImgXSpot = rdPtr->hotSpot.x;
		//rdPtr->rHo.hoImgYSpot = rdPtr->hotSpot.y;

		rdPtr->rHo.hoImgXSpot = 0;
		rdPtr->rHo.hoImgYSpot = 0;

		rdPtr->rHo.hoImgWidth = rdPtr->pMemSf->GetWidth();
		rdPtr->rHo.hoImgHeight = rdPtr->pMemSf->GetHeight();

		rdPtr->bChanged = true;
	}
}

inline void InitSurface(LPRDATA rdPtr,const int width, const int height) {
	if (rdPtr->pMemSf == nullptr) {
		rdPtr->pMemSf = CreateSurface(24, width, height);

		auto alphaSz = width * height;

		auto pAlpha = new BYTE[alphaSz];
		memset(pAlpha, 255, alphaSz);

		rdPtr->pMemSf->SetAlpha(pAlpha, rdPtr->pMemSf->GetWidth());

		delete[] pAlpha;
		pAlpha = nullptr;
	}
}

inline void CopyData(const unsigned char* pData, LPSURFACE pMemSf, bool bPm) {
	auto sfCoef = GetSfCoef(pMemSf);

	auto lineSz = sfCoef.pitch;
	auto alphaSz = sfCoef.sz / sfCoef.byte;

	for (int y = 0; y < pMemSf->GetHeight(); y++) {
		auto pMemData = sfCoef.pData + y * sfCoef.pitch;
		auto pVideo = pData + (pMemSf->GetHeight() - 1 - y) * sfCoef.pitch;

		memcpy(pMemData, pVideo, lineSz);
	}

//#ifdef _DEBUG
//	_SavetoClipBoard(pMemSf, false);
//#endif // _DEBUG

	ReleaseSfCoef(pMemSf, sfCoef);

	if (bPm) {
		pMemSf->PremultiplyAlpha();		// only needed in DX11 premultiplied mode
	}
}

// pMemSf
using blitCallBack = std::function<void(LPSURFACE&)>;

// allocate pMemSf
inline void BlitVideoFrame(LPRDATA rdPtr, size_t ms, blitCallBack callBack) {
	if (rdPtr->pFFMpeg == nullptr) {
		return;
	}

	rdPtr->pFFMpeg->get_videoFrame(ms, [&] (const unsigned char* pData, const int width, const int height) {
		CopyData(pData, rdPtr->pMemSf, rdPtr->bPm);
		ReDisplay(rdPtr);

		callBack(rdPtr->pMemSf);
		});
}

inline void NextVideoFrame(LPRDATA rdPtr) {
	if (rdPtr->pFFMpeg == nullptr) {
		return;
	}

	rdPtr->pFFMpeg->get_nextFrame([&](const unsigned char* pData, const int width, const int height) {
		CopyData(pData, rdPtr->pMemSf, rdPtr->bPm);
		ReDisplay(rdPtr);
		});
}

//inline void update_audiosettings(FFMpeg* pFFMpeg, SDL_AudioSpec* pData) {
//	if (pFFMpeg == nullptr || pData == nullptr) {
//		return;
//	}
//
//	pFFMpeg->update_audiosettings([&](const AVCodecContext* pAudiopCodecContext) {
//		pData->freq = pAudiopCodecContext->sample_rate;//����Ƶ�� DSP frequency -- samples per second
//		pData->format = AUDIO_S16SYS;//������ʽ Audio data format
//		pData->channels = pAudiopCodecContext->channels;//������ Number of channels: 1 mono, 2 stereo
//		pData->silence = 0;//�����ʱ�Ƿ���
//		pData->samples = SDL_AUDIO_BUFFER_SIZE;//Ĭ��ÿ�ζ���Ƶ����Ĵ�С���Ƽ�ֵΪ 512~8192��ffplayʹ�õ���1024 specifies a unit of audio data refers to the size of the audio buffer in sample frames
//
//		pData->callback = audio_callback;//����ȡ��Ƶ���ݵĻص��ӿں��� the function to call when the audio device needs more data
//		pData->userdata = (void*)pAudiopCodecContext;//�����û�����
//		});
//}

inline void CloseGeneral(LPRDATA rdPtr) {
	delete rdPtr->pEncrytpt;
	rdPtr->pEncrytpt = nullptr;

	delete rdPtr->pFFMpeg;
	rdPtr->pFFMpeg = nullptr;

	rdPtr->bOpen = false;
	rdPtr->bPlay = false;
}