#pragma once
#include "Vector3.h"
#include "Vector2.h"
#include <vector>
#include "Entity.h"

class Physics
{
	float m_dist;
public:
	Physics();
	~Physics();
	bool RayTraceDist(Vector3 obj1a, Vector3 obj1b, Vector3 obj2a, Vector3 obj2b);
	bool intersects(Entity* shape1, Entity* shape2);
	float GetDist();
};

struct Projection
{
	float min;
	float max;

	Projection();
	Projection(float min, float max);
	~Projection();
};
