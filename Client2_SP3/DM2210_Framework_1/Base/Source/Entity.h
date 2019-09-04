#ifndef ENTITY_H
#define ENTITY_H

#include "Vector3.h"

struct Entity
{
	enum ENTITY_TYPE
	{
		GO_NONE = 0,
		GO_BALL,
		GO_CUBE,

		GO_ENEMY,
		GO_ENEMY_BULLET,

		GO_WALL,
		GO_PILLAR,
		GO_PADDLE,
		GO_PUSHER,

		GO_TOTAL, //must be last
	};
	ENTITY_TYPE type;
	Vector3 pos;
	Vector3 vel;
	Vector3 scale;
	Vector3 dir; //direction/orientation
	Vector3 normal;
	float momentOfInertia;
	float angularVelocity; //in radians
	bool active;
	float mass;
	float angle;

	Entity(ENTITY_TYPE typeValue = GO_CUBE);
	~Entity();
};

#endif