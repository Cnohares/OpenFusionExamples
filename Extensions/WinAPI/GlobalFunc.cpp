// Common Include
#include	"common.h"

//ȫ�ִ��ھ��
HWND CurrentWindowHandle = NULL;

//ö�ٴ���ص�
BOOL CALLBACK WINAPIEXT_EnumWindowsProc(
	HWND hwnd,      // handle to parent window
	LPARAM lParam   // application-defined value
) {
	DWORD PID;
	GetWindowThreadProcessId(hwnd, &PID);
	//Ϊ���������뵱ǰ����PIDһ��
	if ((PID == GetCurrentProcessId()) && (GetParent(hwnd) == NULL)) {
		//������Ŀ��PID����ľ����ȫ�ֱ���CurrentWindowHandle
		CurrentWindowHandle = hwnd;
		return FALSE;
	}
	return TRUE;
}

//���ص�ǰ���ھ��
HWND ReturnCurrentWindowHandle() {

	CurrentWindowHandle = NULL;

	EnumWindows(
		WINAPIEXT_EnumWindowsProc,
		NULL
	);

	return CurrentWindowHandle;
}

//��ʼ������Ϊ������ʾ����С
RECT InitRect(){
	return RECT{ 0,0, GetSystemMetrics(SM_CXSCREEN) , GetSystemMetrics(SM_CYSCREEN) };
}

//�������
//���ڴ���
void LockMouse(LPRDATA rdPtr, LT Type) {
	//��ǰ���ھ���
	RECT CurrentWindowRect;
	::GetWindowRect(rdPtr->MainWindowHandle, &CurrentWindowRect);

	//��ȡ��������
	RECT ClipRect = InitRect();

	switch (Type) {
		case LOCK_CURRENTWINDOW: {
			::GetWindowRect(rdPtr->MainWindowHandle, &ClipRect);
			break;
		}
		case LOCK_CLIENTAREA: {
			GetCurrentClientRectToScreen(rdPtr->MainWindowHandle, &ClipRect);
			break;
		}
		case LOCK_FRAMEAREA: {
			::GetWindowRect(rdPtr->FrameWindowHandle, &ClipRect);
			break;
		}
		//case:LOCK_WINDOWNAME{
		//	::GetWindowRect(FindWindow(NULL, (LPTSTR)param1), &ClipRect);
		//}
		default: {

		}
	}

	rdPtr->Lock = true;
	rdPtr->LockType = Type;	
	rdPtr->RectOffset = ClipRect - CurrentWindowRect;
	
	rdPtr->CurrentLockRect = ClipRect;
	::ClipCursor(&(rdPtr->CurrentLockRect));

	return;
}
//���ھ���
void LockMouse(LPRDATA rdPtr, RECT Rect, RT Type) {
	//��ǰ���ھ���
	RECT MainWindowRect;
	::GetWindowRect(rdPtr->MainWindowHandle, &MainWindowRect);

	//��ȡ��������
	RECT ClipRect = InitRect();	

	DPOINT Scale = GetFrameScale(rdPtr);

	switch (Type) {
		case RELATIVE_SCREEN: {
			ClipRect = Rect;
			break;
		}
		case RELATIVE_CURRENTWINDOW: {
			RECT CurrentWindowRect;
			::GetWindowRect(rdPtr->MainWindowHandle, &CurrentWindowRect);
			ClipRect = { Rect.left+ CurrentWindowRect.left,Rect.top+ CurrentWindowRect.top ,Rect.right+ CurrentWindowRect.left,Rect.bottom + CurrentWindowRect.top };
			break;
		}
		case RELATIVE_CLIENTAREA: {
			RECT CurrentClinetRect;
			GetCurrentClientRectToScreen(rdPtr->MainWindowHandle, &CurrentClinetRect);
			ClipRect = { Rect.left + CurrentClinetRect.left,Rect.top + CurrentClinetRect.top ,Rect.right + CurrentClinetRect.left,Rect.bottom + CurrentClinetRect.top };
			break;
		}
		case RELATIVE_FRAMEAREA: {
			RECT CurrentFrameRect;
			::GetWindowRect(rdPtr->FrameWindowHandle, &CurrentFrameRect);
			ClipRect = { (LONG)(Rect.left * Scale.x) + CurrentFrameRect.left,(LONG)(Rect.top * Scale.y) + CurrentFrameRect.top ,(LONG)(Rect.right * Scale.x) + CurrentFrameRect.left,(LONG)(Rect.bottom * Scale.y) + CurrentFrameRect.top };
			rdPtr->FrameScale = Scale;
			break;
		}
		default: {

		}
	}

	rdPtr->Lock = true;
	rdPtr->LockType = LOCK_BYRECT;
	rdPtr->RectOffset_Type = Type;
	rdPtr->RectOffset = ClipRect - MainWindowRect;

	rdPtr->CurrentLockRect = ClipRect;
	::ClipCursor(&(rdPtr->CurrentLockRect));

	return;
}

//�ͷ����
void UnlockMouse(LPRDATA rdPtr) {
	::ClipCursor(NULL);
	rdPtr->Lock = false;
}

//��ȡ��ǰ�ͻ������������Ļ�ľ�������
BOOL GetCurrentClientRectToScreen(HWND hWnd, LPRECT lpRect)
{
	BOOL RetBool = 1;
	
	RECT CurrentWindowRect;
	POINT Point = { 0,0 };

	RetBool = RetBool && ::GetClientRect(hWnd, &CurrentWindowRect);
	RetBool = RetBool && ::ClientToScreen(hWnd,&Point);
	*lpRect = CurrentWindowRect + RECT{ Point.x , Point.y , Point.x , Point.y };

	return RetBool;
}

//��ȡ��ǰ���������������Ļ�ľ�������
BOOL GetCurrentFrameRectToScreen(HWND hWnd, LPRECT lpRect,POINT WindowSize)
{
	BOOL RetBool = 1;

	RECT CurrentClientRect;
	POINT Offset = { 0,0 };
	
	RetBool = RetBool && GetCurrentClientRectToScreen(hWnd, &CurrentClientRect);
	Offset = (POINT{ CurrentClientRect.right - CurrentClientRect.left, CurrentClientRect.bottom - CurrentClientRect.top }- WindowSize)/2;

	*lpRect = CurrentClientRect + RECT{ Offset.x , Offset.y , -Offset.x , -Offset.y };

	return RetBool;
}

//���س����������ű���
DPOINT GetFrameScale(LPRDATA rdPtr) {
	//��ȡ��ǰ������ʾ�����С
	RECT CurrentFrameRect;
	::GetWindowRect(rdPtr->FrameWindowHandle, &CurrentFrameRect);
	POINT Cur = { CurrentFrameRect.right - CurrentFrameRect.left,CurrentFrameRect.bottom - CurrentFrameRect.top };

	//��ȡ����ǰ��ʾ����С�����С
	POINT Min = { min(rdPtr->AppW,rdPtr->FrameW),min(rdPtr->AppH,rdPtr->FrameH) };

	return DPOINT{ Cur.x / (double)Min.x,Cur.y / (double)Min.y };
}

//ָ���������
void SetMousePosition(LPRDATA rdPtr, int x, int y, ST Type) {

	//����ָ��������
	POINT Coordinate = {0,0};

	//������Ͳ��Ϸ����򷵻ص�ǰ����
	::GetCursorPos(&Coordinate);

	DPOINT Scale = GetFrameScale(rdPtr);

	switch (Type) {
		case SET_SCREEN: {
			Coordinate = { x,y };
			break;
		}
		case SET_CURRENTWINDOW: {
			RECT CurrentWindowRect;
			::GetWindowRect(rdPtr->MainWindowHandle, &CurrentWindowRect);
			Coordinate = POINT{ x,y } + POINT{ CurrentWindowRect.left, CurrentWindowRect.top };
			break;
		}
		case SET_CLIENTAREA: {
			RECT CurrentClinetRect;
			GetCurrentClientRectToScreen(rdPtr->MainWindowHandle, &CurrentClinetRect);
			Coordinate = POINT{ x,y } + POINT{ CurrentClinetRect.left, CurrentClinetRect.top };
			break;
		}
		case SET_FRAMEAREA: {
			RECT CurrentFrameRect;
			::GetWindowRect(rdPtr->FrameWindowHandle, &CurrentFrameRect);			
			Coordinate = POINT{ (LONG)(x * Scale.x),(LONG)(y * Scale.y) } + POINT{ CurrentFrameRect.left, CurrentFrameRect.top };
			break;
		}
		default: {
			
		}
	}
	
	::SetCursorPos(Coordinate.x, Coordinate.y);

	return;
}

//����ƫ��ֵ
POINT GetOffset(LPRDATA rdPtr, GT Type) {
	
	POINT Offset = { 0,0 };
	RECT Src = { 0,0,0,0 }, Des = { 0,0,0,0 }, Res = { 0,0,0,0 };

	switch (Type) {
		case GET_CLIENTTOCURRENTWINDOW: {
			::GetWindowRect(rdPtr->MainWindowHandle, &Des);
			GetCurrentClientRectToScreen(rdPtr->MainWindowHandle, &Src);
			break;
		}
		case GET_FRAMETOCLIENTAREA: {			
			GetCurrentClientRectToScreen(rdPtr->MainWindowHandle, &Des);
			::GetWindowRect(rdPtr->FrameWindowHandle, &Src);
			break;
		}
		case GET_FRAMEAREATOCURRENTWINDOW: {
			::GetWindowRect(rdPtr->MainWindowHandle, &Des);
			::GetWindowRect(rdPtr->FrameWindowHandle, &Src);
			break;
		}
		default: {
			
		}
	}

	Res = Src - Des;
	Offset = { Res.left,Res.top };

	return Offset;
}

//���ؾ�������
RECT GetRect(LPRDATA rdPtr,GR Type) {
	//Windows�ڴ���DPI����ʱ���ǽ�������ʾ���ķֱ��ʽ������ź��ٴ���
	//��һ��50*50�Ĵ��������Ƿ�������ţ���ȡ���Ĵ�С����50*50
	RECT CurrentRect = InitRect();

	switch (Type) {
		case GET_CURRENTWINDOW: {
			::GetWindowRect(rdPtr->MainWindowHandle, &CurrentRect);		
			break;
		}
		case GET_CLIENTAREA: {
			GetCurrentClientRectToScreen(rdPtr->MainWindowHandle, &CurrentRect);		
			break;
		}
		case GET_FRAMEAREA: {
			::GetWindowRect(rdPtr->FrameWindowHandle, &CurrentRect);
			break;
		}
		default: {

		}
	}

	return CurrentRect;
}

//����DPI����
int ReturnDPIScaling(bool AppScaled) {
	// Get desktop dc
	HDC desktopDc = GetDC(NULL);
	if (AppScaled) {
		return 100;
	}

	// Get native resolution	
	switch (GetDeviceCaps(desktopDc, LOGPIXELSX))
	{
	case 96:
		return 100;
	case 120:
		return 125;
	case 144:
		return 150;
	case 192:
		return 200;
	default:
		return 0;
	}
}

//����ע����ַ���ֵ
char* ReturnRegValue(HKEY hkey,LPWSTR lpSubKey, LPWSTR lpValue) {
	DWORD reg_type = REG_SZ;
	char* reg_value = new char[MAX_PATH];
	DWORD res_size = 0;

	RegGetValue(
		hkey,
		lpSubKey,
		lpValue,
		reg_type,
		NULL,
		(LPBYTE)reg_value,
		&res_size
	);

	/*int result = atoi(reg_value);
	delete[] reg_value;
	return result;*/
	return reg_value;
}

//���뷨״̬
bool IMEState;

//���뷨����
BOOL IMEStateControl(HWND hWnd, bool State)
{
	IMEState = State;
	return ImmSetOpenStatus(ImmGetContext(hWnd), State);
}

//Surface

void ReDisplay(LPRDATA rdPtr) {
	//callRunTimeFunction(rdPtr, RFUNCTION_REDRAW, 0, 0);
	rdPtr->rc.rcChanged = true;
}

DWORD GetFilterIDByFileName(LPRDATA rdPtr, LPCTSTR FilePath) {

	//Surface���
	CImageFilterMgr* pImgMgr = rdPtr->rhPtr->rh4.rh4Mv->mvImgFilterMgr;
	CImageFilter    pFilter(pImgMgr);

	//�ַ���תСд
	auto LowerWStr = [](LPWSTR Str)->LPWSTR {
		int i = 0;
		while (Str[i]) {
			Str[i] = towlower(Str[i]);
			i++;
		}
		return Str;
	};

	//��ȡ��չ����Ӧ��FilterName
	auto  GetFilterName = [LowerWStr](LPCWSTR Name)->LPCWSTR {
		//�����ᱻ����ȫ�ֱ�����
		LPCWSTR ExtList[8][2] = {
			_T(".png"),_T("Portable Network Graphics"),
			_T(".tga"),_T("Targa Bitmap"),
			_T(".bmp"),_T("Windows Bitmap"),
			_T(".jpg"),_T("JPEG"),
			_T(".flc"),_T("Autodesk FLIC"),
			_T(".gif"),_T("Compuserve Bitmap"),
			_T(".avi"),_T("Video For Windows"),
			_T(".pcx"),_T("PaintBrush")
		};

		WCHAR* Ext = new WCHAR[FILENAME_MAX];
		wcscpy_s(Ext, FILENAME_MAX, Name);
		LowerWStr(Ext);

		for (int i = 0; i < 8; i++) {
			if (wcscmp(Ext, ExtList[i][0]) == 0) {
				delete[] Ext;
				return ExtList[i][1];
			}
		}

		delete[] Ext;
		//return _T("Windows Bitmap");
		return ExtList[2][1];
	};

	//��ȡ��չ����Ӧ��FilterID
	auto GetFilterID = [pImgMgr, GetFilterName](LPCWSTR Name) -> DWORD {
		LPCWSTR FilterName = GetFilterName(Name);
		for (int i = 0; i < pImgMgr->GetFilterCount(); i++) {
			if (wcscmp(pImgMgr->GetFilterNameW(i), FilterName) == 0) {
				DWORD FilterID = pImgMgr->GetFilterID(i);
				return pImgMgr->GetFilterID(i);
			}
		}
		return pImgMgr->GetFilterID(0);
	};

	//��ȡ��չ��
	WCHAR* Ext = new WCHAR[FILENAME_MAX];
	_wsplitpath_s(FilePath, NULL, 0, NULL, 0, NULL, 0, Ext, FILENAME_MAX);

	DWORD FilterID = GetFilterID(Ext);
	
	delete[] Ext;

	return FilterID;
}

//���д����̵߳Ľ�����
std::deque <LPTSTR> RunApplicationName;

//�����µ��ڴ�ռ临�ƣ�������ָ����Ƭ�ڴ�ռ��ָ��
void AddNewApplicationName(LPTSTR lpApplicationName) {
	size_t total_length = _MAX_FNAME + _MAX_EXT;
	LPTSTR ANCL = new WCHAR[total_length];
	wcscpy_s(ANCL, total_length, PathFindFileName(lpApplicationName));
	RunApplicationName.push_back(ANCL);
}

//����ȫ���ռ�
void DeleteAllApplicationName() {
	for (auto t = RunApplicationName.begin(); t != RunApplicationName.end(); t++) {
		delete[](*t);
	}
	RunApplicationName.clear();
}

//ͨ��λ����������ռ�
void DeleteRunApplicationNameByPos(size_t pos) {
	delete[] RunApplicationName[pos];

	auto t = RunApplicationName.begin() + pos;
	RunApplicationName.erase(t);	
}

//ͨ����������������ռ�
void DeleteRunApplicationName(LPCTSTR ApplicationName) {
	for (auto t = RunApplicationName.begin(); t != RunApplicationName.end();) {
		//�����Ƿ�ɾ��Ԫ�ظ��µ�����
		if (wcscmp(ApplicationName, *t) == 0) {
			delete[](*t);
			t = RunApplicationName.erase(t);
		}
		else {
			t++;
		}
	}
	return;
}

//ͨ�����ֹͣȫ���������߳�
void StopAllApplication() {
	
	//���û�����г�����������һ�׶�
	if (RunApplicationName.size() == 0) {
		return;
	}
		
	//��ȡ����
	HANDLE	snapshot;
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//ѭ������
	PROCESSENTRY32* info;
	info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);

	Process32First(snapshot, info);
	while (Process32Next(snapshot, info) != FALSE) {
		for (size_t i = 0; i < RunApplicationName.size(); i++) {
			//������һ�����������		
			if (wcscmp(RunApplicationName[i], info->szExeFile) == 0) {
				//PROCESS_TERMINATE��ʾΪ����������,FALSE=�ɼ̳�,info.th32ProcessID=����ID    
				TerminateProcess(OpenProcess(PROCESS_TERMINATE, FALSE, info->th32ProcessID), 0);
			}
		}
	}
	
	DeleteAllApplicationName();	
	
	delete info;
	return;
}


DWORD GetProcessIDByName(LPCTSTR ApplicationName) {
	//���ز���
	DWORD	ProcessID = 0;

	//��ȡ����
	HANDLE	snapshot;
	snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);

	//ѭ������
	PROCESSENTRY32* info;
	info = new PROCESSENTRY32;
	info->dwSize = sizeof(PROCESSENTRY32);

	Process32First(snapshot, info);
	while (Process32Next(snapshot, info) != FALSE) {
		//������һ�����������		
		if (wcscmp(ApplicationName, info->szExeFile) == 0) {
			ProcessID = info->th32ProcessID;
			break;
		}
	}

	delete info;
	return ProcessID;
}


