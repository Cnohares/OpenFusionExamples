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
DWORD GetFilterIDByFileName(LPRDATA rdPtr, LPCTSTR FilePath);

void GetValidScale(float* scale);
void GetMaxmiumDivide(int* divide);

//GetAlphaBuff
LPBYTE GetBuff(LPSURFACE Src);
//StretchAlpha
BOOL Stretch(LPBYTE Src, int SW, int SH, LPBYTE Des, int DW, int DH);
//StretchSurfaceWithAlpha
BOOL Stretch(LPSURFACE Src, LPSURFACE Des, LPRDATA rdPtr);
//StretchSurface
BOOL Stretch(LPSURFACE Src, LPSURFACE Des);


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