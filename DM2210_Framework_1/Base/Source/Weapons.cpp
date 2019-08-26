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
	, activateonce (true)
	, maxswings(5)
{
}

Weapons::~Weapons()
{
}

void Weapons::Init(void)
{
	curr = rightmaxr;
	curt = rightmaxt;
	blockset.Set(-100, -100, -100);
}

void Weapons::UpdateAnimal(const double dt, Vector3 dir, Vector3 origin, std::vector<CAnimal*> animalist, float m_fPlayersDamage)
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
				for (int i = 0; i < animalist.size(); i++)
				{
					if (!animalist[i]->GetActive())
					{
						continue;
					}
					Vector3 temp1;
					Vector3 temp2;
					if (animalist[i]->type == CAnimal::ANIMAL_TYPE::GO_COW)
					{
						temp1.Set(15 * -0.5f, 24 * -0.5f, 30 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1 += animalist[i]->GetPosition();
						temp1.y += 14;
						temp1.z += 3;
						temp2.Set(15 * 0.5f, 24 * 0.5f, 30 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2 += animalist[i]->GetPosition();
						temp2.y += 14;
						temp2.z += 3;
					}
					else if (animalist[i]->type == CAnimal::ANIMAL_TYPE::GO_CHICKEN)
					{
						temp1.Set(10 * -0.5f, 12 * -0.5f, 14 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1 += animalist[i]->GetPosition();
						temp1.y += 8;
						temp1.z += 2;
						temp2.Set(10 * 0.5f, 12 * 0.5f, 14 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2 += animalist[i]->GetPosition();
						temp2.y += 8;
						temp2.z += 2;
					}
					else if (animalist[i]->type == CAnimal::ANIMAL_TYPE::GO_PIG)
					{
						temp1.Set(14 * -0.5f, 22 * -0.5f, 29 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1 += animalist[i]->GetPosition();
						temp1.y += 11;
						temp1.z += 4;
						temp2.Set(14 * 0.5f, 22 * 0.5f, 29 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2 += animalist[i]->GetPosition();
						temp2.y += 11;
						temp2.z += 4;
					}
					if (weaponphysics.RayTraceDist(dir, origin, temp1, temp2))
					{
						/*std::cout << "dank";*/
						if (!rclick)
						{
							animalist[i]->SetBehaviour(4);
							animalist[i]->SetPlayersDamage(m_fPlayersDamage);
						}
						else
						{
							if (feedanimal)
							{
								animalist[i]->SetFed(true);
							}						
						}
					}
				}
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
				
				for (int i = 0; i < animalist.size(); i++)
				{
					if (!animalist[i]->GetActive())
					{
						continue;
					}
					Vector3 temp1;
					Vector3 temp2;
					if (animalist[i]->type == CAnimal::ANIMAL_TYPE::GO_COW)
					{
						temp1.Set(15 * -0.5f, 24 * -0.5f, 30 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1 += animalist[i]->GetPosition();
						temp1.y += 14;
						temp1.z += 3;
						temp2.Set(15 * 0.5f, 24 * 0.5f, 30 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2 += animalist[i]->GetPosition();
						temp2.y += 14;
						temp2.z += 3;
					}
					else if (animalist[i]->type == CAnimal::ANIMAL_TYPE::GO_CHICKEN)
					{
						temp1.Set(10 * -0.5f, 12 * -0.5f, 14 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1 += animalist[i]->GetPosition();
						temp1.y += 8;
						temp1.z += 2;
						temp2.Set(10 * 0.5f, 12 * 0.5f, 14 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2 += animalist[i]->GetPosition();
						temp2.y += 8;
						temp2.z += 2;
					}
					else if (animalist[i]->type == CAnimal::ANIMAL_TYPE::GO_PIG)
					{
						temp1.Set(14 * -0.5f, 22 * -0.5f, 29 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp1.z;
						temp1 += animalist[i]->GetPosition();
						temp1.y += 11;
						temp1.z += 4;
						temp2.Set(14 * 0.5f, 22 * 0.5f, 29 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(animalist[i]->GetAngle())) * temp2.z;
						temp2 += animalist[i]->GetPosition();
						temp2.y += 11;
						temp2.z += 4;
					}
					if (weaponphysics.RayTraceDist(dir, origin, temp1, temp2))
					{
						std::cout << "dank";
						if (!rclick)
						{
							animalist[i]->SetBehaviour(4);
							animalist[i]->SetPlayersDamage(m_fPlayersDamage);
						}
					}
				}
			}
		}
	}
	else if (type == RANGE)
	{
		
	}
}

void Weapons::UpdateEnemy(const double dt, Vector3 dir, Vector3 origin, std::vector<CEnemy*> enemylist, float m_fPlayersDamage)
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
				for (int i = 0; i < enemylist.size(); i++)
				{
					if (!enemylist[i]->GetActive())
					{
						continue;
					}
					Vector3 temp1;
					Vector3 temp2;

					if (enemylist[i]->type == CEnemy::ENEMY_TYPE::GO_ZOMBIE)
					{
						temp1.Set(15 * -0.5f, 24 * -0.5f, 30 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1 += enemylist[i]->GetPosition();
						temp1.y += 14;
						temp1.z += 3;
						temp2.Set(15 * 0.5f, 24 * 0.5f, 30 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2 += enemylist[i]->GetPosition();
						temp2.y += 14;
						temp2.z += 3;
					}
					else if (enemylist[i]->type == CEnemy::ENEMY_TYPE::GO_WITCH)
					{
						temp1.Set(10 * -0.5f, 12 * -0.5f, 14 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1 += enemylist[i]->GetPosition();
						temp1.y += 8;
						temp1.z += 2;
						temp2.Set(10 * 0.5f, 12 * 0.5f, 14 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2 += enemylist[i]->GetPosition();
						temp2.y += 8;
						temp2.z += 2;
					}
					
					if (weaponphysics.RayTraceDist(dir, origin, temp1, temp2))
					{
						/*std::cout << "dank";*/
						if (!rclick)
						{
							enemylist[i]->SetBehaviour(4);
							enemylist[i]->SetPlayersDamage(m_fPlayersDamage);
						}
					}
				}
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

				for (int i = 0; i < enemylist.size(); i++)
				{
					if (!enemylist[i]->GetActive())
					{
						continue;
					}
					Vector3 temp1;
					Vector3 temp2;
					if (enemylist[i]->type == CEnemy::ENEMY_TYPE::GO_ZOMBIE)
					{
						temp1.Set(15 * -0.5f, 24 * -0.5f, 30 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1 += enemylist[i]->GetPosition();
						temp1.y += 14;
						temp1.z += 3;
						temp2.Set(15 * 0.5f, 24 * 0.5f, 30 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2 += enemylist[i]->GetPosition();
						temp2.y += 14;
						temp2.z += 3;
					}
					else if (enemylist[i]->type == CEnemy::ENEMY_TYPE::GO_WITCH)
					{
						temp1.Set(14 * -0.5f, 22 * -0.5f, 29 * -0.5f);
						temp1.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp1.z;
						temp1 += enemylist[i]->GetPosition();
						temp1.y += 11;
						temp1.z += 4;
						temp2.Set(14 * 0.5f, 22 * 0.5f, 29 * 0.5f);
						temp2.x = cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x - sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2.z = sinf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.x + cosf(Math::DegreeToRadian(enemylist[i]->GetAngle())) * temp2.z;
						temp2 += enemylist[i]->GetPosition();
						temp2.y += 11;
						temp2.z += 4;
					}
					if (weaponphysics.RayTraceDist(dir, origin, temp1, temp2))
					{
						std::cout << "dank";
						if (!rclick)
						{
							enemylist[i]->SetBehaviour(4);
							enemylist[i]->SetPlayersDamage(m_fPlayersDamage);
						}
					}
				}
			}
		}
	}
	else if (type == RANGE)
	{

	}
}
void Weapons::UpdateTile(const double dt, Vector3 dir, Vector3 origin, char tilearray[])
{
	Vector3 smallestvertex;
	Vector3 biggestvertex;
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
				}
				Vector3 facingtile;
				facingtile = origin + dir * 100;
				facingtile.x += 50;
				facingtile.z += 50;
				if (facingtile.x > ((int)(origin.x+50)/100)*100 && facingtile.x < ((int)(origin.x+50)/100+1)*100)
				{
					smallestvertex.x = ((int)(origin.x+50) / 100) * 100;
					biggestvertex.x = ((int)(origin.x+50) / 100 + 1) * 100;
					blockset.x = origin.x;
					if (facingtile.z > ((int)(origin.z+50) / 100) * 100 && facingtile.z < ((int)(origin.z+50) / 100 + 1) * 100)
					{
						tiletype = tilearray[0];
						standingonem = true;
						smallestvertex.z = ((int)(origin.z+50) / 100) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100 + 1) * 100;
						blockset.z = origin.z;
					}
					if (facingtile.z < ((int)(origin.z+50)/100)*100)
					{
						tiletype = tilearray[4];
						smallestvertex.z = ((int)(origin.z+50) / 100-1) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100) * 100;
						blockset.z = origin.z - 100;
					}
					if (facingtile.z > ((int)(origin.z+50)/100 + 1)*100)
					{
						tiletype = tilearray[5];
						smallestvertex.z = ((int)(origin.z+50) / 100 + 1) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100 + 2) * 100;
						blockset.z = origin.z + 100;
					}
				}
				else if (facingtile.x < ((int)(origin.x+50)/100)*100)
				{
					smallestvertex.x = ((int)(origin.x+50) / 100 - 1) * 100;
					biggestvertex.x = ((int)(origin.x+50) / 100) * 100;
					blockset.x = origin.x - 100;
					if (facingtile.z < ((int)(origin.z+50) / 100) * 100)
					{
						tiletype = tilearray[1];
						smallestvertex.z = ((int)(origin.z+50) / 100 - 1) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100) * 100;
						blockset.z = origin.z - 100;
					}
					if (facingtile.z > ((int)(origin.z+50) / 100 + 1) * 100)
					{
						tiletype = tilearray[3];
						smallestvertex.z = ((int)(origin.z+50) / 100 + 1) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100 + 2) * 100;
						blockset.z = origin.z + 100;
					}
					if (facingtile.z > ((int)(origin.z+50) / 100) * 100 && facingtile.z < ((int)(origin.z+50) / 100 + 1) * 100)
					{
						tiletype = tilearray[2];
						smallestvertex.z = ((int)(origin.z+50) / 100) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100 + 1) * 100;
						blockset.z = origin.z;
					}
				}
				else if (facingtile.x > ((int)(origin.x+50)/100+1)*100)
				{
					smallestvertex.x = ((int)(origin.x+50) / 100 + 1) * 100;
					biggestvertex.x = ((int)(origin.x+50) / 100 + 2) * 100;
					blockset.x = origin.x + 100;
					if (facingtile.z < ((int)(origin.z+50) / 100) * 100)
					{
						tiletype = tilearray[6];
						smallestvertex.z = ((int)(origin.z+50) / 100 - 1) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100) * 100;
						blockset.z = origin.z - 100;
					}
					if (facingtile.z > ((int)(origin.z+50) / 100 + 1) * 100)
					{
						tiletype = tilearray[8];
						smallestvertex.z = ((int)(origin.z+50) / 100 + 1) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100 + 2) * 100;
						blockset.z = origin.z + 100;
					}
					if (facingtile.z > ((int)(origin.z+50) / 100) * 100 && facingtile.z < ((int)(origin.z+50) / 100 + 1) * 100)
					{
						tiletype = tilearray[7];
						smallestvertex.z = ((int)(origin.z+50) / 100) * 100;
						biggestvertex.z = ((int)(origin.z+50) / 100 + 1) * 100;
						blockset.z = origin.z;
					}
				}
				if (tiletype == 'G')
				{
					smallestvertex.y = -5;
					biggestvertex.y = 0;
				}
				else if (tiletype == 'C')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'W')
				{
					smallestvertex.y = -5;
					biggestvertex.y = 0;
				}
				else if (tiletype == 'B')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'T')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'O')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'C')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'F')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				if (weaponphysics.RayTraceDist(dir, origin, smallestvertex, biggestvertex))
				{
					std::cout << "tile collide";
					if (rclick)
					{
						if (tiletype == 'F')
						{
							furnaceclick = true;
						}
						if (tiletype == 'B')
						{
							berryclick = true;
						}
					}
				}
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
				Vector3 facingtile;
				facingtile = origin + dir * 100;
				facingtile.x += 50;
				facingtile.z += 50;
				if (facingtile.x > ((int)(origin.x + 50) / 100) * 100 && facingtile.x < ((int)(origin.x + 50) / 100 + 1) * 100)
				{
					smallestvertex.x = ((int)(origin.x + 50) / 100) * 100;
					biggestvertex.x = ((int)(origin.x + 50) / 100 + 1) * 100;
					blockset.x = origin.x;
					if (facingtile.z > ((int)(origin.z + 50) / 100) * 100 && facingtile.z < ((int)(origin.z + 50) / 100 + 1) * 100)
					{
						tiletype = tilearray[0];
						standingonem = true;
						smallestvertex.z = ((int)(origin.z + 50) / 100) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100 + 1) * 100;
						blockset.z = origin.z;
					}
					if (facingtile.z < ((int)(origin.z + 50) / 100) * 100)
					{
						tiletype = tilearray[4];
						smallestvertex.z = ((int)(origin.z + 50) / 100 - 1) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100) * 100;
						blockset.z = origin.z - 100;
					}
					if (facingtile.z > ((int)(origin.z + 50) / 100 + 1) * 100)
					{
						tiletype = tilearray[5];
						smallestvertex.z = ((int)(origin.z + 50) / 100 + 1) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100 + 2) * 100;
						blockset.z = origin.z + 100;
					}
				}
				else if (facingtile.x < ((int)(origin.x + 50) / 100) * 100)
				{
					smallestvertex.x = ((int)(origin.x + 50) / 100 - 1) * 100;
					biggestvertex.x = ((int)(origin.x + 50) / 100) * 100;
					blockset.x = origin.x - 100;
					if (facingtile.z < ((int)(origin.z + 50) / 100) * 100)
					{
						tiletype = tilearray[1];
						smallestvertex.z = ((int)(origin.z + 50) / 100 - 1) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100) * 100;
						blockset.z = origin.z - 100;
					}
					if (facingtile.z > ((int)(origin.z + 50) / 100 + 1) * 100)
					{
						tiletype = tilearray[3];
						smallestvertex.z = ((int)(origin.z + 50) / 100 + 1) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100 + 2) * 100;
						blockset.z = origin.z + 100;
					}
					if (facingtile.z > ((int)(origin.z + 50) / 100) * 100 && facingtile.z < ((int)(origin.z + 50) / 100 + 1) * 100)
					{
						tiletype = tilearray[2];
						smallestvertex.z = ((int)(origin.z + 50) / 100) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100 + 1) * 100;
						blockset.z = origin.z;
					}
				}
				else if (facingtile.x > ((int)(origin.x + 50) / 100 + 1) * 100)
				{
					smallestvertex.x = ((int)(origin.x + 50) / 100 + 1) * 100;
					biggestvertex.x = ((int)(origin.x + 50) / 100 + 2) * 100;
					blockset.x = origin.x + 100;
					if (facingtile.z < ((int)(origin.z + 50) / 100) * 100)
					{
						tiletype = tilearray[6];
						smallestvertex.z = ((int)(origin.z + 50) / 100 - 1) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100) * 100;
						blockset.z = origin.z - 100;
					}
					if (facingtile.z > ((int)(origin.z + 50) / 100 + 1) * 100)
					{
						tiletype = tilearray[8];
						smallestvertex.z = ((int)(origin.z + 50) / 100 + 1) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100 + 2) * 100;
						blockset.z = origin.z + 100;
					}
					if (facingtile.z > ((int)(origin.z + 50) / 100) * 100 && facingtile.z < ((int)(origin.z + 50) / 100 + 1) * 100)
					{
						tiletype = tilearray[7];
						smallestvertex.z = ((int)(origin.z + 50) / 100) * 100;
						biggestvertex.z = ((int)(origin.z + 50) / 100 + 1) * 100;
						blockset.z = origin.z;
					}
				}
				if (tiletype == 'G')
				{
					smallestvertex.y = -5;
					biggestvertex.y = 0;
				}
				else if (tiletype == 'C')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'W')
				{
					smallestvertex.y = -5;
					biggestvertex.y = 0;
				}
				else if (tiletype == 'B')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'T')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'O')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'C')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
				}
				else if (tiletype == 'F')
				{
					smallestvertex.y = 0;
					biggestvertex.y = 100;
					/*if (activateonce)
					{
						furnaceclick = true;
						activateonce = false;
					}*/
				}
				if (weaponphysics.RayTraceDist(dir, origin, smallestvertex, biggestvertex))
				{
					std::cout << "tile collide";
				}
			}
		}
	}
	else if (type == RANGE)
	{

	}
}

void Weapons::UpdateFurnace(std::vector<char> FurnaceX, std::vector<char> FurnaceZ)
{
	for (int i = 0; i < FurnaceX.size(); i++)
	{
		if (blockset.x >= (FurnaceX[i]*100-50) && blockset.x <= ((FurnaceX[i]+1)*100-50))
		{
			if (blockset.z >= (FurnaceZ[i] * 100 - 50) && blockset.z <= ((FurnaceZ[i] + 1) * 100 - 50))
			{
				FurnaceID = i;
				return;
			}
		}
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

void Weapons::SetTileType(char type)
{
	tiletype = type;
}

char Weapons::GetTileType()
{
	return tiletype;
}

void Weapons::SetStandinOn(bool stand)
{
	standingonem = stand;
}

bool Weapons::GetStandinOn()
{
	return standingonem;
}

Vector3 Weapons::GetBlockPlacement()
{
	return blockset;
}

void Weapons::SetRClick()
{
	rclick = true;
}

void Weapons::SetLClick()
{
	rclick = false;
}

bool Weapons::GetFurnaceClick()
{
	return furnaceclick;
}

void Weapons::SetFurnaceClick(bool set)
{
	furnaceclick = set;
}

int Weapons::GetFurnaceID()
{
	return FurnaceID;
}

void Weapons::ResetFurnaceID()
{
	FurnaceID = -1;
}

bool Weapons::GetClick()
{
	return rclick;
}

void Weapons::SetFeedAnimal(bool set)
{
	feedanimal = set;
}

int Weapons::GetIntMaxSwings()
{
	return maxswings;
}