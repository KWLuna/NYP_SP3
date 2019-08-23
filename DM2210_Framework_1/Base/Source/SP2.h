#ifndef SCENE_SHADOW_H
#define SCNE_SHADOW_H

#include "MyMath.h"
#include "Scene.h"
#include "Mtx44.h"
#include "Camera3.h"
#include "Mesh.h"
#include "MatrixStack.h"
#include "Light.h"
#include "LoadHmap.h"
#include "Particle.h"
#include "DepthFBO.h"

#include "PlayerInformation.h"
#include "Furnace.h"

#include "CAnimal.h"
#include "CEnemy.h"
#include "Physics.h"
#include "Season.h"
#include "Crops.h"
#include "Projectile.h"

class SP2 : public Scene
{
	enum UNIFORM_TYPE
	{
		U_MVP = 0,
		U_MODELVIEW,
		U_MODELVIEW_INVERSE_TRANSPOSE,
		U_MATERIAL_AMBIENT,
		U_MATERIAL_DIFFUSE,
		U_MATERIAL_SPECULAR,
		U_MATERIAL_SHININESS,
		U_LIGHTENABLED,
		U_NUMLIGHTS,
		U_LIGHT0_TYPE,
		U_LIGHT0_POSITION,
		U_LIGHT0_COLOR,
		U_LIGHT0_POWER,
		U_LIGHT0_KC,
		U_LIGHT0_KL,
		U_LIGHT0_KQ,
		U_LIGHT0_SPOTDIRECTION,
		U_LIGHT0_COSCUTOFF,
		U_LIGHT0_COSINNER,
		U_LIGHT0_EXPONENT,
		
		//Week 4
		U_COLOR_TEXTURE_ENABLED,
		U_COLOR_TEXTURE_ENABLED1,
		U_COLOR_TEXTURE_ENABLED2,
		U_COLOR_TEXTURE,
		U_COLOR_TEXTURE1,
		U_COLOR_TEXTURE2,
		U_TEXT_ENABLED,
		U_TEXT_COLOR,

		U_TRANSLATE_TEX_COORD,

		//Fog Week 7
		U_FOG_COLOR,
		U_FOG_START,
		U_FOG_END,
		U_FOG_DENSITY,
		U_FOG_TYPE,
		U_FOG_ENABLED,

		//Shadow
		U_LIGHT_DEPTH_MVP_GPASS,
		U_LIGHT_DEPTH_MVP,
		U_SHADOW_MAP,
		U_TOTAL,

		U_SHADOW_COLOR_TEXTURE,
		U_SHADOW_COLOR_TEXTURE1,
		U_SHADOW_COLOR_TEXTURE2,
		U_SHADOW_COLOR_TEXTURE_ENABLED,
		U_SHADOW_COLOR_TEXTURE_ENABLED1,
		U_SHADOW_COLOR_TEXTURE_ENABLED2,

	};
	enum GEOMETRY_TYPE
	{
		GEO_AXES,
		GEO_CROSSHAIR,
		GEO_LIGHTBALL,
		GEO_SPHERE,
		GEO_QUAD,
		GEO_CUBE,
		GEO_RING,
		GEO_CONE,
		GEO_TEXT,

		//Inventory
		GEO_INVENTORY,
		GEO_EMPTY_INVENTORY,
		GEO_HIGHLIGHT_INVENTORY,
		//

		//2D Image Sprites 
		GEO_ITEMS_START,
		
		//Make sure its symetrical with item.h DONT TOUCH IF YOU DONT KNOW WHAT YOURE DOING
		GEO_MEAT,
		GEO_COOKED_MEAT,

		GEO_WHEAT,
		GEO_BREAD,
		GEO_CARROT,
		GEO_SEED,

		GEO_WOOD,
		GEO_STICK,
		GEO_COAL,
		GEO_STONE,
		GEO_GOLD_NUGGET,

		GEO_TORCH,

		GEO_WOODEN_SWORD,
		GEO_WOODEN_PICKAXE,
		GEO_WOODEN_AXE,
		GEO_WOODEN_HOE,

		GEO_STONE_SWORD,
		GEO_STONE_PICKAXE,
		GEO_STONE_AXE,
		GEO_STONE_HOE,

		GEO_GOLD_SWORD,
		GEO_GOLD_PICKAXE,
		GEO_GOLD_AXE,
		GEO_GOLD_HOE,

		GEO_FURNACE,
		//End of symetry.

		//Crop models
		GEO_SPROUT_CROP,
		
		GEO_CARROT_CROP,

		GEO_WHEAT_CROP,
		//

		GEO_WOODEN_SWORD_MODEL,
		GEO_STONE_SWORD_MODEL,

		//
		GEO_LIGHTNING,

		//World
			//Skybox
			GEO_SKYBOX_SPRING,
			GEO_SKYBOX_SUMMER,
			GEO_SKYBOX_WINTER,
			//
			GEO_LIGHT_AFFECTED,
			//Ground textures
			GEO_TILLED_DIRT,
			GEO_GRASS_SPRING,
			GEO_GRASS_SUMMER,
			GEO_GRASS_WINTER,
			GEO_WATER,
			GEO_WATER_WINTER,
			//Dungeon
			GEO_STONE_BRICK_FLOOR,
			//Ground Objects
			GEO_BERRY,
			GEO_GOLD_ORE,
			GEO_COAL_ORE,
			GEO_TREE_SPRING,
			GEO_TREE_SUMMER,
			GEO_TREE_WINTER,
			GEO_WALL,
		//
		GEO_PLAYER,
		//Animals
		GEO_PIG, 
		GEO_CHICKEN,
		GEO_COW,

		//Enemy
		GEO_ZOMBIE,

		//BLOCKS PLACE
		GEO_FURNACE_BLOCK,
		GEO_CHEST_BLOCK,
		GEO_LIGHT_AFFECTED_END,

		//Crafting
		GEO_CRAFTING_MENU,
		GEO_SMELTING_MENU,
		GEO_EMPTY_CRAFTING,

		//TSL
		GEO_SPRITE_ANIMATION,
		//Particles
		GEO_PARTICLE_WATER, 
		GEO_PARTICLE_SNOWFLAKE,
		GEO_PARTICLE_LEAF,
		GEO_PARTICLE_HEART,

		//UI
			//HEALTH
			GEO_HEALTH_FULL,
			GEO_HEALTH_HALF,
			GEO_HEALTH_EMPTY,
			//HUNGER
			GEO_HUNGER_FULL,
			GEO_HUNGER_HALF,
			GEO_HUNGER_EMPTY,
			//Thirst
			GEO_THIRST_FULL,
			GEO_THIRST_EMPTY,
			//Instructions
			GEO_INSTRUCTION1,
		//
		GEO_LIGHT_DEPTH_QUAD, //SHADOW
		GEO_MENU,
		GEO_PLAYER_SELECTION,
		GEO_NAVIGATOR,
		GEO_GAME_OVER,

		NUM_GEOMETRY,
	};

	enum RENDER_PASS
	{
		RENDER_PASS_PRE,
		RENDER_PASS_MAIN,
	};
public:
	SP2();
	~SP2();

	virtual void Init();
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();

	void InitGround();

	void RenderText(Mesh* mesh, std::string text, Color color);
	void RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y);
	void RenderMeshIn2D(Mesh *mesh, bool enableLight, float size = 1.0f, float x = 0.0f, float y = 0.0f);
	void RenderMesh(Mesh *mesh, bool enableLight);
	void RenderAnimation();
	void RenderImageToScreen(Mesh *mesh, bool enableLight, float scaleX, float scaleY, float xPos, float yPos, float zPos);

	void RenderItem(float posX , float posY , float posZ , float scaleX , float scaleY , int slot);
	//Gary
	void RenderGroundObjects();
	void RenderGround();

	void RenderCrafting();
	void RenderFurnace();
	void UpdateWorldVars();
	void RenderCrops();
	void RenderInventory();
	void LoadWorld();

	char GetPlayerCurrentTile(float xPos , float yPos);

	void SaveWorld();
	//
	
	Color fogColor;

	//Ke Wei
		//Animal
	CAnimal* AnimalFetchGO();
	void SpawningAnimal();
	void RenderAnimal(CAnimal* animal);
	void AnimalChecker(double dt);
		//Season Changing
	void SeasonChanger(double dt);
	void RenderSkyBox();
		//HP&Hunger&ThirstUI
	void RenderPlayerInfo();
		//Enemy
	CEnemy* EnemyFetchGO();
	void SpawningEnemy();
	void RenderEnemy(CEnemy* enemy);
	void EnemyChecker(double dt);
		//Projectile
	void UpdateProjectile(double dt);
	void RenderProjectile(ProjectileObject * Projectile);
	ProjectileObject* GetProjectile(void);

	//Instructions
	void RenderInstructions();
	//
	//Yanson
	void Render3DHandHeld();
	//Shadow
	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();

	//Particles
	void UpdateParticles(double dt);
	void RenderParticles(ParticleObject * particle);
	ParticleObject* GetParticle(void);

private:
	double m_dBounceTime;
	unsigned m_vertexArrayID;
	Mesh* meshList[NUM_GEOMETRY];
	unsigned m_programID;
	unsigned m_parameters[U_TOTAL];

	Camera3 camera;

	PlayerInformation * player;

	char world[500][500];

	MS modelStack;
	MS viewStack;
	MS projectionStack;

	Light lights[1];

	bool bLightEnabled;
	float fps;
	float translate_tex_coord;
	int m_particleCount;
	int MAX_PARTICLE;

	//Terrain
	std::vector<unsigned char> m_heightMap;

	//Physics engine
	Physics Phengine;

	//Particles
	std::vector<ParticleObject*> particleList;

	std::vector<Furnace*> FurnaceList;
	std::vector<Crops*> CropList;
	//Shadow
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;
	
	//Enemy
	std::vector<CEnemy *> m_EnemyList;
	int m_NumOfEnemy;
	//Animal
	std::vector<CAnimal *> m_AnimalList;
	int m_NumOfAnimal;
	//Season
	Season* SP2_Seasons;
	float m_fWindBounceTime;
	bool m_bTexChange;
	bool WindAffecting;

	//World.
	float scale ;

	float pX;
	float pZ;

	float outwards;
	bool m_bLightningStrike;
	bool m_bRandTimeTillLightning;

	bool m_bRandLightning;

	float m_fTimeTillLightning;
	float m_fLightningDuration;
	
	int minOutwardsFromPlayerX;
	int minOutwardsFromPlayerZ;
	int maxOutwardsFromPlayerX;
	int maxOutwardsFromPlayerZ;

	float m_fAmbient;
	float m_iDayNight = 1;
	float m_fDayNightDuration = 120;

	float lightningX;
	float lightningZ;

	//instructions
	int instructionorder;
	float instructiontimer;
	//Projectile
	std::vector<ProjectileObject*> ProjectileList;
	int m_iProjectileCount;
	int MAX_PROJECTILE;

	float m_fNavigatorY;

	bool m_bMenu;
	bool m_bContinue;
};

#endif