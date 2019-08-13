#pragma once

#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadHmap.h"
#include "Particle.h"
#include "DepthFBO.h"

class World
{
public:
	World();
	~World();

	MS modelStack;
	MS viewStack;
	MS projectionStack;
};

