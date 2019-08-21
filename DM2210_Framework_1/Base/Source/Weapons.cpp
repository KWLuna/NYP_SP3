#include "Weapons.h"

Weapons::Weapons()
	:m_range(3.0f)
	, rightmaxr(-15.5)
	, rightmaxt(0.2)
	, leftmaxr(15.5)
	, leftmaxt(-0.2)
	, attacktiltup(20)
	, swingtime(1.0)
	, curswing(false)
	, side(false)
	, rightrestr(-15.5)
	, rightrestt(0.2)
	, leftrestr(15.5)
	, leftrestt(-0.2)
	, resttiltup(30.0)
{
}

Weapons::~Weapons()
{
}

void Weapons::Init(void)
{
	curr = rightmaxr;
	curt = rightmaxt;
}

void Weapons::Update(const double dt, Vector3 dir, Vector3 origin)
{
	if (type == MELEE)
	{
		if (curswing)
		{
			if (side)
			{
				curt += ((leftmaxt - rightmaxt) / swingtime * dt);
				curr += ((leftmaxr - rightmaxr) / swingtime * dt);
				if (curt <= leftmaxt)
				{
					curt = leftmaxt;
				}
				if (curr >= leftmaxr)
				{
					curr = leftmaxr;
					curswing = false;
				}
				weaponphysics.RayTraceDist(dir, origin, Vector3(1,1,1), Vector3(1,1,1));
			}
			else
			{
				curt += ((rightmaxt - leftmaxt) / swingtime * dt);
				curr += ((rightmaxr - leftmaxr) / swingtime * dt);
				if (curt >= rightmaxt)
				{
					curt = rightmaxt;
				}
				if (curr <= rightmaxr)
				{
					curr = rightmaxr;
					curswing = false;
				}
				weaponphysics.RayTraceDist(dir, origin, Vector3(1, 1, 1), Vector3(1, 1, 1));
			}
		}
	}
	else if (type == RANGE)
	{

	}
}

void Weapons::SetPosition(Vector3 vposition)
{
	this->vPosition = vposition;
}

Vector3 Weapons::GetPosition(void) const
{
	return vPosition;
}

void Weapons::SetRotation(const float fRotation)
{
	this->fRotation = fRotation;
}


float Weapons::GetRotation(void) const
{
	return fRotation;
}

void Weapons::SetScale(Vector3 vScale)
{
	this->vScale = vScale;
}

Vector3 Weapons::GetScale(void) const
{
	return vScale;
}

void Weapons::Render(void)
{
}

float Weapons::GetLeftMaxR()
{
	return leftmaxr;
}

float Weapons::GetLeftMaxT()
{
	return leftmaxt;
}

float Weapons::GetRightMaxR()
{
	return rightmaxr;
}

float Weapons::GetRightMaxT()
{
	return rightmaxt;
}

float Weapons::GetAttackUpTilt()
{
	return attacktiltup;
}

float Weapons::GetLeftRestR()
{
	return leftrestr;
}

float Weapons::GetLeftRestT()
{
	return leftrestt;
}

float Weapons::GetRightRestR()
{
	return rightrestr;
}

float Weapons::GetRightRestT()
{
	return rightrestt;
}

float Weapons::GetRestUpTilt()
{
	return resttiltup;
}

void Weapons::SetCurSwing()
{
	if (!curswing)
	{
		curswing = true;
		if (side == true)
		{
			side = false;
		}
		else
			side = true;
	}
}

float Weapons::GetCurR()
{
	return curr;
}

float Weapons::GetCurT()
{
	return curt;
}

bool Weapons::GetCurSwing()
{
	return curswing;
}

bool Weapons::GetSide()
{
	return side;
}