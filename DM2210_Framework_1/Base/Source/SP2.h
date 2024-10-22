#ifndef SCENE_SHADOW_H
#define SCNE_SHADOW_H

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
#include "Physics.h"
#include "Season.h"

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
		GEO_MEAT,
		GEO_COOKED_MEAT,

		GEO_WHEAT,
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

		GEO_STONE_SWORD,
		GEO_STONE_PICKAXE,

		GEO_GOLD_SWORD,
		GEO_GOLD_PICKAXE,
		//

		//World
			//Ground textures
			GEO_GRASS,
			GEO_WATER,
			//Ground Objects
			GEO_BERRY,
			GEO_ORE,
			GEO_TREE,
		//
		
		//Animals
		GEO_PIG, 
		GEO_CHICKEN,
		GEO_COW,

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
		GEO_PARTICLE_DEADLEAF,

		GEO_LIGHT_DEPTH_QUAD, //SHADOW
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
	Color fogColor;

	//Ke Wei
		//Animal
	CAnimal* FetchGO();
	void SpawningAnimal();
	void RenderAnimal(CAnimal* animal);
	void AnimalChecker(double dt);
		//Season Changing
	void SeasonChanger(double dt);

	//Shadow
	void RenderPassGPass();
	void RenderPassMain();
	void RenderWorld();
private:
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
	void UpdateParticles(double dt);
	void RenderParticles(ParticleObject * particle);


	ParticleObject* GetParticle(void);
	std::vector<ParticleObject*> particleList;
	std::vector<Furnace*> FurnaceList;
	//Shadow
	unsigned m_gPassShaderID;
	DepthFBO m_lightDepthFBO;
	Mtx44 m_lightDepthProj;
	Mtx44 m_lightDepthView;
	RENDER_PASS m_renderPass;
	
	//Animal
	std::vector<CAnimal *> m_AnimalList;
	int m_NumOfAnimal;
	//Season
	Season SP2_Seasons;
	float m_fWindBounceTime;
	bool m_bTexChange;
};

#endif