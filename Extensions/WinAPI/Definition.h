#ifndef _DEFINITION_
#define _DEFINITION_

typedef struct DOUBLEPOINT
{
	DOUBLE  x;
	DOUBLE  y;
} DPOINT;

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

#endif // !_DEFINITION_

