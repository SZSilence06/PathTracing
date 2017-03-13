#include "stdafx.h"
#include "Matrix.h"
#include "stdlib.h"
#include "Math.h"

Vector3f::Vector3f()
{
	x = y = z = 0;
}

Vector3f::Vector3f(float x,float y,float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

float Vector3f::Norm()
{
	return sqrt(x*x + y*y + z*z);
}

Vector3f Vector3f::Cross(Vector3f a,Vector3f b)
{
	Vector3f v;
	v.x = a.z*b.y - a.y*b.z;
	v.y = a.x*b.z - a.z*b.x;
	v.z = a.y*b.x - a.x*b.y;
	return v;
}

float Vector3f::Dot(Vector3f a,Vector3f b)
{
	return a.x*b.x + a.y*b.y + a.z*b.z;
}

Vector3f operator *(Vector3f v,float a)
{
	Vector3f ret;
	ret.x = v.x * a;
	ret.y = v.y * a;
	ret.z = v.z * a;
	return ret;
}

Vector3f operator *(float a,Vector3f v)
{
	return v * a;
}

Vector3f Vector3f::Normalize()
{
	float norm = Norm();
	ASSERT(Norm()!=0);
	x /= norm;
	y /= norm;
	z /= norm;
	return *this;
}

Vector3f Vector3f::operator +(Vector3f a)
{
	Vector3f v;
	v.x = this->x + a.x;
	v.y = this->y + a.y;
	v.z = this->z + a.z;
	return v;
}

Vector3f Vector3f::operator -(Vector3f a)
{
	Vector3f v;
	v.x = this->x - a.x;
	v.y = this->y - a.y;
	v.z = this->z - a.z;
	return v;
}

Vector3f Vector3f::operator -()
{
	Vector3f v;
	v.x = -this->x;
	v.y = -this->y;
	v.z = -this->z;
	return v;
}

bool Vector3f::operator ==(Vector3f a)
{
	float ex = fabs(x - a.x);
	float ey = fabs(y - a.y);
	float ez = fabs(z - a.z);
	return ex < 1e-4 && ey < 1e-4 &&ez < 1e-4;
}

bool Vector3f::operator !=(Vector3f a)
{
	return !(*this == a);
}

/****************************Vector4f********************************/
Vector4f::Vector4f()
{
	memset(_data,0,sizeof(_data));
}

Vector4f::Vector4f(float a1,float a2,float a3,float a4)
{
	_data[0] = a1;
	_data[1] = a2;
	_data[2] = a3;
	_data[3] = a4;
}

Vector4f Vector4f::operator *(Matrix a)
{
	Vector4f ret;
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			ret._data[i] += this->_data[j] * a._data[j][i];
		}
	}
	return ret;
}

Matrix::Matrix()
{
	memset(_data,0,sizeof(_data));
}

Matrix::Matrix(float **a)
{
	int i,j;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			_data[i][j] = a[i][j];
		}
	}
}

Matrix Matrix::operator *(Matrix a)
{
	int i,j,k;
	Matrix ret;
	for(i=0;i<4;i++){
		for(j=0;j<4;j++){
			for(k=0;k<4;k++){
				ret._data[i][j] += a._data[i][k] * this->_data[k][j];
			}
		}
	}
	return ret;
}

Matrix Matrix::Identity()
{
	Matrix matrix;
	matrix._data[0][0] = matrix._data[1][1] = matrix._data[2][2] = matrix._data[3][3] = 1;
	return matrix;
}

Matrix Matrix::Perspective(float fov, float aspect, float zn, float zf)
{
	Matrix matrix;

	matrix._data[0][0] = 1 / (tan(fov * 0.5f) *aspect) ;
	matrix._data[1][1] = 1 / tan(fov * 0.5f) ;
	matrix._data[2][2] = zf / (zf - zn) ;
	matrix._data[2][3] = 1.0f;
	matrix._data[3][2] = (zn * zf) / (zn - zf);

	return matrix;
}

Matrix Matrix::LookAt(Vector3f position,Vector3f up,Vector3f lookAt)
{
	 // Calculate d
    Vector3f d = lookAt - position;
    d.Normalize();

    // Calculate r
    Vector3f r;
	r = Vector3f::Cross(d,up);
    r.Normalize();

    // Calculate up
	up = Vector3f::Cross(r,d);
	up.Normalize();

    // Fill in the view matrix entries.
    float x = -Vector3f::Dot(position,r);
    float y = -Vector3f::Dot(position,up);
    float z = -Vector3f::Dot(position,d);

    Matrix M;
    M._data[0][0] = r.x; 
    M._data[1][0] = r.y; 
    M._data[2][0] = r.z; 
    M._data[3][0] = x;   

    M._data[0][1] = up.x;
    M._data[1][1] = up.y;
    M._data[2][1] = up.z;
    M._data[3][1] = y;  

    M._data[0][2] = d.x; 
    M._data[1][2] = d.y; 
    M._data[2][2] = d.z; 
    M._data[3][2] = z;   

    M._data[0][3] = 0.0f;
    M._data[1][3] = 0.0f;
    M._data[2][3] = 0.0f;
    M._data[3][3] = 1.0f;

    return M;
}

Matrix Matrix::Viewport(int x,int y,int width,int height,int MinZ,int MaxZ)
{
	Matrix ret;
	ret._data[0][0] = (float)width/2;
	ret._data[1][1] = -(float)height/2;
	ret._data[2][2] = MaxZ - MinZ;
	ret._data[3][0] = x + (float)width/2;
	ret._data[3][1] = y = (float)height/2;
	ret._data[3][2] = MinZ;
	ret._data[3][3] = 1;
	return ret;
}



