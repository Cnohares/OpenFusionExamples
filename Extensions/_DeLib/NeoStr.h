﻿#pragma once

//#define _FONTEMBEDDEBUG
//#define _CONSOLE

#include <map>
#include <vector>
#include <functional>
#include <string_view>

//#include <FusionUtilities.h>

#ifdef _DEBUG		
#include <assert.h>
#endif

// compatible with MMF
#undef Font
#undef fpFont

#pragma region _GDIPLUS_INIT


//#define _BLUR

#ifdef _BLUR
#define GDIPVER 0x0110
#endif

#include <gdiplus.h>
#pragma comment(lib,"Gdiplus")

#include <gdiplusheaders.h>

#ifdef _BLUR
#include <gdipluseffects.h>
#endif

using Gdiplus::GdiplusStartupInput;
using Gdiplus::Graphics;
using Gdiplus::GraphicsPath;
using Gdiplus::StringFormat;
using Gdiplus::StringAlignment;
using Gdiplus::FillMode;
using Gdiplus::Color;
using Gdiplus::Font;
using Gdiplus::Pen;
using Gdiplus::LineJoin;
using Gdiplus::SolidBrush;
using Gdiplus::Rect;
using Gdiplus::RectF;
using Gdiplus::PointF;
using Gdiplus::Region;
using Gdiplus::Bitmap;
using Gdiplus::BitmapData;
using Gdiplus::ImageLockMode;
using Gdiplus::ImageCodecInfo;
using Gdiplus::PrivateFontCollection;
using Gdiplus::InstalledFontCollection;
using Gdiplus::FontFamily;
using Gdiplus::GetImageEncodersSize;

#ifdef _BLUR
using Gdiplus::Blur;
using Gdiplus::BlurParams;
#endif

#pragma endregion

inline RECT operator+(RECT rA, RECT rB) {
	return RECT { rA.left + rB.left
				,rA.top + rB.top
				,rA.right + rB.right
				,rA.bottom + rB.bottom };
}

constexpr auto DEFAULEBORDEROFFSET = 20;

class NeoStr {
private:
	HDC hdc;
	COLORREF dwTextColor;

	HFONT hFont;
	LOGFONT logFont;

	TEXTMETRIC tm;
	int nRowSpace = 0;
	int nColSpace = 0;

	DWORD dwDTFlags;

	bool bClip = true;

	unsigned short borderOffsetX = 0;
	unsigned short borderOffsetY = 0;

	bool bVerticalAlignOffset;

	int renderWidth = 0;
	int renderHeight = 0;

	Gdiplus::TextRenderingHint textRenderingHint = Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias;
	Gdiplus::SmoothingMode smoothingMode = Gdiplus::SmoothingMode::SmoothingModeHighQuality;
	Gdiplus::PixelOffsetMode pixelOffsetMode = Gdiplus::PixelOffsetMode::PixelOffsetModeHalf;

	bool bOutLine = false;
	BYTE nOutLinePixel = 1;
	COLORREF dwOutLineColor = RGB(255, 255, 0);

	bool bShadow = false;
	BYTE nShadowOffsetX = 0;
	BYTE nShadowOffsetY = 0;

	// Set to false if app have a shared GDI plus environment
	bool needGDIPStartUp = true;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;

	Bitmap* pBitmap = nullptr;
	PrivateFontCollection* pFontCollection = nullptr;

	struct StrPos {
		size_t start;
		size_t end;
		size_t length;
		long width;
		long height;
		long x;
		long y;
	};

	std::vector<StrPos> strPos;

	struct StrSize {
		long width;
		long height;
	};

	StrSize* pStrSizeArr = nullptr;

	struct CharPos {
		long x;
		long y;
		long maxWidth;
		long totalHeight;
	};

	CharPos* pCharPosArr = nullptr;

	long rcWidth = 0;
	long rcHeight = 0;

	long totalWidth = 0;
	long totalHeight = 0;

	long startY = 0;

	LPSURFACE pMemSf = nullptr;

	int hotSpotX = 0;
	int hotSpotY = 0;

	float xScale = 1.0;
	float yScale = 1.0;

	int angle = 0;

	int hwaType = 0;
	int hwaDriver = 0;
	bool preMulAlpha = false;

	LPSURFACE pHwaSf = nullptr;

	std::wstring notAtStart = L"!%),.:;>?]}¢¨°·ˇˉ―‖’”…‰′″›℃∶、。〃〉》」』】〕〗〞︶︺︾﹀﹄﹚﹜﹞！＂％＇），．：；？］｀｜｝～￠";
	std::wstring notAtEnd = L"$([{£¥·‘“〈《「『【〔〖〝﹙﹛﹝＄（．［｛￡￥";

	std::map<wchar_t, StrSize> charSzCache;

	inline bool CheckMatch(wchar_t wChar, std::wstring& data) {
		for (auto& wChartoCheck : data) {
			if (wChartoCheck == wChar) {
				return true;
			}
		}

		return false;
	}

	inline bool NotAtStart(wchar_t wChar) {
		return CheckMatch(wChar, notAtStart);
	}

	inline bool NotAtEnd(wchar_t wChar) {
		return CheckMatch(wChar, notAtEnd);
	}

	inline int GetStartPosX(long totalWidth, long rcWidth) const {
		//DT_LEFT | DT_CENTER | DT_RIGHT
		//if (this->dwDTFlags & DT_LEFT) {
		//	return 0;
		//}
		if (this->dwDTFlags & DT_CENTER) {
			return ((rcWidth - totalWidth) >> 1);
		}
		if (this->dwDTFlags & DT_RIGHT) {
			return rcWidth - totalWidth;
		}

		return 0;
	}

	inline int GetStartPosY(long totalHeight, long rcHeight) const {
		//	DT_TOP | DT_VCENTER | DT_BOTTOM
		//if (this->dwDTFlags & DT_TOP) {
		//	//return 0;
		//	return this->tm.tmInternalLeading;
		//}
		if (this->dwDTFlags & DT_VCENTER) {
			//return ((rcHeight - totalHeight) >> 1);

			//remove offset to make exactly at the center
			//https://docs.microsoft.com/en-us/windows/win32/gdi/string-widths-and-heights

			auto verticalAlignOffset = bVerticalAlignOffset
				? this->tm.tmInternalLeading + this->tm.tmExternalLeading
				: 0;

			//return ((rcHeight - (totalHeight - (this->tm.tmInternalLeading + this->tm.tmExternalLeading))) >> 1);
			return ((rcHeight - (totalHeight - verticalAlignOffset)) >> 1);
		}
		if (this->dwDTFlags & DT_BOTTOM) {
			return rcHeight - totalHeight;
		}

		return 0;
	}

	int GetEncoderClsid(const WCHAR* format, CLSID* pClsid) {
		UINT  num = 0;          // number of image encoders
		UINT  size = 0;         // size of the image encoder array in bytes
		ImageCodecInfo* pImageCodecInfo = NULL;

		GetImageEncodersSize(&num, &size);

		if (size == 0) {
			return -1;  // Failure
		}

		pImageCodecInfo = (ImageCodecInfo*)(malloc(size));

		if (pImageCodecInfo == NULL) {
			return -1;  // Failure
		}

		GetImageEncoders(num, size, pImageCodecInfo);

		for (UINT j = 0; j < num; ++j) {
			if (wcscmp(pImageCodecInfo [j].MimeType, format) == 0) {
				*pClsid = pImageCodecInfo [j].Clsid;
				free(pImageCodecInfo);

				return j;  // Success
			}
		}

		free(pImageCodecInfo);

		return -1;  // Failure
	}

	inline size_t LogFontHasher(LOGFONT logFont) {
		constexpr auto HASHER_MAGICNUMBER = 0x9e3779b9;
		constexpr auto HASHER_MOVE = [](size_t seed) { return HASHER_MAGICNUMBER + (seed << 6) + (seed >> 2); };

		size_t seed = 65535;

		seed ^= logFont.lfHeight + HASHER_MOVE(seed);
		seed ^= logFont.lfWidth + HASHER_MOVE(seed);
		seed ^= logFont.lfEscapement + HASHER_MOVE(seed);
		seed ^= logFont.lfOrientation + HASHER_MOVE(seed);
		seed ^= logFont.lfWeight + HASHER_MOVE(seed);
		seed ^= logFont.lfItalic + HASHER_MOVE(seed);
		seed ^= logFont.lfUnderline + HASHER_MOVE(seed);
		seed ^= logFont.lfStrikeOut + HASHER_MOVE(seed);
		seed ^= logFont.lfCharSet + HASHER_MOVE(seed);
		seed ^= logFont.lfOutPrecision + HASHER_MOVE(seed);
		seed ^= logFont.lfClipPrecision + HASHER_MOVE(seed);
		seed ^= logFont.lfQuality + HASHER_MOVE(seed);
		seed ^= logFont.lfPitchAndFamily + HASHER_MOVE(seed);

		for (auto i = 0; i < LF_FACESIZE; i++) {
			seed ^= logFont.lfFaceName[i] + HASHER_MOVE(seed);
		}

		return seed;
	}

public:
	NeoStr(DWORD dwAlignFlags, COLORREF color
		, HFONT hFont
		, bool needGDIPStartUp = true
		, PrivateFontCollection* pFontCollection = nullptr) {
		this->hdc = GetDC(NULL);
		//this->hdc = GetDC(rdPtr->rHo.hoAdRunHeader->rhHEditWin);

		SelectObject(this->hdc, hFont);

		this->SetColor(color);

		this->hFont = hFont;

		GetObject(this->hFont, sizeof(LOGFONT), &this->logFont);
		GetTextMetrics(hdc, &this->tm);

		this->dwDTFlags = dwAlignFlags | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL;

		this->strPos.reserve(20);

		this->pFontCollection = pFontCollection;

		this->needGDIPStartUp = needGDIPStartUp;

		if (this->needGDIPStartUp) {
			Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		}
		
		// add a default char to return default value when input text is empty
		this->GetCharSizeWithCache(L'露');
	}

	~NeoStr() {
		ReleaseDC(NULL, this->hdc);

		delete[] this->pStrSizeArr;
		this->pStrSizeArr = nullptr;

		delete[] this->pCharPosArr;
		this->pCharPosArr = nullptr;

		delete this->pMemSf;
		this->pMemSf = nullptr;

		delete this->pBitmap;
		this->pBitmap = nullptr;

		delete this->pHwaSf;
		this->pHwaSf = nullptr;

		if (this->needGDIPStartUp) {
			Gdiplus::GdiplusShutdown(gdiplusToken);
		}
	}

	//https://stackoverflow.com/questions/42595856/fonts-added-with-addfontresourceex-are-not-working-in-gdi
	//https://docs.microsoft.com/zh-cn/windows/win32/api/gdiplusheaders/nf-gdiplusheaders-privatefontcollection-addfontfile
	//https://www.codeproject.com/Articles/42041/How-to-Use-a-Font-Without-Installing-it
	//https://blog.csdn.net/analogous_love/article/details/45845971
	inline void EmbedFont(LPCWSTR pFontFile) {
		this->pFontCollection->AddFontFile(pFontFile);
		auto count = this->pFontCollection->GetFamilyCount();

		return;
	}

	inline static void EmbedFont(LPCWSTR pFontFile, PrivateFontCollection& fontCollection) {
		fontCollection.AddFontFile(pFontFile);

		return;
	}

	inline static bool FontCollectionHasFont(LPWSTR pFaceName
		, Gdiplus::FontCollection* pFontCollection) {
		if (pFontCollection == nullptr) {
			return false;
		}

		int n = pFontCollection->GetFamilyCount();

		if (n == 0) {
			return false;
		}

		FontFamily* ffs = new FontFamily [n];

		int found;
		pFontCollection->GetFamilies(n, ffs, &found);

		if (found == 0) {
			return false;
		}

		wchar_t name [LF_FACESIZE] { 0 };

		LANGID language = MAKELANGID(LANG_CHINESE, SUBLANG_CHINESE_SIMPLIFIED);

		bool has = false;
		bool hasSuffixRegular = false;
		//bool hasSuffixNormal = false;

		std::wstring withRegular = (std::wstring)pFaceName + (std::wstring)L" Regular";
		//std::wstring withNormal = (std::wstring)pFaceName + (std::wstring)L" Normal";

		for (int i = 0; i < n; i++) {
			has = false;
			hasSuffixRegular = false;
			//hasSuffixNormal = false;

			auto hasName = [&] (LANGID language = (LANGID)0U) {
				memset(name, 0, LF_FACESIZE * sizeof(wchar_t));
				ffs [i].GetFamilyName(name, language);

				has |= (_wcsicmp(name, pFaceName) == 0);

				if (has) {
					return;
				}

				hasSuffixRegular |= (_wcsicmp(name, withRegular.c_str()) == 0);

				if (hasSuffixRegular) {
					return;
				}

				//hasSuffixNormal |= (_wcsicmp(name, withNormal.c_str()) == 0);
			};

			hasName();
			hasName(language);

			if (has || hasSuffixRegular/* || hasSuffixNormal*/) {
				if (hasSuffixRegular) {
					wcscpy_s(pFaceName, LF_FACESIZE, withRegular.c_str());
				}
				//if (hasSuffixNormal) {
				//	wcscpy_s(pFaceName, LF_FACESIZE, withNormal.c_str());
				//}

				break;
			}
		}

		delete[] ffs;

		return has || hasSuffixRegular/* || hasSuffixNormal*/;
	}

	inline static int GetFontStyle(LOGFONT& logFont) {
		int fontStyle = Gdiplus::FontStyleRegular;

		if (logFont.lfWeight >= 600) {
			fontStyle = fontStyle | Gdiplus::FontStyleBold;
		}
		if (logFont.lfItalic) {
			fontStyle = fontStyle | Gdiplus::FontStyleRegular;
		}
		if (logFont.lfUnderline) {
			fontStyle = fontStyle | Gdiplus::FontStyleUnderline;
		}
		if (logFont.lfStrikeOut) {
			fontStyle = fontStyle | Gdiplus::FontStyleStrikeout;
		}

		return fontStyle;
	}

	inline Font GetFont(LOGFONT logFont) {
		//auto bTest = StrIEqu(this->logFont.lfFaceName, L"思源黑体 CN");
		auto bFound = FontCollectionHasFont(logFont.lfFaceName, this->pFontCollection);

#ifdef _FONTEMBEDDEBUG
		if (!bFound) {
			MSGBOX((std::wstring)this->logFont.lfFaceName + (std::wstring)L" Not Found");
		}
#endif // _FONTEMBEDDEBUG

		return Font(logFont.lfFaceName
			, (float)abs(logFont.lfHeight)
			, GetFontStyle(logFont)
			, Gdiplus::UnitWorld
			, bFound ? this->pFontCollection
			: nullptr);
	}

	inline void SetHWA(int type, int driver, bool preMulAlpha) {
		this->hwaType = type;
		this->hwaDriver = driver;
		this->preMulAlpha = preMulAlpha;
	}

	inline void SetColor(DWORD color) {
		this->dwTextColor = color;
	}

	inline void SetBorderOffset(unsigned short borderOffsetX, unsigned short borderOffsetY) {
		this->borderOffsetX = borderOffsetX;
		this->borderOffsetY = borderOffsetX;
	}

	// set row/col space
	inline void SetSpace(int nRowSpace = 0, int nColSpace = 0) {
		this->nRowSpace = nRowSpace /*+ this->tm.tmInternalLeading + this->tm.tmExternalLeading*/;
		this->nColSpace = nColSpace;
	}

	inline void SetSmooth(
		Gdiplus::TextRenderingHint textRenderingHint = Gdiplus::TextRenderingHint::TextRenderingHintAntiAlias
		, Gdiplus::SmoothingMode smoothingMode = Gdiplus::SmoothingMode::SmoothingModeHighQuality
		, Gdiplus::PixelOffsetMode pixelOffsetMode = Gdiplus::PixelOffsetMode::PixelOffsetModeHalf) {
		this->textRenderingHint = textRenderingHint;
		this->smoothingMode = smoothingMode;
		this->pixelOffsetMode = pixelOffsetMode;
	}

	inline void SetAlign(DWORD dwAlign, bool bVerticalAlignOffset) {
		this->dwDTFlags = dwAlign | DT_NOPREFIX | DT_WORDBREAK | DT_EDITCONTROL;
		this->bVerticalAlignOffset = bVerticalAlignOffset;
	}

	inline void SetClip(bool clip, int renderWidth, int renderHeight) {
		this->bClip = clip;
		this->renderWidth = renderWidth;
		this->renderHeight = renderHeight;
	}

	//inline void SetOutLine(BYTE outLinePixel, DWORD color) {
	//	this->bOutLine = outLinePixel;
	//	this->nOutLinePixel = outLinePixel;

	//	this->dwOutLineColor = color;
	//}

	inline void SetHotSpot(int hotSpotX = 0, int hotSpotY = 0) {
		this->hotSpotX = hotSpotX;
		this->hotSpotY = hotSpotY;
	}

	inline void SetScale(float xScale = 1.0, float yScale = 1.0) {
		this->xScale = xScale;
		this->yScale = yScale;
	}

	inline void SetAngle(int angle = 0) {
		this->angle = angle;
	}

	inline StrSize GetCharSizeRaw(wchar_t wChar) {
		//Graphics g(hdc);
		//auto font = GetFont(this->logFont);
		//
		//PointF origin(0, 0);
		//
		//RectF boundRect;
		//auto pStringFormat = StringFormat::GenericDefault();
		//g.MeasureString(&wChar, 1, &font, origin, pStringFormat, &boundRect);
		//
		//RectF boundRect_2;
		//auto pStringFormat_2 = StringFormat::GenericTypographic();
		//g.MeasureString(&wChar, 1, &font, origin, pStringFormat_2, &boundRect_2);

		SIZE sz;
		GetTextExtentPoint32(hdc, &wChar, 1, &sz);

		//ABC abc;
		//GetCharABCWidths(hdc, wChar, wChar, &abc);
		//
		//TEXTMETRIC tm;
		//GetTextMetrics(hdc, &tm);
		//
		//sz.cy -= (this->tm.tmInternalLeading + this->tm.tmExternalLeading);

		return *(StrSize*)&sz;
	}

	inline StrSize GetCharSizeWithCache(wchar_t wChar) {
		auto it = charSzCache.find(wChar);

		if (it != charSzCache.end()) {
			return it->second;
		}
		else {
			auto sz = GetCharSizeRaw(wChar);
			charSzCache [wChar] = sz;

			return sz;
		}
	}

#define GetCharSize(wChar) this->GetCharSizeWithCache(wChar)

	inline StrSize GetStrSize(LPCWSTR pStr, size_t pStrLen = -1) {
		RECT change = { 0,0,65535,1 };

		DrawTextW(
			hdc,
			pStr,
			pStrLen == -1 ? wcslen(pStr) : pStrLen,
			&change
			, this->dwDTFlags | DT_CALCRECT
		);

		return StrSize { change.right - change.left,change.bottom - change.top };
	}

	inline CharPos CalculateRange(LPCWSTR pText, LPRECT pRc) {
		this->strPos.clear();

		size_t pTextLen = wcslen(pText);

		if (pTextLen == 0) {
			auto it = charSzCache.cend();
			it--;

			return { it->second.width / 2,it->second.height / 2,0 };
		}

		delete[] this->pStrSizeArr;
		this->pStrSizeArr = nullptr;

		pStrSizeArr = new StrSize [pTextLen + 1];
		memset(pStrSizeArr, 0, sizeof(StrSize) * (pTextLen + 1));

		this->rcWidth = pRc->right - pRc->left;
		this->rcHeight = pRc->bottom - pRc->top;

		this->totalWidth = 0;
		this->totalHeight = 0;
		//this->totalHeight = this->tm.tmInternalLeading;

		size_t pCharStart = 0;
		long maxWidth = 0;

		bool bNegColSpace = nColSpace < 0;
		bool bNegRowSpace = nRowSpace < 0;

		size_t notAtStartCharPos = -1;
		bool bPunctationSkip = false;

		for (size_t pChar = 0; pChar < pTextLen; ) {
			bool newLine = false;		// newline
			bool skipLine = false;		// current line only has /r/n

			long curWidth = 0;
			long curHeight = 0;

			pCharStart = pChar;

			while (true) {
				auto pCurChar = pText + pChar;

				auto curChar = pCurChar [0];
				auto nextChar = pCurChar [1];

				pStrSizeArr [pChar] = GetCharSize(curChar);
				auto charSz = &pStrSizeArr [pChar];

				totalWidth = curWidth;

				curWidth += charSz->width;
				curHeight = max(curHeight, charSz->height);

				if (curWidth > rcWidth) {
					if (NotAtStart(curChar)) {
						notAtStartCharPos = pChar;

						if (NotAtStart(nextChar)) {
							if (pChar != pCharStart) {
								pChar--;

								auto pPreviousChar = pText + pChar;
								auto PreviousChar = pPreviousChar [0];

								auto previousCharSz = &pStrSizeArr [pChar];

								curWidth -= charSz->width;

								curWidth -= previousCharSz->width;
								curWidth -= nColSpace;
							}

							break;
						}
					}
					else {
						if (pChar != pCharStart) {
							auto pPreviousChar = pText + pChar - 1;
							auto PreviousChar = pPreviousChar [0];

							auto previousCharSz = &pStrSizeArr [pChar - 1];

							if (NotAtEnd(PreviousChar)) {
								pChar--;

								curWidth -= charSz->width;

								curWidth -= previousCharSz->width;
								curWidth -= nColSpace;
							}
						}

						break;
					}
				}

				if (curChar == L'\r' && nextChar == L'\n') {
					newLine = true;
					skipLine = (pChar == pCharStart);

					if ((notAtStartCharPos + 1) == pChar) {
						bPunctationSkip = true;
					}

					pChar += 2;

					break;
				}

				pChar++;

				if (pChar > pTextLen) {
					break;
				}

				curWidth += nColSpace;
			}

			if (!skipLine) {
				auto end = min(pChar, pTextLen) - 2 * newLine;

				this->strPos.emplace_back(StrPos {
					pCharStart,			// start
					end,				// end
					end - pCharStart,	// length
					totalWidth,			// width
					//curWidth,			// width
					curHeight,			// height;
					0,					// x
					totalHeight,		// y
					});

#ifdef _DEBUG
				std::wstring str(pText + pCharStart, end - pCharStart);
#endif // _DEBUG

				maxWidth = curWidth != 0
					? max(maxWidth, curWidth - nColSpace)
					: maxWidth;
				//maxWidth = max(maxWidth, curWidth);
				//maxWidth = max(maxWidth, totalWidth);
			}

			if (!bPunctationSkip) {
				// empty line (skipLine == true) also need to add height
				// ...unless it's following a 'not at start' punctuation
				totalHeight += (curHeight + nRowSpace);
			}
			else {
				bPunctationSkip = false;
			}
		}

		const auto& lastStrPos = strPos.back();
		const auto lastCharSize = &pStrSizeArr [lastStrPos.start + lastStrPos.length - 1];

		this->startY = GetStartPosY(totalHeight - nRowSpace, rcHeight);

		auto lastCharPos = CharPos {
			GetStartPosX(lastStrPos.width, rcWidth) - lastCharSize->width / 4
			+ lastStrPos.width + (lastCharSize->width >> 1)
			,startY + lastStrPos.y + (lastCharSize->height >> 1)
			,maxWidth
			,totalHeight - nRowSpace };

		return lastCharPos;
	}

	inline void RenderPerChar(LPCWSTR pText, LPRECT pRc) {
		size_t pTextLen = wcslen(pText);

		if (pTextLen == 0) {
			return;
		}

		delete[] this->pCharPosArr;
		this->pCharPosArr = nullptr;

		pCharPosArr = new CharPos [pTextLen + 1];
		memset(pCharPosArr, 0, sizeof(CharPos) * (pTextLen + 1));

		rcWidth = pRc->right - pRc->left;
		rcHeight = pRc->bottom - pRc->top;

		char scale = 1;
		auto width = abs((rcWidth + this->borderOffsetX * 2) * scale);
		auto height = abs((totalHeight + this->borderOffsetY * 2) * scale);

		if (this->pMemSf == nullptr
			|| this->pMemSf->GetWidth() != width
			|| this->pMemSf->GetHeight() != height) {
			delete this->pMemSf;
			this->pMemSf = nullptr;

			pMemSf = CreateSurface(32, width, height);

			delete this->pBitmap;
			this->pBitmap = nullptr;

			this->pBitmap = new Bitmap(width, height, PixelFormat32bppARGB);
		}

		pMemSf->Fill(BLACK);
		pMemSf->CreateAlpha();

#ifdef _DEBUG
		auto type = pMemSf->GetType();
#endif

		Graphics g(pBitmap);
		
		g.Clear(Color(0, 0, 0, 0));

		g.SetTextRenderingHint(this->textRenderingHint);
		g.SetSmoothingMode(this->smoothingMode);
		g.SetPixelOffsetMode(this->pixelOffsetMode);
		
		auto font = GetFont(this->logFont);

		//Color fontColor(255, 50, 150, 250);
		Color fontColor(255, GetRValue(this->dwTextColor), GetGValue(this->dwTextColor), GetBValue(this->dwTextColor));
		SolidBrush solidBrush(fontColor);

		RECT displayRc = { 0,0,(LONG)this->renderWidth, (LONG)this->renderHeight, };

		auto clip = [this, pRc, displayRc] (int startX, int startY, const StrSize* charSz)->bool {
			if (this->bClip == false) {
				return false;
			}

			RECT charRc = { pRc->left + startX
				,pRc->top + startY
				,pRc->left + startX + charSz->width
				,pRc->top + startY + charSz->height };

			return !(charRc.left < displayRc.right
				&& charRc.right > displayRc.left
				&& charRc.top < displayRc.bottom
				&& charRc.bottom > displayRc.top);
		};

		for (auto& curStrPos : this->strPos) {
#ifdef _DEBUG
			std::wstring str(pText + curStrPos.start, curStrPos.length);
#endif // _DEBUG

			StrSize* charSz = nullptr;

			int x = GetStartPosX(curStrPos.width, rcWidth);
			x -= pStrSizeArr [curStrPos.start].width / 8;

			for (size_t curChar = 0; curChar < curStrPos.length; curChar++) {
				auto offset = curStrPos.start + curChar;
				auto pCurChar = pText + offset;
				charSz = &pStrSizeArr [offset];

				pCharPosArr[offset] = CharPos{ x + pStrSizeArr[curStrPos.start].width / 8
											,this->startY + curStrPos.y
											,0,0 };

				if (!clip(x, (this->startY + curStrPos.y), charSz)) {
					auto positionX = (float)(x)+(float)(this->borderOffsetY);
					auto positionY = (float)(curStrPos.y) + (float)(this->borderOffsetY);
		
					auto status = g.DrawString(pCurChar, 1, &font
						, PointF(positionX, positionY), &solidBrush);
				}

				x += (charSz->width + nColSpace);
			}
		}

#ifdef _BLUR
		Blur blur;
		BlurParams bp = { 0 };
		RECT rc = { 0,0,pBitmap->GetWidth(),pBitmap->GetHeight() };

		bp.expandEdge = false;
		bp.radius = 5;

		blur.SetParameters(&bp);

		pBitmap->ApplyEffect(&blur, &rc);
#endif

#ifdef _DEBUG
		//CLSID pngClsid;
		//GetEncoderClsid(L"image/png", &pngClsid);
		//bitmap.Save(L"F:\\NeoStr.png", &pngClsid, NULL);
#endif // _DEBUG

		BitmapData bitmapData;
		auto bitmapRect = Rect(0, 0, width, height);
		auto lockBitsRet = pBitmap->LockBits(&bitmapRect, ImageLockMode::ImageLockModeWrite, PixelFormat32bppARGB, &bitmapData);
		unsigned int* pRawBitmap = (unsigned int*)bitmapData.Scan0;   // for easy access and indexing

		auto sfCoef = GetSfCoef(pMemSf);

		auto lineSz = sfCoef.pitch;

		for (int y = 0; y < height; y++) {
			auto pData = sfCoef.pData + y * sfCoef.pitch;
			auto pBmp = pRawBitmap + (height - 1 - y) * bitmapData.Stride / 4;

			memcpy(pData, pBmp, lineSz);

			for (int x = 0; x < width; x++) {
				auto pAlphaData = sfCoef.pAlphaData + (height - 1 - y) * sfCoef.alphaPitch + x * sfCoef.alphaByte;
				auto curAlpha = pRawBitmap + (height - 1 - y) * bitmapData.Stride / 4 + x;
				pAlphaData [0] = (*curAlpha & 0xff000000) >> 24;
			}
		}

		ReleaseSfCoef(pMemSf, sfCoef);

		pBitmap->UnlockBits(&bitmapData);

#ifdef _DEBUG
		//_SavetoClipBoard(pMemSf, false);
#endif // _DEBUG

		delete pHwaSf;
		pHwaSf = nullptr;

		pHwaSf = CreateHWASurface(32, width, height, this->hwaType, this->hwaDriver);

		if (this->preMulAlpha) {
			pMemSf->PremultiplyAlpha();		// only needed in DX11 premultiplied mode
		}

		pMemSf->Blit(*pHwaSf);
	}

	inline CharPos GetCharPos(LPCWSTR pText, size_t pos) {
		auto invalid = CharPos { -1, -1, -1, -1 };

		if (pCharPosArr == nullptr) {
			return invalid;
		}

		size_t pTextLen = wcslen(pText);

		if (pTextLen == 0) {
			return invalid;
		}

		if (pTextLen <= pos) {
			return invalid;
		}

		return pCharPosArr [pos];
	}

	inline void DisplayPerChar(LPSURFACE pDst, LPCWSTR pText, LPRECT pRc
		, BlitMode bm = BMODE_TRANSP, BlitOp bo = BOP_COPY, LPARAM boParam = 0, int bAntiA = 0
		, DWORD dwLeftMargin = 0, DWORD dwRightMargin = 0, DWORD dwTabSize = 8) {

		size_t pTextLen = wcslen(pText);

		if (pTextLen == 0) {
			return;
		}

		if (pDst != nullptr && pHwaSf != nullptr && pMemSf != nullptr) {
			auto pSf = pHwaSf;

			int xOffset = -this->borderOffsetX;
			int yOffset = -this->borderOffsetY + this->startY;

			POINT hotSpot = { this->hotSpotX - xOffset,this->hotSpotY - yOffset };

			int xPos = pRc->left + this->hotSpotX;
			int yPos = pRc->top + this->hotSpotY;

			pSf->BlitEx(*pDst, (float)xPos, (float)yPos, this->xScale, this->yScale
				, 0, 0, pSf->GetWidth(), pSf->GetHeight(), &hotSpot, (float)this->angle
				, bm, bo, boParam, bAntiA);

		}
	}
};

// compatible with MMF
#define Font FontW
#define fpFont fpFontW
