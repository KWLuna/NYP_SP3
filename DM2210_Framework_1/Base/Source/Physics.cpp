#include "Physics.h"


Physics::Physics()
{
}


Physics::~Physics()
{
}

bool Physics::RayTraceDist(Vector3 obj1a, Vector3 obj1b, Vector3 obj2a, Vector3 obj2b)
{
	// obj1a is unit direction vector of ray
	Vector3 dirfrac;
	dirfrac.x = 1.0f / obj1a.x;
	dirfrac.y = 1.0f / obj1a.y;
	dirfrac.z = 1.0f / obj1a.z;
	// obj2a is the corner of AABB with minimal coordinates - left bottom, obj2b is maximal corner
	// obj1b is origin of ray
	float t1 = (obj2a.x - obj1b.x)*dirfrac.x;
	float t2 = (obj2b.x - obj1b.x)*dirfrac.x;
	float t3 = (obj2a.y - obj1b.y)*dirfrac.y;
	float t4 = (obj2b.y - obj1b.y)*dirfrac.y;
	float t5 = (obj2a.z - obj1b.z)*dirfrac.z;
	float t6 = (obj2b.z - obj1b.z)*dirfrac.z;

	float tmin = Math::Max(Math::Max(Math::Min(t1, t2), Math::Min(t3, t4)), Math::Min(t5, t6));
	float tmax = Math::Min(Math::Min(Math::Max(t1, t2), Math::Max(t3, t4)), Math::Max(t5, t6));
	float xd = 1;
	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
	if (tmax < 0)
	{
		m_dist = tmax;
		return false;
	}

	// if tmin > tmax, ray doesn't intersect AABB
	if (tmin > tmax)
	{
		m_dist = tmax;
		return false;
	}

	m_dist = tmin;
	return true;
}

float Physics::GetDist()
{
	return m_dist;
}


Projection::Projection(float min, float max)
{
	this->min = min;
	this->max = max;
}

Projection::~Projection()
{

}

// Shape1 and Shape2 must be CONVEX HULLS
bool Physics::intersects(Entity* shape1, Entity* shape2)
{
	Vector3 vertices1[4] = {shape1->pos};
	Vector3 vertices2[4] = {shape2->pos};
	vertices1[0].x += shape1->scale.x * 0.5f;
	vertices1[1].x -= shape1->scale.x * 0.5f;
	vertices1[2].z += shape1->scale.z * 0.5f;
	vertices1[3].z -= shape1->scale.z * 0.5f;

	vertices2[0].x += shape2->scale.x * 0.5f;
	vertices2[1].x -= shape2->scale.x * 0.5f;
	vertices2[2].z += shape2->scale.z * 0.5f;
	vertices2[3].z -= shape2->scale.z * 0.5f;

	// Get the normals for one of the shapes,
	Vector3 axes1[4];
	Vector3 axes2[4];
	for (int i = 0; i < 4; i++)
	{
		axes1[i].x = -vertices1[i].y;
		axes1[i].y = vertices1[i].x;
	}
	for (int i = 0; i < 4; i++)
	{
		axes2[i].x = -vertices2[i].y;
		axes2[i].y = vertices2[i].x;
	}

	// loop over the axes1
	for (int i = 0; i < 4; i++)
	{
		// project both shapes onto the axis
		double min = axes1[i].Dot(vertices1[i]);
		double max = min;
		for (int j = 0; j < 4; j++)
		{
			double p = axes1[j].Dot(vertices1[j]);
			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}
		Projection p1(min, max);
		min = axes1[i].Dot(vertices2[i]);
		max = min;
		for (int j = 0; j < 4; j++)
		{
			double p = axes1[j].Dot(vertices2[j]);
			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}
		Projection p2(min, max);
		if (p1.max < p2.min)
		{
			return false;
		}
	}
	for (int i = 0; i < 4; i++)
	{
		// project both shapes onto the axis
		double min = axes2[i].Dot(vertices1[i]);
		double max = min;
		for (int j = 0; j < 4; j++)
		{
			double p = axes2[j].Dot(vertices1[j]);
			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}
		Projection p1(min, max);
		min = axes2[i].Dot(vertices2[i]);
		max = min;
		for (int j = 0; j < 4; j++)
		{
			double p = axes2[j].Dot(vertices2[j]);
			if (p < min)
			{
				min = p;
			}
			else if (p > max)
			{
				max = p;
			}
		}
		Projection p2(min, max);
		if (p2.max < p1.min)
		{
			return false;
		}
	}
	return true;
}
