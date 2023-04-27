#pragma once

//#define _FFMPEG

constexpr auto ONMONITORCHANGE = 13;

typedef struct DOUBLEPOINT
{
	DOUBLE  x;
	DOUBLE  y;
} DPOINT;

typedef struct Gauss3_Coefs {
	int N;
	double sigma;
	double B;
	double b[4];
} GCoef;

//�����������
typedef enum _LOCKTYPE {
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
inline RECT operator+(RECT A, RECT B) {
	A.left += B.left;
	A.right += B.right;
	A.top += B.top;
	A.bottom += B.bottom;
	return A;
}

//RECT��������� +=
inline RECT operator +=(RECT A, RECT B) {
	A.left += B.left;
	A.right += B.right;
	A.top += B.top;
	A.bottom += B.bottom;
	return A;
}

//RECT��������� -
inline RECT operator-(RECT A, RECT B) {
	A.left -= B.left;
	A.right -= B.right;
	A.top -= B.top;
	A.bottom -= B.bottom;
	return A;
}

//RECT��������� -=
inline RECT operator -=(RECT A, RECT B) {
	A.left -= B.left;
	A.right -= B.right;
	A.top -= B.top;
	A.bottom -= B.bottom;
	return A;
}

//POINT��������� +
inline POINT operator +(POINT A, POINT B) {
	A.x += B.x;
	A.y += B.y;
	return A;
}

//POINT��������� +=
inline POINT operator +=(POINT A, POINT B) {
	A.x += B.x;
	A.y += B.y;
	return A;
}

//POINT��������� -
inline POINT operator -(POINT A, POINT B) {
	A.x -= B.x;
	A.y -= B.y;
	return A;
}

//POINT��������� -=
inline POINT operator -=(POINT A, POINT B) {
	A.x -= B.x;
	A.y -= B.y;
	return A;
}

//POINT��������� *
inline POINT operator *(POINT A, LONG B) {
	A.x *= B;
	A.y *= B;
	return A;
}

//POINT��������� /
inline POINT operator /(POINT A, LONG B) {
	A.x /= B;
	A.y /= B;
	return A;
}

