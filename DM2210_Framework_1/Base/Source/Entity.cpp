#include "Entity.h"

Entity::Entity(ENTITY_TYPE typeValue)
	: type(typeValue),
	scale(1, 1, 1),
	active(false),
	mass(1.f),
	angle(0.f),
	momentOfInertia(0.0f),
	angularVelocity(0.0f)
{
}

Entity::~Entity()
{
}