#include "stdafx.h"
#include "Camera.h"

Camera::Camera(Vector3f pos, Vector3f up, Vector3f lookat, float fov, float aspect, float zn, float zf)
{
	_pos = pos;
	_up = up.Normalize();
	_fov = fov;
	_aspect = aspect;
	_zn = zn;
	_zf = zf;
	_front = (lookat - pos).Normalize();
	_right = Vector3f::Cross(_front, _up).Normalize();
}

Ray Camera::GenerateRay(int x, int y, int width, int height)
{
	float newWidth = _zn * tan(_fov / 2 * PI / 180) * 2;
	float newX = (float)x / width * newWidth;
	float newY = (float)y / height * newWidth;

	Ray ray;
	ray.start = this->_pos;
	ray.dir = (_zn * _front + (newX - newWidth / 2) * _right + (newWidth / 2 - newY) *_up).Normalize();
	return ray;
}