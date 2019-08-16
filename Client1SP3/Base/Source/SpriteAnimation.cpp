#include "SpriteAnimation.h"
#include "GL\glew.h"

#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

SpriteAnimation::SpriteAnimation(const std::string &meshName, int row, int col) : Mesh(meshName)
, m_row(row)
, m_col(col)
, m_currentTime(0)
, m_currentFrame(0)
, m_playCount(0)
{
	m_anim = NULL;
}

SpriteAnimation::~SpriteAnimation()
{
	if (m_anim)
		delete m_anim;
}

void SpriteAnimation::Update(double dt)
{
	if (m_anim->animActive == true)
	{
		//Get animation current time, m_currentTime
		//Animation current time increments with dt
		m_currentTime += static_cast<float>(dt);

		//find numFrame
		//GetframeTime
		//FrameTime = animTime / numFrame;
		int numFrame = Math::Max(1, m_anim->endFrame - m_anim->startFrame + 1);
		float frameTime = m_anim->animTime / numFrame;

		//Get Animation current frame, m_currentframe
		m_currentFrame = Math::Min(m_anim->endFrame, m_anim->startFrame + static_cast<int>(m_currentTime / frameTime));

		//Check if m_currentTime is greater than or equal animTime
		if (m_currentTime >= m_anim->animTime)
		{
			//Set end flag, "ended" to true
			m_anim->ended = true;
			
			//If you want to repeat animation, do this
			//Check if repeatCount = 0
			if (m_anim->repeatCount == 0)
			{
				m_anim->animActive = false;
				m_currentTime = 0.f;
				m_currentFrame = m_anim->startFrame;
			}

			//If you want to repeat animation, do this
			//Check if repeatCount == 1
			if (m_anim->repeatCount == 1)
			{
				m_currentTime = 0.f;
				m_currentFrame = m_anim->startFrame;
			}
		}
	}
}

void SpriteAnimation::Render()
{
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);

	glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Position));
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color)));
	if (textureArray[0] > 0)
	{
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(sizeof(Position) + sizeof(Color) + sizeof(Vector3)));
	}


	//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBuffer);

	if (mode == DRAW_LINES)
		glDrawElements(GL_LINES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
	else if (mode == DRAW_TRIANGLE_STRIP)
		glDrawElements(GL_TRIANGLE_STRIP, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));
	else
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)(m_currentFrame * 6 * sizeof(GLuint)));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	if (textureArray[0] > 0)
	{
		glDisableVertexAttribArray(3);
	}
}