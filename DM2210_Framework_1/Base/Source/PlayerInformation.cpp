#include "PlayerInformation.h"
#include "Application.h"
PlayerInformation::PlayerInformation()
{
}


PlayerInformation::~PlayerInformation()
{
}

void PlayerInformation::AttachCamera(Camera3* _cameraPtr)
{
	attachedCamera = _cameraPtr;
}

void PlayerInformation::Constrain()
{
	//Anchor player to the ground
	Vector3 viewVector = attachedCamera->target - attachedCamera->position;
	attachedCamera->position.y = 40;
	attachedCamera->target = attachedCamera->position + viewVector;
	//
}

void PlayerInformation::update(double dt)
{
	if (Application::IsKeyPressed('W') ||
		Application::IsKeyPressed('A') ||
		Application::IsKeyPressed('S') ||
		Application::IsKeyPressed('D'))
	{
		float m_fSpeed = 100;

		Vector3 viewVector = attachedCamera->target - attachedCamera->position;
		Vector3 rightUV;
		if (Application::IsKeyPressed('W'))
		{
			if ((Application::IsKeyPressed('W')) && (Application::IsKeyPressed(VK_SHIFT)))
				attachedCamera->position += viewVector.Normalized() * m_fSpeed * 2.0f * (float)dt;
			else
			{
				attachedCamera->position = attachedCamera->position + viewVector.Normalized() * m_fSpeed * (float)dt;
			}
		}
		else if (Application::IsKeyPressed('S'))
		{
			attachedCamera->position -= viewVector.Normalized() * m_fSpeed * (float)dt;
		}
		if (Application::IsKeyPressed('A'))
		{
			rightUV = (viewVector.Normalized()).Cross(attachedCamera->up);
			rightUV.y = 0;
			rightUV.Normalize();
			attachedCamera->position -= rightUV * m_fSpeed * (float)dt;
		}
		else if (Application::IsKeyPressed('D'))
		{
			rightUV = (viewVector.Normalized()).Cross(attachedCamera->up);
			rightUV.y = 0;
			rightUV.Normalize();
			attachedCamera->position += rightUV * m_fSpeed * (float)dt;
		}
		// Constrain the position
		Constrain();
		// Update the target
		attachedCamera->target = attachedCamera->position + viewVector;
	}
}