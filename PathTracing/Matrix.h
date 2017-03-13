#ifndef MATRIX_H
#define MATRIX_H

#define PI 3.1415926

class Vector3f;
class Vector4f;
class Matrix;

class Vector3f
{
public:
	float x;
	float y;
	float z;
public:
	Vector3f();
	Vector3f(float a1,float a2,float a3);
	float Norm();
	Vector3f Normalize();
	Vector3f operator +(Vector3f a);
	Vector3f operator -(Vector3f a);
	Vector3f operator -();
	bool operator ==(Vector3f a);
	bool operator !=(Vector3f a);
	
	static Vector3f Cross(Vector3f a,Vector3f b);
	static float Dot(Vector3f a,Vector3f b);
};

Vector3f operator *(Vector3f v, float a);
Vector3f operator *(float a,Vector3f v);

class Vector4f
{
public:
	float _data[4];
public:
	Vector4f();
	Vector4f(float a1,float a2,float a3,float a4);

	Vector4f operator *(Matrix a);
};

class Matrix
{
public:
	float _data[4][4];
public:
	Matrix();
	Matrix(float **a);
	Matrix operator *(Matrix a);

	static Matrix Identity();
	static Matrix Perspective(float fov, float aspect, float zn, float zf);
	static Matrix LookAt(Vector3f position,Vector3f up,Vector3f lookAt);
	static Matrix Viewport(int x,int y,int width,int height,int MinZ,int MaxZ);
};

#endif