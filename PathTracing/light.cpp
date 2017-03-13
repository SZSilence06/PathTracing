#include "stdafx.h"
#include "light.h"

Color Light::GetLightColor()
{
	return _light;
}

Ray PointLight::GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal)
{
	Ray ray;
	ray.start = pos;
	ray.dir = (_pos - pos).Normalize();
	distance = (_pos - pos).Norm();
	return ray;
}

Color PointLight::CalcLighting(Ray ray, float distance, Vector3f LightNormal){
	return _light;
}

bool PointLight::OnTheLight(Vector3f pos)
{
	return (pos == _pos);
}

Ray DirectionalLight::GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal)
{
	Ray ray;
	ray.start = pos;
	ray.dir = -_dir;
	distance = FLT_MAX;
	return ray;
}

Color DirectionalLight::CalcLighting(Ray ray, float distance, Vector3f LightNormal){
	return _light;
}

bool DirectionalLight::OnTheLight(Vector3f pos)
{
	return false;
}

AreaLight::~AreaLight()
{
	for (int i = 0; i < _faces.size(); i++){
		delete _faces[i];
	}
}

void  AreaLight::SetFaces(const vector<RenderObject*>& faces)
{
	_faces = faces;
}

bool AreaLight::OnTheLight(Vector3f pos)
{
	for (int i = 0; i < _faces.size(); i++){
		RenderObject* obj = _faces[i];
		if (obj->OnTheSurface(pos)){
			return true;
		}
	}
	return false;
}

Ray AreaLight::GenerateRay(Vector3f pos, float& distance, Vector3f& LightNormal)
{
	vector<float> areas;
	float area = 0;
	Ray ray;
	for (int i = 0; i < _faces.size(); i++){
		RenderObject* obj = _faces[i];
		area += obj->Area();
		areas.push_back(area);
	}
	int rand1 = rand() % 10000;
	float rand2 = rand1 * 0.0001;
	for (int i = 0; i < _faces.size(); i++){
		if (areas[i] / area > rand2){
			RenderObject* obj = _faces[i];
			Vector3f lightPos = obj->UniformSample();
			ray.start = pos;
			ray.dir = (lightPos - pos).Normalize();
			distance = (lightPos - pos).Norm();
			LightNormal = obj->GetSurfaceNormal(lightPos);
			break;
		}
	}
	return ray;
}

Color AreaLight::CalcLighting(Ray ray, float distance, Vector3f LightNormal){
	Color color = Color(0, 0, 0);
	float cosine = Vector3f::Dot(LightNormal, ray.dir);
	return _light * cosine;
}