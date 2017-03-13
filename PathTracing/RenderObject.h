#ifndef RENDEROBJECT_H
#define RENDEROBJECT_H

#include "Matrix.h"

struct Color{
	float r, g, b;
	Color(){}
	Color(float r, float g, float b) :r(r), g(g), b(b){}

	Color operator +=(Color color);
	Color operator +(Color color);
	Color operator *(Color color);
	Color operator *(float a);
	Color operator /(float a);
	bool operator ==(Color color);
	bool operator !=(Color color);
};

struct Pixel{
	GLubyte r;
	GLubyte g;
	GLubyte b;
};

struct Ray{
	Vector3f start;
	Vector3f dir;
};

struct Material{
	Color diffuse;
	Color specular;
	Color transmissive;
	float refractivity;
	float roughness;
};

class RenderObject{
public:	
	enum TYPE{
		SPHERE,
		TRIANGLE,
		RECT,
		CIRCLE
	};

protected:
	TYPE _type;
	Material _material;
	Color _light;
	bool _isLight;

public:
	RenderObject();
	int GetType(){ return _type; }
	virtual bool  Intersect(Ray ray, Vector3f& HitPoint, float* dist) = 0;
	virtual bool  OnTheSurface(Vector3f pos) = 0;
	virtual Vector3f GetSurfaceNormal(Vector3f point) = 0;
	virtual Vector3f UniformSample() = 0;
	virtual float Area() = 0;
	void SetMaterial(Material material){ _material = material; }
	Material GetMaterial();
	void SetLight(bool isLight);
	void SetLightColor(Color light);
	bool IsLight();
	Color GetLight();
	bool Specular();
	bool Transmissive();
	bool Diffuse();
};

class Sphere:public RenderObject{
private:
	Vector3f _center;
	float _radius;
public:
	Sphere(Vector3f center, float radius) :_center(center), _radius(radius){ this->_type = SPHERE; }
	virtual bool  Intersect(Ray ray, Vector3f& HitPoint, float* dist);
	virtual bool  OnTheSurface(Vector3f pos);
	virtual Vector3f GetSurfaceNormal(Vector3f point);
	virtual Vector3f UniformSample();
	virtual float Area();
};

class Triangle :public RenderObject{
private:
	Vector3f _v1, _v2, _v3;
	Vector3f _normal;
	float _area;
public:
	Triangle(Vector3f v1, Vector3f v2, Vector3f v3) :_v1(v1), _v2(v2), _v3(v3){ 
		this->_type = TRIANGLE;
		_normal = (Vector3f::Cross(v1 - v2, v1 - v3)).Normalize();
		_area = (Vector3f::Cross(_v1 - _v2, _v1 - _v3).Norm()) / 2;
	}
	virtual bool  Intersect(Ray ray, Vector3f& HitPoint, float* dist);
	virtual bool  OnTheSurface(Vector3f pos);
	virtual Vector3f GetSurfaceNormal(Vector3f point);
	virtual Vector3f UniformSample();
	virtual float Area();
};

class Rect:public RenderObject{
private:
	Vector3f _v1, _v2, _v3,_v4;
	Vector3f _normal;
	float _area;
public:
	Rect(Vector3f v1, Vector3f v2, Vector3f v3, Vector3f v4) :_v1(v1), _v2(v2), _v3(v3), _v4(v4){
		this->_type = RECT;
		_normal = (Vector3f::Cross(v1 - v2, v1 - v3)).Normalize();
		_area = _area = (Vector3f::Cross(_v1 - _v2, _v1 - _v3).Norm());
	}
	virtual bool  Intersect(Ray ray, Vector3f& HitPoint, float* dist);
	virtual bool  OnTheSurface(Vector3f pos);
	virtual Vector3f GetSurfaceNormal(Vector3f point);
	virtual Vector3f UniformSample();
	virtual float Area();
};

class Circle :public RenderObject{
private:
	Vector3f _center;
	float _radius;
	Vector3f _normal;
	float _area;
public:
	Circle(Vector3f center, float radius, Vector3f normal) :_center(center), _radius(radius), _normal(normal) {
		this->_type = CIRCLE;
		this->_area = PI * _radius *_radius;
	}
	virtual bool  Intersect(Ray ray, Vector3f& HitPoint, float* dist);
	virtual bool  OnTheSurface(Vector3f pos);
	virtual Vector3f GetSurfaceNormal(Vector3f point);
	virtual Vector3f UniformSample();
	virtual float Area();
};

#endif