#include "Weapons.h"



Weapons::Weapons ()
{
}


Weapons::~Weapons()
{
}

//bool Weapons::RayTraceDist(Vector3 obj1a, Vector3 obj1b, Vector3 obj2a, Vector3 obj2b)
//{
//	// obj1a is unit direction vector of ray
//	Vector3 dirfrac;
//	dirfrac.x = 1.0f / obj1a.x;
//	dirfrac.y = 1.0f / obj1a.y;
//	dirfrac.z = 1.0f / obj1a.z;
//	// obj2a is the corner of AABB with minimal coordinates - left bottom, obj2b is maximal corner
//	// obj1b is origin of ray
//	float t1 = (obj2a.x - obj1b.x)*dirfrac.x;
//	float t2 = (obj2b.x - obj1b.x)*dirfrac.x;
//	float t3 = (obj2a.y - obj1b.y)*dirfrac.y;
//	float t4 = (obj2b.y - obj1b.y)*dirfrac.y;
//	float t5 = (obj2a.z - obj1b.z)*dirfrac.z;
//	float t6 = (obj2b.z - obj1b.z)*dirfrac.z;
//
//	float tmin = Math::Max(Math::Max(Math::Min(t1, t2), Math::Min(t3, t4)), Math::Min(t5, t6));
//	float tmax = Math::Min(Math::Min(Math::Max(t1, t2), Math::Max(t3, t4)), Math::Max(t5, t6));
//	float xd = 1;
//	// if tmax < 0, ray (line) is intersecting AABB, but the whole AABB is behind us
//	if (tmax < 0)
//	{
//		m_dist = tmax;
//		return false;
//	}
//
//	// if tmin > tmax, ray doesn't intersect AABB
//	if (tmin > tmax)
//	{
//		m_dist = tmax;
//		return false;
//	}
//
//	m_dist = tmin;
//	return true;
//}

//float Physics::GetDist()
//{
//	return m_dist;
//}