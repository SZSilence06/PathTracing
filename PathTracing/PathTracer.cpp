#include "stdafx.h"
#include "PathTracer.h"

Scene::~Scene()
{
	for (int i = 0; i < _Objects.size(); i++){
		delete _Objects[i];
	}
	for (int i = 0; i < _lights.size(); i++){
		delete _lights[i];
	}
}

int Scene::GetNumberOfObjects()
{
	return _Objects.size();
}

RenderObject* Scene::GetRenderObject(int id)
{
	return _Objects[id];
}

void Scene::AddRenderObject(RenderObject *obj)
{
	_Objects.push_back(obj);
}

void Scene::AddLight(Light* light)
{
	_lights.push_back(light);
}

Light* Scene::GetLight(int id)
{
	return _lights[id];
}

int Scene::GetNumberOfLights()
{
	return _lights.size();
}

PathTracer::PathTracer()
{

}

PathTracer::PathTracer(int width, int height)
{
	m_width = width;
	m_height = height;
	m_buffer = new Color[width * height];
}

PathTracer::~PathTracer()
{
	delete[] m_buffer;
}

void PathTracer::SetBuffer(int x, int y, Color color)
{
	ASSERT(x < m_width);
	ASSERT(y < m_height);
	*(m_buffer + m_width*y + x) = color;
}

int PathTracer::GetWidth()
{
	return m_width;
}

int PathTracer::GetHeight()
{
	return m_height;
}

void PathTracer::ClearBuffer(Color color)
{
	for (int i = 0; i < m_width; i++){
		for (int j = 0; j < m_height; j++){
			SetBuffer(i, j, color);
		}
	}
}

Color PathTracer::GetBuffer(int x, int y)
{
	ASSERT(x < m_width);
	ASSERT(y < m_height);
	return *(m_buffer + m_width*y + x);
}

void PathTracer::SetCamera(Vector3f pos, Vector3f up, Vector3f lookat, float fov, float aspect, float zn, float zf)
{
	m_camera = Camera(pos, up, lookat, fov, aspect, zn, zf);
}

void PathTracer::SetScene(Scene *scene)
{
	m_scene = *scene;
}

Scene* PathTracer::GetScene()
{
	return &m_scene;
}

void PathTracer::TracePath(int x, int y)
{
	ASSERT(x < m_width);
	ASSERT(y < m_height);
	
	const int samples = 5000;

	Ray ray = m_camera.GenerateRay(x, y, m_width, m_height);
	Color color = Color(0, 0, 0);
	for (int i = 0; i < samples; i++){
		color += ShootRay(ray, 0);
	}
	color = color / samples;
	//color = color * 2 * PI;
	if (color.r > 1.0f){
		color.r = 1.0f;
	}
	if (color.g > 1.0f){
		color.g = 1.0f;
	}
	if (color.b > 1.0f){
		color.b = 1.0f;
	}
	color.r = sqrt(color.r);
	color.g = sqrt(color.g);
	color.b = sqrt(color.b);
	SetBuffer(x, y, color);
}

void PathTracer::GetIntersectResult(Ray ray, RenderObject** obj, Vector3f* pos)
{
	float MinDist = FLT_MAX;
	for (int i = 0; i < m_scene.GetNumberOfObjects(); i++){
		RenderObject* obj2 = m_scene.GetRenderObject(i);
		Vector3f HitPoint;
		float dist;
		if (obj2->Intersect(ray, HitPoint, &dist)){
			if (dist < MinDist && fabs(dist)>1e-4){
				MinDist = dist;
				*pos = HitPoint;
				*obj = obj2;
			}
		}
	}
	if (MinDist == FLT_MAX){
		*obj = NULL;
	}
}

Color PathTracer::ShootRay(Ray& ray, int depth)
{
	Color color = Color(0, 0, 0);
	RenderObject* obj;
	Vector3f HitPoint;

	if (depth > 5){
		return color;
	}
	GetIntersectResult(ray, &obj, &HitPoint);
	color += CalcEmit(HitPoint);
	if (obj){
		//color += CalcDirectLighting(HitPoint, obj->GetSurfaceNormal(HitPoint), obj->GetMaterial(), -ray.dir);
		Vector3f norm = obj->GetSurfaceNormal(HitPoint);
		float cosine = Vector3f::Dot(norm, ray.dir);
		if (cosine > 0){
			norm = -norm;
			cosine = -cosine;
		}
		if (obj->Diffuse()){
			Ray diffuseRay = SampleRay(HitPoint, norm);
			float cosine2 = Vector3f::Dot(diffuseRay.dir, norm);
			color += ShootRay(SampleRay(HitPoint, norm), depth + 1) * fabs(cosine2) * obj->GetMaterial().diffuse;
		}
		if (obj->Specular()){
			Vector3f reflectDir = ray.dir - 2 * cosine * norm;
			Ray reflect;
			reflect.start = HitPoint;
			reflect.dir = reflectDir.Normalize();
			if (fabs(obj->GetMaterial().roughness) < 1e-3){
				color += ShootRay(reflect, depth + 1) * obj->GetMaterial().specular;
			}
			else{
				bool retry = true;
				while (retry){
					Ray reflect2 = SampleRay(HitPoint, norm);
					float sine2 = (Vector3f::Cross(reflect.dir, reflect2.dir)).Norm();
					if (fabs(sine2) <= obj->GetMaterial().roughness){
						retry = false;
						color += ShootRay(reflect2, depth + 1) * obj->GetMaterial().specular;
					}
				}
			}
		}
		if (obj->Transmissive()){
			Vector3f transDir;
			if (CalcTransDir(ray.dir, norm, obj->GetMaterial().refractivity, transDir)) {
				Ray trans;
				trans.start = HitPoint;
				trans.dir = transDir;
				color += ShootRay(trans, depth + 1) * obj->GetMaterial().transmissive;
			}
			else{
				Ray trans;
				trans.start = HitPoint;
				trans.dir = transDir;
				color += ShootRay(trans, depth + 1) * (obj->GetMaterial().transmissive + obj->GetMaterial().specular);
			}
		}
	}
	return color;
}

Color PathTracer::CalcEmit(Vector3f& pos)
{
	Color color = Color(0, 0, 0);
	for (int i = 0; i < m_scene.GetNumberOfLights(); i++){
		Light* light = m_scene.GetLight(i);
		if (light->OnTheLight(pos)){
			color += light->GetLightColor();
		}
	}
	return color;
}

Color PathTracer::CalcDirectLighting(Vector3f& pos, Vector3f& normal, Material& material, Vector3f& OutDir)
{
	Color color = Color(0, 0, 0);
	for (int i = 0; i < m_scene.GetNumberOfLights(); i++){
		Light* light = m_scene.GetLight(i);
		float dist1;
		Vector3f LightNormal;
		Ray ray = light->GenerateRay(pos, dist1, LightNormal);
		RenderObject* obj;
		Vector3f HitPoint;
		
		GetIntersectResult(ray, &obj, &HitPoint);
		if (obj && pos != HitPoint && (pos - HitPoint).Norm() < dist1){  //±»ÕÚµ²
			continue;
		}

		float cosine = fabs(Vector3f::Dot(ray.dir, normal));
		Color InputLight = light->CalcLighting(ray, dist1, LightNormal);
		color += InputLight * cosine * material.diffuse;
		
		ray.dir = -ray.dir;
		Vector3f reflectDir = ray.dir - 2 * cosine * normal;
		if (reflectDir == OutDir){
			color += InputLight * material.specular;
		}

		if (material.transmissive != Color(0, 0, 0)){
			Vector3f TransDir;
			if (CalcTransDir(ray.dir, normal, material.refractivity, TransDir)){
				if (TransDir == OutDir){
					color += InputLight * material.transmissive;
				}
			}
		}
	}
	return color;
}

bool PathTracer::CalcTransDir(Vector3f& InputDir, Vector3f& normal, float refractivity, Vector3f& OutDir)
{
	float cosine1 = Vector3f::Dot(normal, InputDir);
	float sine1 = sqrt(1 - cosine1 * cosine1);
	float sine2;
	if (cosine1 < 0){
		sine2 = sine1 / refractivity;
	}
	else{
		sine2 = sine1 * refractivity;
	}
	if (sine2 <= 1){
		float cosine2 = sqrt(1 - sine2 * sine2);
		float tan2 = sine2 / cosine2;
		Vector3f transDir;
		if (cosine1 > 0){
			OutDir = (InputDir - (sine1 / tan2 - cosine1) * normal).Normalize();
		}
		else{
			OutDir = (InputDir + (cosine1 - sine1 / tan2) * normal).Normalize();
		}
		return true;
	}
	else{
		OutDir = (InputDir - 2 * cosine1*normal).Normalize();
	}
	return false;
}

Ray PathTracer::SampleRay(Vector3f pos, Vector3f normal)
{
	float f1 = rand()*(1.0f / RAND_MAX);
	float f2 = rand()*(1.0f / RAND_MAX);
	float f3 = sqrt(f2);
	float theta = 2 * PI * f1;
	Vector3f w = normal;
	Vector3f u;
	if (fabs(w.x) > 1e-3){
		u = (Vector3f::Cross(Vector3f(0, 1, 0), w)).Normalize();
	}
	else{
		u = (Vector3f::Cross(Vector3f(1, 0, 0), w)).Normalize();
	}
	Vector3f v = Vector3f::Cross(w, u);
	Ray ray;
	ray.start = pos;
	ray.dir = (u*cos(theta)*f3 + v*sin(theta)*f3 + w*sqrt(1 - f3)).Normalize();
	return ray;
}