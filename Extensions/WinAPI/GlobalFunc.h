#ifndef _GLOBALFUNC_
#define _GLOBALFUNC_

//ȫ�ִ��ھ��
extern HWND CurrentWindowHandle;

//�����������
typedef enum _LOCKTYPE{
	LOCK_CURRENTWINDOW,
	LOCK_CLIENTAREA,
	LOCK_FRAMEAREA,	
	LOCK_BYRECT,
}LT;

//��������ʱ�����λ��
typedef enum _RELATIVETYPE {
	RELATIVE_SCREEN,
	RELATIVE_CURRENTWINDOW,
	RELATIVE_CLIENTAREA,
	RELATIVE_FRAMEAREA,	
}RT;

//ָ������������
typedef enum _SETMOUSETYPE {
	SET_SCREEN,
	SET_CURRENTWINDOW,
	SET_CLIENTAREA,
	SET_FRAMEAREA,
}ST;

//��ȡƫ��ֵ���
typedef enum _GETOFFSETTYPE {
	GET_CLIENTTOCURRENTWINDOW,
	GET_FRAMETOCLIENTAREA,
	GET_FRAMEAREATOCURRENTWINDOW,
}GT;

//��ȡ�����������
typedef enum _GETRECT {
	GET_CURRENTWINDOW,
	GET_CLIENTAREA,
	GET_FRAMEAREA,	
}GR;

//RECT��������� +
RECT operator+(RECT A, RECT B);

//RECT��������� +=
RECT operator +=(RECT A, RECT B);

//RECT��������� -
RECT operator-(RECT A, RECT B);

//RECT��������� -=
RECT operator -=(RECT A, RECT B);

//POINT��������� +
POINT operator +(POINT A, POINT B);

//POINT��������� +=
POINT operator +=(POINT A, POINT B);

//POINT��������� -
POINT operator -(POINT A, POINT B);

//POINT��������� -=
POINT operator -=(POINT A, POINT B);

//POINT��������� *
POINT operator *(POINT A, LONG B);

//POINT��������� /
POINT operator /(POINT A, LONG B);


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

typedef struct DOUBLEPOINT
{
	DOUBLE  x;
	DOUBLE  y;
} DPOINT;

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