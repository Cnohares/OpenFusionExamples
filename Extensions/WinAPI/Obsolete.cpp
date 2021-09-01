//Funcs that don't use but may use as a reference later


////��ȡ�������򵽼�����
//short WINAPI DLLExport BitBltFrameArea(LPRDATA rdPtr, long param1, long param2) {
//	// ��ȡ
//	// �ο�������
//	// https://docs.microsoft.com/en-us/windows/win32/gdi/capturing-an-image
//
//	// Source Area
//	RECT CurrentFrameRect;
//	::GetWindowRect(rdPtr->FrameWindowHandle, &CurrentFrameRect);
//	int FrameWidth = CurrentFrameRect.right - CurrentFrameRect.left;
//	int FrameHeight = CurrentFrameRect.bottom - CurrentFrameRect.top;
//
//	// Defination
//	HDC hdcWindow;
//	HDC hdcMemDC = NULL;
//	HBITMAP hbmScreen = NULL;
//
//	// Retrieve the handle to a display device context.
//	hdcWindow = GetDC(rdPtr->FrameWindowHandle);
//
//	// Create a compatible DC, which is used in a BitBlt from the window DC.
//	hdcMemDC = CreateCompatibleDC(hdcWindow);
//
//	// Create a compatible bitmap from the Window DC.
//	hbmScreen = CreateCompatibleBitmap(hdcWindow, FrameWidth, FrameHeight);
//
//	// Select the compatible bitmap into the compatible memory DC.
//	SelectObject(hdcMemDC, hbmScreen);
//
//	// Bit block transfer into our compatible memory DC.
//	BitBlt(hdcMemDC,
//		0, 0,
//		FrameWidth, FrameHeight,
//		hdcWindow,
//		0, 0,
//		SRCCOPY);
//
//	// �ݴ浽������
//	OpenClipboard(rdPtr->MainWindowHandle);
//	EmptyClipboard();
//	SetClipboardData(CF_BITMAP, hbmScreen);
//	CloseClipboard();
//
//	// Clean up.
//	DeleteObject(hbmScreen);
//	DeleteObject(hdcMemDC);
//	ReleaseDC(rdPtr->FrameWindowHandle, hdcWindow);
//
//	//����������
//	int Width = CNC_GetIntParameter(rdPtr);
//	int Height = CNC_GetIntParameter(rdPtr);
//	bool SaveToFile = CNC_GetIntParameter(rdPtr);
//	LPCTSTR FilePath = (LPCTSTR)CNC_GetStringParameter(rdPtr);
//
//	cSurface img;
//	LPSURFACE proto = nullptr;
//	CImageFilterMgr* pImgMgr = rdPtr->rhPtr->rh4.rh4Mv->mvImgFilterMgr;
//	CImageFilter    pFilter(pImgMgr);
//
//	//������ֱ�Ӵ�HBITMAP��ȡ��Ϣ
//	//BITMAPINFO* bmp = (BITMAPINFO*)malloc(sizeof(BITMAPINFO));
//	//memset(bmp, 0, sizeof(*bmp));
//	//bmp->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
//	//GetDIBits(hdcMemDC, hbmScreen, 0, 1, NULL, (BITMAPINFO*)bmp, DIB_RGB_COLORS);
//
//	//�Ӽ������ȡλͼ��Ϣ
//	OpenClipboard(rdPtr->MainWindowHandle);
//	BITMAPINFO* bmp = (BITMAPINFO*)GlobalLock(GetClipboardData(CF_DIB));
//
//	//Surface��ȡλͼ��Ϣ
//	GetSurfacePrototype(&proto, 24, ST_MEMORYWITHDC, SD_DIB);
//	img.Create(FrameWidth, FrameHeight, proto);
//	img.LoadImage(bmp, GetDIBBitmap(bmp));
//	//img->SetTransparentColor(transCol);
//
//	CloseClipboard();
//
//	//����
//	cSurface ResizedImg;
//	ResizedImg.Clone(img);
//
//	img.Delete();
//	img.Create(Width, Height, proto);
//
//	ResizedImg.Stretch(img, 0, 0, Width, Height, BMODE_OPAQUE, BOP_COPY, 0, STRF_RESAMPLE);
//
//	//������ź��ͼ�񵽼�����
//	OpenClipboard(rdPtr->MainWindowHandle);
//	EmptyClipboard();
//
//	HGLOBAL cb = GlobalAlloc(GMEM_MOVEABLE, img.GetDIBSize());
//	BITMAPINFO* OutPut = (BITMAPINFO*)GlobalLock(cb);
//
//	img.SaveImage(OutPut, (BYTE*)(OutPut + 1) - 4);
//	SetClipboardData(CF_DIB, OutPut);
//
//	GlobalUnlock(cb);
//	CloseClipboard();
//
//	//���浽�ļ�	
//	if (!SaveToFile) {
//		return 0;
//	}
//
//	//��ȡJPEG��ʽ��FilterID
//	auto GetFilterID = [pImgMgr]() -> DWORD {
//		for (int i = 0; i < pImgMgr->GetFilterCount(); i++)
//		{
//			if (wcscmp(pImgMgr->GetFilterNameW(i), _T("JPEG")) == 0) {
//				return pImgMgr->GetFilterID(i);
//			}
//		}
//		return pImgMgr->GetFilterID(0);
//	};
//
//	ExportImage(pImgMgr, FilePath, &img, GetFilterID());
//
//	return 0;
//}