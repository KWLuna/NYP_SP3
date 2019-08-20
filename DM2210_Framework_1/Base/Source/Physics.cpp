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
bool Physics::intersects(Entity* obj1, Entity* obj2)
{ 
	Vector3 vertices1[3];
	vertices1[0].x = obj1->scale.x * -0.5f;
	vertices1[0].z = obj1->scale.z * -0.5f;
	vertices1[1].x = obj1->scale.x * 0.5f;
	vertices1[1].z = obj1->scale.z * -0.5f;
	vertices1[2].x = obj1->scale.x * -0.5f;
	vertices1[2].z = obj1->scale.z * 0.5f;

	Vector3 vertices2[3];
	vertices2[0].x = obj2->scale.x * -0.5f;
	vertices2[0].z = obj2->scale.z * -0.5f;
	vertices2[1].x = obj2->scale.x * 0.5f;
	vertices2[1].z = obj2->scale.z * -0.5f;
	vertices2[2].x = obj2->scale.x * -0.5f;
	vertices2[2].z = obj2->scale.z * 0.5f;

	for (int i = 0; i < 4; i++)
	{
		vertices1[i].x = cosf(Math::DegreeToRadian(obj1->angle)) * vertices1[i].x - sinf(Math::DegreeToRadian(obj1->angle)) * vertices1[i].z;
		vertices1[i].z = sinf(Math::DegreeToRadian(obj1->angle)) * vertices1[i].x + cosf(Math::DegreeToRadian(obj1->angle)) * vertices1[i].z;
	}
	for (int i = 0; i < 4; i++)
	{
		vertices2[i].x = cosf(Math::DegreeToRadian(obj2->angle)) * vertices2[i].x - sinf(Math::DegreeToRadian(obj2->angle)) * vertices2[i].z;
		vertices2[i].z = sinf(Math::DegreeToRadian(obj2->angle)) * vertices2[i].x + cosf(Math::DegreeToRadian(obj2->angle)) * vertices2[i].z;
	}
	for (int i = 0; i < 4; i++)
	{
		vertices1[i] += obj1->pos;
		vertices2[i] += obj2->pos;
	}
	Vector3 axes1[2];
	Vector3 axes2[2];
	axes1[0] = (vertices1[1] - vertices1[0]).Normalize();
	axes1[1] = (vertices1[2] - vertices1[0]).Normalize();
	axes2[0] = (vertices2[1] - vertices2[0]).Normalize();
	axes2[1] = (vertices2[2] - vertices2[0]).Normalize();

	Projection objproj1;
	if ((obj2->pos - obj1->pos).x > 0)
	{
		objproj1.min = vertices1[0].x;
		objproj1.max = vertices1[1].x;
		
	}

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
		if (p1.max > p2.min)
		{
			return false;
		}
		if (p1.min > p2.max)
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
		if (p2.max > p1.min)
		{
			return false;
		}
		if (p2.min > p1.max)
		{
			return false;
		}
	}
	return true;

}