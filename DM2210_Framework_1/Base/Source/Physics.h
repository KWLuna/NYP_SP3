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
	bool CheckCollision();
	void SATtest(const Vector3 & axis, const std::vector<Vector3>& ptSet, float & minAlong, float & maxAlong);
	bool intersects(Entity* shape1, Entity* shape2);
	bool overlaps(float min1, float max1, float min2, float max2);
	bool isBetweenOrdered(float val, float lowerBound, float upperBound);
	float GetDist();
};

struct Projection
{
	float min;
	float max;

	Projection(float min, float max);
	~Projection();
};
