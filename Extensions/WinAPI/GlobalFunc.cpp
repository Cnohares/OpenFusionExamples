// Common Include
#include	"common.h"


//ȫ�ִ��ھ��
HWND CurrentWindowHandle = NULL;

//�����������
bool Lock = false;

//��ǰ��������ľ�������
RECT CurrentWindowRect;

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

//�ͷ����
void UnlockLockedMouse() {
	if (Lock) {
		::ClipCursor(NULL);
		Lock = false;
	}
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

//ͨ��λ�����������ռ�
void DeleteRunApplicationNameByPos(size_t pos) {
	delete[] RunApplicationName[pos];

	auto t = RunApplicationName.begin() + pos;
	RunApplicationName.erase(t);	
}

//ͨ�����������������ռ�
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

