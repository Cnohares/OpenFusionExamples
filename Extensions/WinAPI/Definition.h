#ifndef _DEFINITION_
#define _DEFINITION_

constexpr auto Dir_X = false;
constexpr auto Dir_Y = true;

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

struct RGBA {
	double r;
	double g;
	double b;
	double a;
	//int r;
	//int g;
	//int b;
	//int a;
};

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

//RGBA��������� +
RGBA operator +(RGBA A, RGBA B);

//RGBA��������� -
RGBA operator -(RGBA A, RGBA B);

//RGBA��������� +=
RGBA operator +=(RGBA A, RGBA B);

//RGBA��������� -=
RGBA operator -=(RGBA A, RGBA B);


//RGBA��ֵ����
RGBA Range(RGBA A);
double Range(double A);

//RGBA��������� *
RGBA operator *(RGBA A, double B);

//RGBA��������� /
RGBA operator /(RGBA A, double B);

//RGBA��������� *
RGBA operator *(double B, RGBA A);

//RGBA��������� /
RGBA operator /(double B, RGBA A);

//RGBA��������� >>
RGBA operator >>(RGBA A, int B);

#endif // !_DEFINITION_

