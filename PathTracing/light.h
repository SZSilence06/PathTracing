#ifndef LIGHT_H
#define LIGHT_H

#include "RenderObject.h"
#include <vector>
using namespace std;

class Light{
public:
	enum TYPE{
		POINT_LIGHT,
		DIRECTIONAL_LIGHT,
		AREA_LIGHT
	};
protected:
	Color _light;
	TYPE _type;

public:
	virtual ~Light(){};
	virtual Ray GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal) = 0;
	virtual Color CalcLighting(Ray ray, float distance, Vector3f LightNormal) = 0;
	virtual bool OnTheLight(Vector3f pos) = 0;
	Color GetLightColor();
};

class PointLight :public Light{
protected:
	Vector3f _pos;
public:
	PointLight(Vector3f pos, Color light){
		this->_pos = pos;
		this->_light = light;
		this->_type = POINT_LIGHT;
	}
	virtual Ray GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal);
	virtual Color CalcLighting(Ray ray, float distance, Vector3f LightNormal);
	virtual bool OnTheLight(Vector3f pos);
};

class DirectionalLight :public Light{
protected:
	Vector3f _dir;
public:
	DirectionalLight(Vector3f dir, Color light){
		this->_dir = dir.Normalize();
		this->_light = light;
		this->_type = DIRECTIONAL_LIGHT;
	}
	virtual Ray GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal);
	virtual Color CalcLighting(Ray ray, float distance, Vector3f LightNormal);
	virtual bool OnTheLight(Vector3f pos);
};

class AreaLight :public Light{
protected:
	vector<RenderObject*> _faces;
public:
	AreaLight(Color light){
		this->_light = light;
		this->_type = AREA_LIGHT;
	}
	virtual ~AreaLight();
	void SetFaces(const vector<RenderObject*>& faces);
	Vector3f GenerateLightPoint();
	virtual Ray GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal);
	virtual Color CalcLighting(Ray ray, float distance, Vector3f LightNormal);
	virtual bool OnTheLight(Vector3f pos);
};

#endif