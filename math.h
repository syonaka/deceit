#pragma once
#include "include.h"

class Vector3
{
public:
	float x, y, z;

	Vector3(float xx, float yy, float zz) : x(xx), y(yy), z(zz) {}
	Vector3()
	{
		this->x = 0.f; this->y = 0.f; this->z = 0.f;
	}
};

class Quat
{
public:
	Vector3 v; float w;
	Quat()
	{
		this->v.x = 0.f; this->v.y = 0.f; this->v.z = 0.f; this->w = 0.f;
	}
};