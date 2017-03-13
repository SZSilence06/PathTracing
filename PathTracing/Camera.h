#ifndef CAMERA_H
#define CAMERA_H

#include "Matrix.h"
#include "RenderObject.h"

class Camera{
private:
	Vector3f _pos;
	Vector3f _up;
	Vector3f _lookat;
	Vector3f _front;
	Vector3f _right;
	float _fov;
	float _aspect;
	float _zn;
	float _zf;
public:
	Camera(){}
	Camera(Vector3f pos, Vector3f up, Vector3f lookat, float fov, float aspect, float zn, float zf);
	Ray GenerateRay(int x, int y,int width,int height);
};

#endif