#ifndef _GLOBALFUNC_
#define _GLOBALFUNC_

//ȫ�ִ��ھ��
extern HWND CurrentWindowHandle;

//ö�ٴ���ص�
BOOL CALLBACK WINAPIEXT_EnumWindowsProc(
	HWND hwnd,      // handle to parent window
	LPARAM lParam   // application-defined value
);

//���ص�ǰ���ھ��
HWND ReturnCurrentWindowHandle();

//��ʼ������Ϊ������ʾ����С
RECT InitRect();

//�������
//���ڴ���
void LockMouse(LPRDATA rdPtr, LT Type);
//���ھ���
void LockMouse(LPRDATA rdPtr, RECT Rect, RT Type);

//�ͷ����
void UnlockMouse(LPRDATA rdPtr);

//��ȡ��ǰ�ͻ������������Ļ�ľ�������
BOOL GetCurrentClientRectToScreen(HWND hWnd,LPRECT lprect);

//��ȡ��ǰ���������������Ļ�ľ�������
BOOL GetCurrentFrameRectToScreen(HWND hWnd, LPRECT lpRect, POINT offset);

//���س����������ű���
DPOINT GetFrameScale(LPRDATA rdPtr);

//ָ���������
void SetMousePosition(LPRDATA rdPtr, int x, int y, ST Type);

//����ƫ��ֵ
POINT GetOffset(LPRDATA rdPtr, GT Type);

//���ؾ�������
RECT GetRect(LPRDATA rdPtr, GR Type);

//����DPI����
int ReturnDPIScaling(bool AppScaled);

//����ע����ַ���ֵ
char* ReturnRegValue(HKEY hkey, LPWSTR lpSubKey, LPWSTR lpValue);

//���뷨״̬
extern bool IMEState;

//���뷨����
BOOL IMEStateControl(HWND hWnd, bool State);

//Surface
void ReDisplay(LPRDATA rdPtr);

//��ȡ��չ����Ӧ��FilterName
LPCWSTR GetFilterName(LPRDATA rdPtr, LPCWSTR Name);
DWORD GetFilterIDByFileName(LPRDATA rdPtr, LPCTSTR FilePath);

void GetValidScale(float* scale);
void GetMaxmiumDivide(int* divide);

//CustomStretch
void Stretch(bool YReverse, LPBYTE Src, uint SW, uint SH, uint SBW, LPBYTE Des, uint DW, uint DH, uint DBW);
//StretchSurface
void Stretch(LPSURFACE Src, LPSURFACE Des, bool HighQuality);
//StretchBlt Stretch Surface
BOOL Stretch(LPSURFACE Src, LPSURFACE Des);

//Blt To Surface
void BltToSurface(HDC Src, int SH, int SW, LPSURFACE Des);

//Mutex
extern std::mutex mtx;

//Save to Clipboard
void _SavetoClipBoard(LPSURFACE Src, LPRDATA rdPtr, bool release);
//Save to Clipboard
void SavetoClipBoard(LPSURFACE Src, LPRDATA rdPtr, bool release);
//Save to File
void _SavetoFile(LPSURFACE Src, LPCWSTR FilePath, LPRDATA rdPtr, bool release);
//Save to File
void SavetoFile(LPSURFACE Src, LPCWSTR FilePath, LPRDATA rdPtr, bool release);

//���д����̵߳ľ��
extern std::deque <LPTSTR> RunApplicationName;

//���̹���
void StopAllApplication();
void DeleteRunApplicationName(LPCTSTR ApplicationName);
void DeleteRunApplicationNameByPos(size_t pos);
void AddNewApplicationName(LPTSTR lpApplicationName);
void DeleteAllApplicationName();

DWORD GetProcessIDByName(LPCTSTR ApplicationName);

#endif // !_GLOBALFUNC_