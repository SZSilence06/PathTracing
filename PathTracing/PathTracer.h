#ifndef PATHTRACER_H
#define PATHTRACER_H

#include "Camera.h"
#include "RenderObject.h"
#include "light.h"
#include <vector>

using namespace std;

class Scene{
private:
	vector<RenderObject*> _Objects;
	vector<Light*> _lights;
public:
	Scene(){}
	~Scene();
	int GetNumberOfObjects();
	RenderObject* GetRenderObject(int id);
	void AddRenderObject(RenderObject *obj);
	void AddLight(Light* light);
	int GetNumberOfLights();
	Light* GetLight(int id);
};

class PathTracer{
private:
	//image
	Color* m_buffer;
	int m_width;
	int m_height;
	
	// camera
	Camera m_camera;

	//scene
	Scene m_scene;
public:
	PathTracer();
	PathTracer(int width, int height);
	~PathTracer();
	void SetCamera(Vector3f pos, Vector3f up, Vector3f lookat, float fov, float aspect, float zn, float zf);
	void ClearBuffer(Color color);
	void SetBuffer(int x, int y, Color color);
    int GetWidth();
	int GetHeight();
	Color GetBuffer(int x, int y);
	void TracePath(int x, int y);
	void GetIntersectResult(Ray ray, RenderObject** obj, Vector3f* pos);
	Ray SampleRay(Vector3f pos, Vector3f normal);
	Color ShootRay(Ray& ray, int depth);
	Color CalcEmit(Vector3f& pos);
	Color CalcDirectLighting(Vector3f& pos, Vector3f& normal, Material& material, Vector3f& OutDir);
	Color PhongLighting(Vector3f pos, Vector3f normal, Material material, Color light, Vector3f LightDir);
	bool CalcTransDir(Vector3f& InputDir, Vector3f& normal, float refractivity,Vector3f& OutDir);
	void SetScene(Scene* scene);
	Scene* GetScene();
};

#endif