#include "stdafx.h"
#include "RenderObject.h"

#include <cmath>

Color Color::operator +(Color color)
{
	Color ret;
	ret.r = r + color.r;
	ret.g = g + color.g;
	ret.b = b + color.b;
	/*if (ret.r > 1.0f){
		ret.r = 1.0f;
	}
	if (ret.g > 1.0f){
		ret.g = 1.0f;
	}
	if (ret.b > 1.0f){
		ret.b = 1.0f;
	}*/
	return ret;
}

Color Color::operator +=(Color color)
{
	r += color.r;
	g += color.g;
	b += color.b;
	/*if (r > 1.0f){
		r = 1.0f;
	}
	if (g > 1.0f){
		g = 1.0f;
	}
	if (b > 1.0f){
		b = 1.0f;
	}*/
	return *this;
}

Color Color::operator *(Color color)
{
	Color ret;
	ret.r = r * color.r;
	ret.g = g * color.g;
	ret.b = b * color.b;
	return ret;
}

Color Color::operator *(float a)
{
	Color ret;
	ret.r = r * a;
	ret.g = g * a;
	ret.b = b * a;
	return ret;
}

Color Color::operator /(float a)
{
	Color ret;
	ret.r = r / a;
	ret.g = g / a;
	ret.b = b / a;
	return ret;
}

bool Color::operator ==(Color color)
{
	float er = r - color.r;
	float eg = g - color.g;
	float eb = b - color.b;
	return fabs(er) < 1e-4 && fabs(eg) < 1e-4 && fabs(eb) < 1e-4;
}

bool Color::operator !=(Color color)
{
	return !(*this == color);
}

Material RenderObject::GetMaterial()
{
	return this->_material;
}

RenderObject::RenderObject()
{
	_isLight = false;
}

void RenderObject::SetLight(bool isLight)
{
	_isLight = isLight;
}

void RenderObject::SetLightColor(Color light)
{
	_light = light;
}

Color RenderObject::GetLight()
{
	return _light;
}

bool RenderObject::IsLight()
{
	return _isLight;
}

bool RenderObject::Specular()
{
	return _material.specular != Color(0,0,0);
}

bool RenderObject::Transmissive()
{
	return _material.transmissive != Color(0, 0, 0);
}

bool RenderObject::Diffuse()
{
	return _material.diffuse != Color(0, 0, 0);
}

bool Sphere::Intersect(Ray ray, Vector3f& HitPoint, float* dist)
{
	Vector3f v = ray.start - this->_center;
	float dDotv = Vector3f::Dot(ray.dir, v);
	if (dDotv > 0){
		return false;
	}
	float dcl = dDotv * dDotv - (Vector3f::Dot(v, v) - _radius*_radius);
	if (dcl < 0){
		return false;
	}
	float sqrtdcl = sqrt(dcl);
	*dist = -dDotv - sqrtdcl;
	HitPoint = ray.start + *dist * ray.dir;
	return true;
}

bool Sphere::OnTheSurface(Vector3f pos)
{
	return (fabs((pos - _center).Norm() - _radius) < 1e-4);
}

Vector3f Sphere::GetSurfaceNormal(Vector3f point)
{
	return (point - _center).Normalize();
}

float Sphere::Area()
{
	return 4 * PI *_radius * _radius;
}

Vector3f Sphere::UniformSample()
{
	Vector3f v;
	return v;
}

bool Triangle::Intersect(Ray ray, Vector3f& HitPoint, float* dist)
{
	float dist1 = Vector3f::Dot(_normal, _v1 - ray.start);  //光线起点到三角形所在平面的距离
	float cosine = Vector3f::Dot(_normal, ray.dir);
	if (fabs(cosine) < 1e-4){    //光线与三角形所在平面平行
		return false;
	}
	float dist2 = dist1 / cosine;
	if (dist2 < 0){
		return false;
	}
	Vector3f p = ray.start + dist2 * ray.dir;

	if (!OnTheSurface(p)){
		return false;
	}
	HitPoint = p;
	*dist = dist2;
	return true;
}

bool Triangle::OnTheSurface(Vector3f pos)
{
	float Sabc = _area;
	float Spab = (Vector3f::Cross(pos - _v1, pos - _v2).Norm()) / 2;
	float Spac = (Vector3f::Cross(pos - _v1, pos - _v3).Norm()) / 2;
	float Spbc = (Vector3f::Cross(pos - _v2, pos - _v3).Norm()) / 2;
	return (fabs(Sabc - Spab - Spac - Spbc) < 1e-4);
}

Vector3f Triangle::UniformSample()
{
	int a1 = rand() % 10000;
	float a = (float)a1 *0.0001;
	int b1 = rand() % 10000;
	float b = (float)b1 *0.0001;
	if (a + b > 1){
		a = 1 - a;
		b = 1 - b;
	}
	return a*(_v2 - _v1) + b*(_v3 - _v1) + _v1;
}

Vector3f Triangle::GetSurfaceNormal(Vector3f point)
{
	return _normal;
}

float Triangle::Area()
{
	return _area;
}

/*************************************/
bool Rect::Intersect(Ray ray, Vector3f& HitPoint, float* dist)
{
	float dist1 = Vector3f::Dot(_normal, _v1 - ray.start);  //光线起点到三角形所在平面的距离
	float cosine = Vector3f::Dot(_normal, ray.dir);
	if (fabs(cosine) < 1e-4){    //光线与三角形所在平面平行
		return false;
	}
	float dist2 = dist1 / cosine;
	if (dist2 < 0){
		return false;
	}
	Vector3f p = ray.start + dist2 * ray.dir;

	if (!OnTheSurface(p)){
		return false;
	}
	HitPoint = p;
	*dist = dist2;
	return true;
}

bool Rect::OnTheSurface(Vector3f pos)
{
	float Sabcd = _area * 2;
	float Spab = (Vector3f::Cross(pos - _v1, pos - _v2).Norm());
	float Spbc = (Vector3f::Cross(pos - _v2, pos - _v3).Norm());
	float Spcd = (Vector3f::Cross(pos - _v3, pos - _v4).Norm());
	float Spda = (Vector3f::Cross(pos - _v4, pos - _v1).Norm());
	return (fabs(Sabcd - Spab - Spbc - Spcd - Spda) < 1e-3);
}

Vector3f Rect::UniformSample()
{
	float a = rand()*1.0f / RAND_MAX;
	float b = rand()*1.0f / RAND_MAX;
	return a*(_v2 - _v1) + b*(_v3 - _v1) + _v1;
}

Vector3f Rect::GetSurfaceNormal(Vector3f point)
{
	return _normal;
}

float Rect::Area()
{
	return _area;
}

/*********************************************/
bool Circle::Intersect(Ray ray, Vector3f& HitPoint, float* dist)
{
	float dist1 = Vector3f::Dot(_normal, _center - ray.start);  //光线起点到平面的距离
	float cosine = Vector3f::Dot(_normal, ray.dir);
	if (fabs(cosine) < 1e-4){    //光线与平面平行
		return false;
	}
	float dist2 = dist1 / cosine;
	if (dist2 < 0){
		return false;
	}
	Vector3f p = ray.start + dist2 * ray.dir;

	if (!OnTheSurface(p)){
		return false;
	}
	HitPoint = p;
	*dist = dist2;
	return true;
}

bool Circle::OnTheSurface(Vector3f pos)
{
	return (pos - _center).Norm() <= _radius;
}

Vector3f Circle::UniformSample()
{
	return Vector3f();
}

Vector3f Circle::GetSurfaceNormal(Vector3f point)
{
	return _normal;
}

float Circle::Area()
{
	return _area;
}