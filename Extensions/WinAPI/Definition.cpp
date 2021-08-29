// Common Include
#include	"common.h"

//RECT��������� +
RECT operator +(RECT A, RECT B) {
	A.left += B.left;
	A.right += B.right;
	A.top += B.top;
	A.bottom += B.bottom;
	return A;
}

//RECT��������� +=
RECT operator +=(RECT A, RECT B) {
	A.left += B.left;
	A.right += B.right;
	A.top += B.top;
	A.bottom += B.bottom;
	return A;
}

//RECT��������� -
RECT operator -(RECT A, RECT B) {
	A.left -= B.left;
	A.right -= B.right;
	A.top -= B.top;
	A.bottom -= B.bottom;
	return A;
}

//RECT��������� -
RECT operator -=(RECT A, RECT B) {
	A.left -= B.left;
	A.right -= B.right;
	A.top -= B.top;
	A.bottom -= B.bottom;
	return A;
}

//POINT��������� +
POINT operator +(POINT A, POINT B) {
	A.x += B.x;
	A.y += B.y;
	return A;
}

//POINT��������� +=
POINT operator +=(POINT A, POINT B) {
	A.x += B.x;
	A.y += B.y;
	return A;
}

//POINT��������� -
POINT operator -(POINT A, POINT B) {
	A.x -= B.x;
	A.y -= B.y;
	return A;
}

//POINT��������� -=
POINT operator -=(POINT A, POINT B) {
	A.x -= B.x;
	A.y -= B.y;
	return A;
}

//POINT��������� *
POINT operator *(POINT A, LONG B) {
	A.x *= B;
	A.y *= B;
	return A;
}

//POINT��������� /
POINT operator /(POINT A, LONG B) {
	A.x /= B;
	A.y /= B;
	return A;
}

//RGBA��ֵ����
RGBA Range(RGBA A) {
	A.r = Range(A.r);
	A.g = Range(A.g);
	A.b = Range(A.b);
	A.a = Range(A.a);

	return A;
}
double Range(double A) {
	return max(0.0, min(255.0, A));
}

//RGBA��������� *
RGBA operator +(RGBA A, RGBA B) {
	A.r += B.r;
	A.g += B.g;
	A.b += B.b;
	A.a += B.a;

	//A.r = max(0, min(255, A.r));
	//A.g = max(0, min(255, A.g));
	//A.b = max(0, min(255, A.b));
	//A.a = max(0, min(255, A.a));

	return A;
}

//RGBA��������� /
RGBA operator -(RGBA A, RGBA B) {
	A.r -= B.r;
	A.g -= B.g;
	A.b -= B.b;
	A.a -= B.a;

	//A.r = max(0, min(255, A.r));
	//A.g = max(0, min(255, A.g));
	//A.b = max(0, min(255, A.b));
	//A.a = max(0, min(255, A.a));

	return A;
}

//RGBA��������� *
RGBA operator *(RGBA A, double B) {
	A.r = A.r * B;
	A.g = A.g * B;
	A.b = A.b * B;
	A.a = A.a * B;
	
	//A.r = (int)(A.r * B);
	//A.g = (int)(A.g * B);
	//A.b = (int)(A.b * B);
	//A.a = (int)(A.a * B);

	//A.r = max(0, min(255, A.r));
	//A.g = max(0, min(255, A.g));
	//A.b = max(0, min(255, A.b));
	//A.a = max(0, min(255, A.a));

	return A;
}

//RGBA��������� /
RGBA operator /(RGBA A, double B) {
	A.r = A.r / B;
	A.g = A.g / B;
	A.b = A.b / B;
	A.a = A.a / B;
	
	//A.r = (int)(A.r / B);
	//A.g = (int)(A.g / B);
	//A.b = (int)(A.b / B);
	//A.a = (int)(A.a / B);

	//A.r = max(0, min(255, A.r));
	//A.g = max(0, min(255, A.g));
	//A.b = max(0, min(255, A.b));
	//A.a = max(0, min(255, A.a));

	return A;
}

//RGBA��������� *
RGBA operator *(double B, RGBA A) {
	return A * B;
}

//RGBA��������� /
RGBA operator /(double B, RGBA A) {
	return A / B;
}
