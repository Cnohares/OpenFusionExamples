#ifndef _GLOBALFUNC_
#define _GLOBALFUNC_

//ȫ�ִ��ھ��
extern HWND CurrentWindowHandle;

//�����������
extern bool Lock;

//�����������
enum {
	LOCK_CURRENTWINDOW,
	LOCK_FRAMEAREA,
	LOCK_BYWINDOWNAME,
	LOCK_BYRECT,
};

//��ǰ��������ľ�������
extern RECT CurrentLockRect;

//��ǰ��������ľ�����������ڴ��ڵ�ƫ��
extern RECT RectOffset;

//RECT��������� +
RECT operator+(RECT A, RECT B);

//RECT��������� -
RECT operator-(RECT A, RECT B);

//ö�ٴ���ص�
BOOL CALLBACK WINAPIEXT_EnumWindowsProc(
	HWND hwnd,      // handle to parent window
	LPARAM lParam   // application-defined value
);

void UnlockLockedMouse();
HWND ReturnCurrentWindowHandle();

//����ע����ַ���ֵ
int ReturnRegValue(HKEY hkey, LPCSTR lpSubKey, LPCSTR lpValue);

//���ش���˵����߶�
int ReturnMenuHeight();

//���ش���������߶�
int ReturnCaptionHeight();

//���뷨״̬
extern bool IMEState;

//���뷨����
BOOL IMEStateControl(bool State);

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