#include "SP2.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include <iostream>

SP2::SP2()
{
}

SP2::~SP2()
{
}

void SP2::InitGround()
{
	int x = 500, z = 500;
	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < z; ++j)
		{
			float randVal = Math::RandFloatMinMax(0, 10);

			if (randVal < 0.2)
			{
				world[i][j] = 'T'; // Tree generation
			}
			else if (randVal < 0.25)
			{
				world[i][j] = 'G'; // Ore generation
			}
			else if (randVal < 0.30)
			{
				world[i][j] = 'C'; // Ore generation
			}
			else if (randVal < 0.33)
			{
				if (i > 1 && i < 499 && j > 1 && j < 499)
					world[i][j] = 'W'; // Water generation
			}
			else if (randVal < 0.35)
			{
				world[i][j] = 'B'; // Berry generation
			}
			else
				world[i][j] = 'G'; // Grass generation
		}
	}

	for (int i = 0; i < 500; ++i)
	{
		for (int k = 0; k < 500; ++k)
		{
			if (i > 0 && i < 499 && k > 0 && k < 499)
			{
				if (world[i][k] == 'W')
				{
					world[i - 1][k] = 'W';
					world[i][k - 1] = 'W';
				}
			}
		}
	}
}

void SP2::Init()
{
	player = new PlayerInformation;
	
	scale = 100;

	pX = camera.position.x / scale;
	pZ = camera.position.z / scale;

	outwards = 20;

	minOutwardsFromPlayerX = pX - outwards;
	minOutwardsFromPlayerZ = pZ - outwards;

	maxOutwardsFromPlayerX = pX + outwards;
	maxOutwardsFromPlayerZ = pZ + outwards;

	InitGround();

	// Black background
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// Enable depth test
	glEnable(GL_DEPTH_TEST);
	// Accept fragment if it closer to the camera than the former one
	glDepthFunc(GL_LESS);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glGenVertexArrays(1, &m_vertexArrayID);
	glBindVertexArray(m_vertexArrayID);

	m_gPassShaderID = LoadShaders("Shader//GPass.vert", "Shader//GPass.frag");
	m_parameters[U_LIGHT_DEPTH_MVP_GPASS] = glGetUniformLocation(m_gPassShaderID, "lightDepthMVP");
	m_programID = LoadShaders("Shader//Shadow.vert", "Shader//Shadow.frag");
	m_parameters[U_LIGHT_DEPTH_MVP] = glGetUniformLocation(m_programID, "lightDepthMVP");

	m_parameters[U_SHADOW_MAP] = glGetUniformLocation(m_programID, "shadowMap");

	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE] = glGetUniformLocation(m_gPassShaderID, "colorTexture[0]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE1] = glGetUniformLocation(m_gPassShaderID, "colorTexture[1]");
	m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_gPassShaderID, "colorTextureEnabled[2]");
	m_parameters[U_SHADOW_COLOR_TEXTURE2] = glGetUniformLocation(m_gPassShaderID, "colorTexture[2]");

	// Get a handle for our uniform
	m_parameters[U_MVP] = glGetUniformLocation(m_programID, "MVP");
	m_parameters[U_MODELVIEW] = glGetUniformLocation(m_programID, "MV");
	m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE] = glGetUniformLocation(m_programID, "MV_inverse_transpose");
	m_parameters[U_MATERIAL_AMBIENT] = glGetUniformLocation(m_programID, "material.kAmbient");
	m_parameters[U_MATERIAL_DIFFUSE] = glGetUniformLocation(m_programID, "material.kDiffuse");
	m_parameters[U_MATERIAL_SPECULAR] = glGetUniformLocation(m_programID, "material.kSpecular");
	m_parameters[U_MATERIAL_SHININESS] = glGetUniformLocation(m_programID, "material.kShininess");
	m_parameters[U_LIGHTENABLED] = glGetUniformLocation(m_programID, "lightEnabled");
	m_parameters[U_NUMLIGHTS] = glGetUniformLocation(m_programID, "numLights");

	m_parameters[U_LIGHT0_TYPE] = glGetUniformLocation(m_programID, "lights[0].type");
	m_parameters[U_LIGHT0_POSITION] = glGetUniformLocation(m_programID, "lights[0].position_cameraspace");
	m_parameters[U_LIGHT0_COLOR] = glGetUniformLocation(m_programID, "lights[0].color");
	m_parameters[U_LIGHT0_POWER] = glGetUniformLocation(m_programID, "lights[0].power");
	m_parameters[U_LIGHT0_KC] = glGetUniformLocation(m_programID, "lights[0].kC");
	m_parameters[U_LIGHT0_KL] = glGetUniformLocation(m_programID, "lights[0].kL");
	m_parameters[U_LIGHT0_KQ] = glGetUniformLocation(m_programID, "lights[0].kQ");
	m_parameters[U_LIGHT0_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[0].spotDirection");
	m_parameters[U_LIGHT0_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[0].cosCutoff");
	m_parameters[U_LIGHT0_COSINNER] = glGetUniformLocation(m_programID, "lights[0].cosInner");
	m_parameters[U_LIGHT0_EXPONENT] = glGetUniformLocation(m_programID, "lights[0].exponent");

	// Get a handle for our "colorTexture" uniform
	m_parameters[U_COLOR_TEXTURE_ENABLED] = glGetUniformLocation(m_programID, "colorTextureEnabled[0]");
	m_parameters[U_COLOR_TEXTURE] = glGetUniformLocation(m_programID, "colorTexture[0]");
	m_parameters[U_COLOR_TEXTURE_ENABLED1] = glGetUniformLocation(m_programID, "colorTextureEnabled[1]");
	m_parameters[U_COLOR_TEXTURE1] = glGetUniformLocation(m_programID, "colorTexture[1]");
	m_parameters[U_COLOR_TEXTURE_ENABLED2] = glGetUniformLocation(m_programID, "colorTextureEnabled[2]");
	m_parameters[U_COLOR_TEXTURE2] = glGetUniformLocation(m_programID, "colorTexture[2]");
	// Get a handle for our "textColor" uniform
	m_parameters[U_TEXT_ENABLED] = glGetUniformLocation(m_programID, "textEnabled");
	m_parameters[U_TEXT_COLOR] = glGetUniformLocation(m_programID, "textColor");

	m_parameters[U_TRANSLATE_TEX_COORD] = glGetUniformLocation(m_programID, "translate_tex_coord");
	//Week 7 - FOG
	m_parameters[U_FOG_COLOR] = glGetUniformLocation(m_programID, "fogParam.color");
	m_parameters[U_FOG_START] = glGetUniformLocation(m_programID, "fogParam.start");
	m_parameters[U_FOG_END] = glGetUniformLocation(m_programID, "fogParam.end");
	m_parameters[U_FOG_DENSITY] = glGetUniformLocation(m_programID, "fogParam.density");
	m_parameters[U_FOG_TYPE] = glGetUniformLocation(m_programID, "fogParam.type");
	m_parameters[U_FOG_ENABLED] = glGetUniformLocation(m_programID, "fogParam.enabled");

	// Use our shader
	glUseProgram(m_programID);

	lights[0].type = Light::LIGHT_DIRECTIONAL;
	lights[0].position.Set(0, 100, 0);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 1);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	
	glUniform1i(m_parameters[U_LIGHT0_TYPE], lights[0].type);
	glUniform3fv(m_parameters[U_LIGHT0_COLOR], 1, &lights[0].color.r);
	glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
	glUniform1f(m_parameters[U_LIGHT0_KC], lights[0].kC);
	glUniform1f(m_parameters[U_LIGHT0_KL], lights[0].kL);
	glUniform1f(m_parameters[U_LIGHT0_KQ], lights[0].kQ);
	glUniform1f(m_parameters[U_LIGHT0_COSCUTOFF], lights[0].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT0_COSINNER], lights[0].cosInner);
	glUniform1f(m_parameters[U_LIGHT0_EXPONENT], lights[0].exponent);

	m_lightDepthFBO.Init(2048, 2048);

	glUniform2f(m_parameters[U_TRANSLATE_TEX_COORD], 0, 0);

	//Initialise FOG
	fogColor.Set(0.5f, 0.5f, 0.5f);
	glUniform3fv(m_parameters[U_FOG_COLOR], 1, &fogColor.r);
	glUniform1f(m_parameters[U_FOG_START], 1000);
	glUniform1f(m_parameters[U_FOG_END], 2000);
	glUniform1f(m_parameters[U_FOG_DENSITY], 0.0001f);
	glUniform1f(m_parameters[U_FOG_TYPE], 1);
	glUniform1f(m_parameters[U_FOG_ENABLED], 1);

	camera.Init(Vector3(12500, 50, 12500), Vector3(0, 200, -10), Vector3(0, 1, 0));

	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}
	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//calibri.tga");
	meshList[GEO_TEXT]->material.kAmbient.Set(1, 0, 0);
	meshList[GEO_RING] = MeshBuilder::GenerateRing("ring", Color(1, 0, 1), 36, 1, 0.5f);
	meshList[GEO_LIGHTBALL] = MeshBuilder::GenerateSphere("lightball", Color(1, 1, 1), 18, 36, 1.f);
	meshList[GEO_SPHERE] = MeshBuilder::GenerateSphere("sphere", Color(1, 0, 1), 18, 36, 1.f);
	meshList[GEO_CONE] = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);
	meshList[GEO_CONE]->material.kDiffuse.Set(0.99f, 0.99f, 0.99f);
	meshList[GEO_CONE]->material.kSpecular.Set(0.f, 0.f, 0.f);
	meshList[GEO_CUBE] = MeshBuilder::GenerateOBJ("inversecube", "OBJ//skybox.obj");

	meshList[GEO_SKYBOX_SPRING] = MeshBuilder::GenerateOBJ("GEO_SKYBOX_SPRING", "OBJ//skybox.obj");
	meshList[GEO_SKYBOX_SPRING]->textureArray[0] = LoadTGA("Image//Skybox_Spring.tga");

	meshList[GEO_SKYBOX_SUMMER] = MeshBuilder::GenerateOBJ("GEO_SKYBOX_SUMMER", "OBJ//skybox.obj");
	meshList[GEO_SKYBOX_SUMMER]->textureArray[0] = LoadTGA("Image//Skybox_Summer.tga");

	meshList[GEO_SKYBOX_FALL] = MeshBuilder::GenerateOBJ("GEO_SKYBOX_FALL", "OBJ//skybox.obj");
	meshList[GEO_SKYBOX_FALL]->textureArray[0] = LoadTGA("Image//Skybox_Fall.tga");

	meshList[GEO_SKYBOX_WINTER] = MeshBuilder::GenerateOBJ("GEO_SKYBOX_WINTER", "OBJ//skybox.obj");
	meshList[GEO_SKYBOX_WINTER]->textureArray[0] = LoadTGA("Image//Skybox_Winter.tga");

	//
	meshList[GEO_GRASS_SPRING] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_SPRING]->textureArray[0] = LoadTGA("Image//Grass.tga");
	meshList[GEO_GRASS_SPRING]->material.kAmbient.Set(0.3, 0.3, 0.3);

	meshList[GEO_GRASS_SUMMER] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_SUMMER]->textureArray[0] = LoadTGA("Image//Grass_summer.tga");
	
	meshList[GEO_GRASS_FALL] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_FALL]->textureArray[0] = LoadTGA("Image//Grass.tga");
	
	meshList[GEO_GRASS_WINTER] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_WINTER]->textureArray[0] = LoadTGA("Image//Grass_winter.tga");
	

	meshList[GEO_WATER] = MeshBuilder::GenerateQuad("Water", Color(1, 1, 1), 1.f);
	meshList[GEO_WATER]->textureArray[0] = LoadTGA("Image//Water.tga");

	meshList[GEO_WATER_WINTER] = MeshBuilder::GenerateQuad("Water", Color(1, 1, 1), 1.f);
	meshList[GEO_WATER_WINTER]->textureArray[0] = LoadTGA("Image//iced_water.tga");

	meshList[GEO_TREE_SPRING] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_SPRING]->textureArray[0] = LoadTGA("Image//Tree_Spring.tga");

	meshList[GEO_TREE_SUMMER] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_SUMMER]->textureArray[0] = LoadTGA("Image//Tree_Summer.tga");

	meshList[GEO_TREE_FALL] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_FALL]->textureArray[0] = LoadTGA("Image//Tree_Fall.tga");

	meshList[GEO_TREE_WINTER] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_WINTER]->textureArray[0] = LoadTGA("Image//Tree_Winter.tga");

	meshList[GEO_GOLD_ORE] = MeshBuilder::GenerateOBJ("Ore", "OBJ//Ore.obj");
	meshList[GEO_GOLD_ORE]->textureArray[0] = LoadTGA("Image//Gold_Ore.tga");

	meshList[GEO_COAL_ORE] = MeshBuilder::GenerateOBJ("Ore", "OBJ//Ore.obj");
	meshList[GEO_COAL_ORE]->textureArray[0] = LoadTGA("Image//Coal_Ore.tga");
	
	meshList[GEO_BERRY] = MeshBuilder::GenerateOBJ("Berry", "OBJ//Bush.obj");
	meshList[GEO_BERRY]->textureArray[0] = LoadTGA("Image//Bush.tga");

	//Animals
	meshList[GEO_PIG] = MeshBuilder::GenerateOBJ("Pig", "OBJ//Pig.obj");
	meshList[GEO_PIG]->textureArray[0] = LoadTGA("Image//PIG.tga");
	
	meshList[GEO_CHICKEN] = MeshBuilder::GenerateOBJ("Chicken", "OBJ//chicken.obj");
	meshList[GEO_CHICKEN]->textureArray[0] = LoadTGA("Image//chicken.tga");
	
	meshList[GEO_COW] = MeshBuilder::GenerateOBJ("Cow", "OBJ//cow.obj");
	meshList[GEO_COW]->textureArray[0] = LoadTGA("Image//cow.tga");

	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("GEO_INVENTORY", Color(1, 1, 1), 1.0f);
	meshList[GEO_INVENTORY]->textureArray[0] = LoadTGA("Image//Inventory.tga");

	//Inventory meshes
	meshList[GEO_MEAT] = MeshBuilder::GenerateQuad("GEO_MEAT", Color(1, 1, 1), 1.0f);
	meshList[GEO_MEAT]->textureArray[0] = LoadTGA("Image//Meat.tga");

	meshList[GEO_COOKED_MEAT] = MeshBuilder::GenerateQuad("GEO_COOKED_MEAT", Color(1, 1, 1), 1.0f);
	meshList[GEO_COOKED_MEAT]->textureArray[0] = LoadTGA("Image//Cooked_Meat.tga");

	meshList[GEO_WHEAT] = MeshBuilder::GenerateQuad("GEO_WHEAT", Color(1, 1, 1), 1.0f);
	meshList[GEO_WHEAT]->textureArray[0] = LoadTGA("Image//Wheat.tga");

	meshList[GEO_CARROT] = MeshBuilder::GenerateQuad("GEO_CARROT", Color(1, 1, 1), 1.0f);
	meshList[GEO_CARROT]->textureArray[0] = LoadTGA("Image//Carrot.tga");

	meshList[GEO_SEED] = MeshBuilder::GenerateQuad("GEO_SEED", Color(1, 1, 1), 1.0f);
	meshList[GEO_SEED]->textureArray[0] = LoadTGA("Image//Seed.tga");

	meshList[GEO_WOOD] = MeshBuilder::GenerateQuad("GEO_WOOD", Color(1, 1, 1), 1.0f);
	meshList[GEO_WOOD]->textureArray[0] = LoadTGA("Image//Wooden_Log.tga");

	meshList[GEO_STICK] = MeshBuilder::GenerateQuad("GEO_STICK", Color(1, 1, 1), 1.0f);
	meshList[GEO_STICK]->textureArray[0] = LoadTGA("Image//Stick.tga");

	meshList[GEO_COAL] = MeshBuilder::GenerateQuad("GEO_WOOD", Color(1, 1, 1), 1.0f);
	meshList[GEO_COAL]->textureArray[0] = LoadTGA("Image//Coal_Shard.tga");

	meshList[GEO_STONE] = MeshBuilder::GenerateQuad("GEO_STONE", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE]->textureArray[0] = LoadTGA("Image//Stone_Shard.tga");

	meshList[GEO_GOLD_NUGGET] = MeshBuilder::GenerateQuad("GEO_GOLD", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_NUGGET]->textureArray[0] = LoadTGA("Image//Gold_Nugget.tga");
	
	meshList[GEO_GOLD_NUGGET] = MeshBuilder::GenerateQuad("GEO_GOLD", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_NUGGET]->textureArray[0] = LoadTGA("Image//Gold_Nugget.tga");

	meshList[GEO_TORCH] = MeshBuilder::GenerateQuad("GEO_TORCH", Color(1, 1, 1), 1.0f);
	meshList[GEO_TORCH]->textureArray[0] = LoadTGA("Image//Torch.tga");

	meshList[GEO_WOODEN_SWORD] = MeshBuilder::GenerateQuad("GEO_WOODEN_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_WOODEN_SWORD]->textureArray[0] = LoadTGA("Image//Wooden_Sword.tga");

	meshList[GEO_WOODEN_SWORD_MODEL] = MeshBuilder::GenerateOBJ("GEO_WOODEN_SWORD_MODEL", "OBJ//sword.obj");
	meshList[GEO_WOODEN_SWORD_MODEL]->textureArray[0] = LoadTGA("Image//Wooden_Sword.tga");

	meshList[GEO_WOODEN_PICKAXE] = MeshBuilder::GenerateQuad("GEO_WOODEN_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_WOODEN_PICKAXE]->textureArray[0] = LoadTGA("Image//Wooden_Pickaxe.tga");

	meshList[GEO_STONE_SWORD] = MeshBuilder::GenerateQuad("GEO_STONE_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE_SWORD]->textureArray[0] = LoadTGA("Image//Stone_Sword.tga");

	meshList[GEO_STONE_SWORD_MODEL] = MeshBuilder::GenerateOBJ("GEO_STONE_SWORD_MODEL", "OBJ//sword.obj");
	meshList[GEO_STONE_SWORD_MODEL]->textureArray[0] = LoadTGA("Image//Stone_Sword.tga");

	meshList[GEO_STONE_PICKAXE] = MeshBuilder::GenerateQuad("GEO_STONE_PICKAXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE_PICKAXE]->textureArray[0] = LoadTGA("Image//Stone_Pickaxe.tga");

	meshList[GEO_GOLD_SWORD] = MeshBuilder::GenerateQuad("GEO_GOLD_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_SWORD]->textureArray[0] = LoadTGA("Image//Gold_Sword.tga");

	meshList[GEO_GOLD_PICKAXE] = MeshBuilder::GenerateQuad("GEO_GOLD_PICKAXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_PICKAXE]->textureArray[0] = LoadTGA("Image//Gold_Pickaxe.tga");

	meshList[GEO_EMPTY_INVENTORY] = MeshBuilder::GenerateQuad("GEO_EMPTY_INVENTORY", Color(1, 1, 1), 1.0f);
	meshList[GEO_EMPTY_INVENTORY]->textureArray[0] = LoadTGA("Image//Empty_Inventory.tga");

	meshList[GEO_HIGHLIGHT_INVENTORY] = MeshBuilder::GenerateQuad("GEO_HIGHLIGHT_INVENTORY", Color(1, 1, 1), 1.0f);
	meshList[GEO_HIGHLIGHT_INVENTORY]->textureArray[0] = LoadTGA("Image//Highlight_Inventory.tga");

	meshList[GEO_EMPTY_CRAFTING] = MeshBuilder::GenerateQuad("GEO_EMPTY", Color(1, 1, 1), 1.0f);
	meshList[GEO_EMPTY_CRAFTING]->textureArray[0] = LoadTGA("Image//Empty_Crafting.tga");

	meshList[GEO_SMELTING_MENU] = MeshBuilder::GenerateQuad("GEO_SMELTING_MENU", Color(1, 1, 1), 1.0f);
	meshList[GEO_SMELTING_MENU]->textureArray[0] = LoadTGA("Image//Furnace_Menu.tga");

	meshList[GEO_FURNACE] = MeshBuilder::GenerateOBJ("GEO_FURNACE", "OBJ//Ore.obj");
	meshList[GEO_FURNACE]->textureArray[0] = LoadTGA("Image//Furnace.tga");

	meshList[GEO_CRAFTING_MENU] = MeshBuilder::GenerateQuad("GEO_CRAFTING_MENU", Color(1, 1, 1), 1.0f);
	meshList[GEO_CRAFTING_MENU]->textureArray[0] = LoadTGA("Image//Crafting.tga");
	//
	//Crops meshes
	meshList[GEO_CARROT_CROP] = MeshBuilder::GenerateQuad("GEO_CARROT_CROP", Color(1, 1, 1), 1.0f);
	meshList[GEO_CARROT_CROP]->textureArray[0] = LoadTGA("Image//Carrot_Crop.tga");

	meshList[GEO_WHEAT_CROP] = MeshBuilder::GenerateQuad("GEO_WHEAT_CROP", Color(1, 1, 1), 1.0f);
	meshList[GEO_WHEAT_CROP]->textureArray[0] = LoadTGA("Image//Wheat_Crop.tga");

	meshList[GEO_SPROUT_CROP] = MeshBuilder::GenerateQuad("GEO_SPROUT_CROP", Color(1, 1, 1), 1.0f);
	meshList[GEO_SPROUT_CROP]->textureArray[0] = LoadTGA("Image//Sprout_Crop.tga");
	//

	meshList[GEO_CRAFTING_MENU] = MeshBuilder::GenerateQuad("GEO_CRAFTING_MENU", Color(1, 1, 1), 1.0f);
	meshList[GEO_CRAFTING_MENU]->textureArray[0] = LoadTGA("Image//Crafting.tga");
	//Particles
	meshList[GEO_PARTICLE_WATER] = MeshBuilder::GenerateQuad("GEO_PARTICLE_WATER", Color(1, 1, 1), 1.0f);
	meshList[GEO_PARTICLE_WATER]->textureArray[0] = LoadTGA("Image//particle_water.tga");
	meshList[GEO_PARTICLE_SNOWFLAKE] = MeshBuilder::GenerateQuad("GEO_PARTICLE_SNOW", Color(1, 1, 1), 1.0f);
	meshList[GEO_PARTICLE_SNOWFLAKE]->textureArray[0] = LoadTGA("Image//particle_snow.tga");
	meshList[GEO_PARTICLE_LEAF] = MeshBuilder::GenerateQuad("GEO_PARTICLE_LEAF", Color(1, 1, 1), 1.0f);
	meshList[GEO_PARTICLE_LEAF]->textureArray[0] = LoadTGA("Image//particle_leaf.tga");
	meshList[GEO_PARTICLE_DEADLEAF] = MeshBuilder::GenerateQuad("GEO_PARTICLE_DEADLEAF", Color(1, 1, 1), 1.0f);
	meshList[GEO_PARTICLE_DEADLEAF]->textureArray[0] = LoadTGA("Image//particle_deadleaf.tga");

	m_particleCount = 0;
	MAX_PARTICLE = 1000;
	//Sprite animation
	meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("cat", 1, 17);
	meshList[GEO_SPRITE_ANIMATION]->textureArray[0] = LoadTGA("Image//tabby.tga");
	//Shadow
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 17, 1, 1.f, true);
		//Startframe, endframe, repeat, time, active
	}

	// Projection matrix : 45° Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	bLightEnabled = true;
	translate_tex_coord = 0;

	player->AttachCamera(&camera);
	
	//Test item stacking
	player->addItem(new Item(Item::ITEM_WOODEN_SWORD, 1));
	player->addItem(new Item(Item::ITEM_STONE, 1));
	player->addItem(new Item(Item::ITEM_GOLD_NUGGET, 1));
	player->addItem(new Item(Item::ITEM_COAL, 100));
	player->addItem(new Item(Item::ITEM_MEAT, 100));
	player->addItem(new Item(Item::ITEM_CARROT, 10));
	player->addItem(new Item(Item::ITEM_WHEAT, 10));
	player->addItem(new Item(Item::ITEM_SEED, 10));

	//Particle
	for (unsigned int i = 0; i < MAX_PARTICLE; ++i)
	{
		particleList.push_back(new ParticleObject(ParticleObject_TYPE::P_WATER));
	}
	//Animals
	unsigned int NUMBEROFOBJECTS = 100;

	for (unsigned int i = 0; i < NUMBEROFOBJECTS; ++i)
	{
		m_AnimalList.push_back(new CAnimal(CAnimal::GO_PIG));
	}
	m_NumOfAnimal = 0;
	//Season
	SP2_Seasons = new Season;
	SP2_Seasons->setSeason(0);
	WindAffecting = false;
	m_fWindBounceTime = 0;
	m_bTexChange = false;

	//Temp furnace for testing , delete when raytrace is introduced.
	//FurnaceList.push_back(new Furnace);
	//FurnaceList[0]->SetStatus(true);

	CropList.push_back(new Crops(0 , 12500 , 12500));
}

void SP2::RenderFurnace()
{
	for (int i = 0; i < FurnaceList.size(); ++i)
	{
		// Render this furnace UI only.
		if (FurnaceList[i]->GetStatus() == true)
		{
			//Interface
			RenderImageToScreen(meshList[GEO_SMELTING_MENU], false, Application::GetWindowWidth() / 2, Application::GetWindowWidth() / 2,
				Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2, 0);

			//Top
			RenderImageToScreen(meshList[GEO_EMPTY_CRAFTING], false, 175, 175,
				Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2 + 150, 1);

			RenderItem(Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2 + 150
				, 2, 150, 150, FurnaceList[i]->GetSmeltingID());

			//Bottom
			RenderImageToScreen(meshList[GEO_EMPTY_CRAFTING], false, 175, 175,
				Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2 - 150, 1);

			RenderItem(Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2 - 150
				, 2, 150, 150, FurnaceList[i]->GetFuelID());

			//Result
			RenderImageToScreen(meshList[GEO_EMPTY_CRAFTING], false, 175, 175,
				Application::GetWindowWidth() / 2 + 150, Application::GetWindowHeight() / 2, 1);

			RenderItem(Application::GetWindowWidth() / 2 + 150, Application::GetWindowHeight() / 2 
				, 2, 150, 150, FurnaceList[i]->GetResultID());

		/*	RenderItem(Application::GetWindowWidth() / 2 + 150, Application::GetWindowHeight() / 2
				, 2, 150, 150, FurnaceList[i]->GetResultID());*/


			if (FurnaceList[i]->GetSlot() == 1)
				RenderImageToScreen(meshList[GEO_HIGHLIGHT_INVENTORY], false, 175, 175,
					Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2 + 150, 2);
			else if (FurnaceList[i]->GetSlot() == 2)
				RenderImageToScreen(meshList[GEO_HIGHLIGHT_INVENTORY], false, 175, 175,
					Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2 - 150, 2);
			else
				RenderImageToScreen(meshList[GEO_HIGHLIGHT_INVENTORY], false, 175, 175,
					Application::GetWindowWidth() / 2 + 150, Application::GetWindowHeight() / 2, 2);

			std::ostringstream ss;
			ss.precision(5);
			ss << FurnaceList[i]->GetFuelTotal();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 100, -170, -200);

			ss.str("");
			ss << FurnaceList[i]->GetSmeltingTotal();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 100, -170, 100);

			ss.str("");
			ss << FurnaceList[i]->GetResultTotal();
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(1, 0, 0), 100, 130 , -40);

			break;
		}
	}
	
}

void SP2::UpdateWorldVars()
{
	scale = 100;

	//World variables.
	pX = camera.position.x / scale;
	pZ = camera.position.z / scale;

	//Constant , dont need to edit.
	//outwards = 20;

	minOutwardsFromPlayerX = pX - outwards;
	minOutwardsFromPlayerZ = pZ - outwards;

	maxOutwardsFromPlayerX = pX + outwards;
	maxOutwardsFromPlayerZ = pZ + outwards;
	//
}

void SP2::Update(double dt)
{
	if (Application::IsKeyPressed('H') && m_dDoubleBounceTime <= 0)
	{
		camera.up.Set(0, -1, 0);
		m_dDoubleBounceTime = 1;
	}

	std::cout << camera.target << " " << camera.up << std::endl;

	if (Application::IsKeyPressed('1'))
		glEnable(GL_CULL_FACE);
	if (Application::IsKeyPressed('2'))
		glDisable(GL_CULL_FACE);
	if (Application::IsKeyPressed('3'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	if (Application::IsKeyPressed('4'))
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	m_dDoubleBounceTime -= 1 * dt;

	if (Application::IsKeyPressed('F') && m_dDoubleBounceTime <= 0)
	{
		FurnaceList.push_back(new Furnace);
		FurnaceList[0]->SetStatus(true);

		m_dDoubleBounceTime = 0.2;
	}

	UpdateWorldVars();
	UpdateParticles(dt);
	player->update(dt);

	//Update all crops present in the world.
	for (int i = 0; i < CropList.size(); ++i)
		CropList[i]->update(dt);

	//Update all the furnaces present in the level.
	for (int i = 0; i < FurnaceList.size(); ++i)
		FurnaceList[i]->update(dt, player);

	//Rmb to update to pointer.
	SP2_Seasons->Update(dt);
	SeasonChanger(dt);

	//Sprite Animation
	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->Update(dt);
		sa->m_anim->animActive = true;
	}

	camera.Update(dt);
	fps = (float)(1.f / dt);

	AnimalChecker(dt);
}

void SP2::AnimalChecker(double dt)
{

	float MinX = camera.position.x - 1000;
	float MaxX = camera.position.x + 1000;
	float MinZ = camera.position.z - 1000;
	float MaxZ = camera.position.z + 1000;

	if (m_NumOfAnimal < 20)
		SpawningAnimal();

	m_NumOfAnimal = 0;
	for (std::vector<CAnimal *>::iterator it = m_AnimalList.begin(); it != m_AnimalList.end(); ++it)
	{
		CAnimal *go = (CAnimal *)*it;

		if (go->m_bSpawned)
		{
			if (go->pos.x > MinX && go->pos.x < MaxX && go->pos.z > MinZ && go->pos.z < MaxZ)
			{
				go->m_bActive = true;
			}
			else
				go->m_bActive = false;

			if (go->m_bActive)
			{
				go->Update(dt);
				m_NumOfAnimal++;
			}
		}
	}
}

CAnimal* SP2::FetchGO()
{
	for (auto go : m_AnimalList)
	{
		if (!go->m_bSpawned)
		{
			go->m_bSpawned = true;
			return go;
		}
	}
	for (unsigned int i = 0; i < 5; ++i)
	{
		CAnimal *go = new CAnimal(CAnimal::GO_PIG);
		m_AnimalList.push_back(go);
	}

	CAnimal *go = m_AnimalList.back();
	go->m_bSpawned = true;
	return go;
}

void SP2::RenderCrops()
{
	for (int i = 0; i < CropList.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			for (int k = 0; k < 3; ++k)
			{
				if (CropList[i]->GetState() == 0)
				{
					modelStack.PushMatrix();
					modelStack.Translate(CropList[i]->GetXPos() - 25 + j * 25, 25, CropList[i]->GetZPos() - 25 + k * 25);
					modelStack.Scale(25, 50, 25);
					modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (CropList[i]->GetXPos() - 25 + j * 25), camera.position.z - (CropList[i]->GetZPos() - 25 + k * 25))), 0, 1, 0);
					RenderMesh(meshList[GEO_SPROUT_CROP], false);
					modelStack.PopMatrix();
				}
				else
				{
					if (CropList[i]->GetCropType() == 0)
					{
						modelStack.PushMatrix();
						modelStack.Translate(CropList[i]->GetXPos() - 25 + j * 25, 25, CropList[i]->GetZPos() - 25 + k * 25);
						modelStack.Scale(25, 50, 25);
						modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (CropList[i]->GetXPos() - 25 + j * 25), camera.position.z - (CropList[i]->GetZPos() - 25 + k * 25))), 0, 1, 0);
						RenderMesh(meshList[GEO_CARROT_CROP], false);
						modelStack.PopMatrix();
					}
					else
					{
						modelStack.PushMatrix();
						modelStack.Translate(CropList[i]->GetXPos() - 25 + j * 25, 25, CropList[i]->GetZPos() - 25 + k * 25);
						modelStack.Scale(25, 50, 25);
						modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (CropList[i]->GetXPos() - 25 + j * 25), camera.position.z - (CropList[i]->GetZPos() - 25 + k * 25))), 0, 1, 0);
						RenderMesh(meshList[GEO_WHEAT_CROP], false);
						modelStack.PopMatrix();
					}
				}
			}
		}
	}
}

void SP2::SpawningAnimal()
{
	// each tile is a scale of x. load 50 blocks. aka 50 * x outwards.
	for (float i = minOutwardsFromPlayerX; i < maxOutwardsFromPlayerX; ++i)
	{
		if (i >= 0 && i <= 500)
		{
			for (float k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 500)
				{
					int choice = Math::RandIntMinMax(0, 10);

					if (choice == 1) //spawn in if it is 1
					{

						CAnimal *go = FetchGO();

						choice = Math::RandIntMinMax(0, 2);
						switch (choice)
						{
						case 0:
							go->type = CAnimal::GO_PIG;
							break;
						case 1:
							go->type = CAnimal::GO_COW;
							break;
						case 2:
							go->type = CAnimal::GO_CHICKEN;
							break;
						default:
							break;
						}
						go->pos.Set(0 + i * scale, 0, 0 + k * scale);
						go->Targetpos.Set(Math::RandFloatMinMax(go->pos.x - 400.f, go->pos.x + 400.f), 1, Math::RandFloatMinMax(go->pos.z - 400.f, go->pos.z + 400.f));
						go->m_bSpawned = true;
						go->scale.Set(5, 5, 5);
					}
				}
			}
		}
	}
}
void SP2::SeasonChanger(double dt)
{

	if (SP2_Seasons->getTimer() == -1)
	{
		m_bTexChange = false;
	}
	if (m_bTexChange == false)
	{
		if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::SPRING)
		{
			//Fog is thicker, longer
			glUniform1f(m_parameters[U_FOG_DENSITY], 0.0001f);


			//Light should be slightly brighter than fall but darker than summer
			lights[0].power = 1.5f;
			m_bTexChange = true;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::SUMMER)
		{
			glUniform1f(m_parameters[U_FOG_DENSITY], 0.00001f);

			lights[0].power = 2.f;

			m_bTexChange = true;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::FALL)
		{
			glUniform1f(m_parameters[U_FOG_DENSITY], 0.00001f);

			lights[0].power = 1.f;

			m_bTexChange = true;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::WINTER)
		{
			glUniform1f(m_parameters[U_FOG_DENSITY], 0.0001f);

			lights[0].power = 0.5f;

			m_bTexChange = true;
		}
	}
}
void SP2::UpdateParticles(double dt)
{
	Vector3 m_wind;

	m_fWindBounceTime += 2 * dt;
	if (m_fWindBounceTime > 10.f)
	{
		if (WindAffecting == false)
		{
			WindAffecting = true;
			m_wind.Set(Math::RandFloatMinMax(1.f, 4.f), 1.f, Math::RandFloatMinMax(1.f, 4.f));
		}
		else
		{
			WindAffecting = false;
			m_wind.Set(1.f, 1.f, 1.f);
		}
		m_fWindBounceTime = 0;
	}

	if (m_particleCount < MAX_PARTICLE)
	{
		if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::SPRING)
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_WATER;
			particle->scale.Set(10, 10, 10);
			particle->vel.Set(1, 1, 1);
			particle->m_gravity.Set(0, -9.8f, 0);
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(camera.position.x - 1000, camera.position.x + 1700), 1200.f, Math::RandFloatMinMax(camera.position.z - 1700, camera.position.z + 1700));
			particle->wind = m_wind;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::SUMMER)
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_DEADLEAF;
			particle->scale.Set(10, 10, 10);
			particle->vel.Set(1, 1, 1);
			particle->m_gravity.Set(0, -9.8f, 0);
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(camera.position.x - 1000, camera.position.x + 1700), 600.f, Math::RandFloatMinMax(camera.position.z - 1700, camera.position.z + 1700));
			particle->wind = m_wind;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::FALL)
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_LEAF;
			particle->scale.Set(10, 10, 10);
			particle->vel.Set(1, 1, 1);
			particle->m_gravity.Set(0, -9.8f, 0);
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(camera.position.x - 1000, camera.position.x + 1700), 600.f, Math::RandFloatMinMax(camera.position.z - 1700, camera.position.z + 1700));
			particle->wind = m_wind;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::WINTER)
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_SNOWFLAKE;
			particle->scale.Set(10, 10, 10);
			particle->vel.Set(1, 1, 1);
			particle->m_gravity.Set(0, -9.8f, 0);
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(camera.position.x - 1000, camera.position.x + 1700), 1200.f, Math::RandFloatMinMax(camera.position.z - 1700, camera.position.z + 1700));
			particle->wind = m_wind;
		}
	}

	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (particle->active)
		{
			particle->Update(dt);
			if (particle->pos.y < (ReadHeightMap(m_heightMap, particle->pos.x / 4000.f, particle->pos.z / 4000.f) * 350.f))
			{
				particle->active = false;
				m_particleCount--;
			}
		}
	}

}

ParticleObject* SP2::GetParticle(void)
{
	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (!particle->active)
		{
			particle->active = true;
			m_particleCount++;
			return particle;
		}
	}
	for (unsigned i = 0; i < MAX_PARTICLE; ++i)
	{
		ParticleObject *particle = new ParticleObject(ParticleObject_TYPE::P_WATER);
		particleList.push_back(particle);
	}

	ParticleObject *particle = particleList.back();
	particle->active = true;
	m_particleCount++;
	return particle;
}

void SP2::RenderAnimal(CAnimal* animal)
{
	switch (animal->type)
	{
	case CAnimal::GO_PIG:
		modelStack.PushMatrix();
		modelStack.Translate(animal->pos.x, animal->pos.y, animal->pos.z);
		modelStack.Rotate(animal->m_fAngle, 0, 1, 0);
		modelStack.Scale(animal->scale.x, animal->scale.y, animal->scale.z);
		RenderMesh(meshList[GEO_PIG], false);
		modelStack.PopMatrix();
		break;
	case CAnimal::GO_COW:
		modelStack.PushMatrix();
		modelStack.Translate(animal->pos.x, animal->pos.y, animal->pos.z );
		modelStack.Rotate(animal->m_fAngle, 0, 1, 0);
		modelStack.Scale(animal->scale.x, animal->scale.y, animal->scale.z);
		RenderMesh(meshList[GEO_COW], false);
		modelStack.PopMatrix();
		break;
	case CAnimal::GO_CHICKEN:
		modelStack.PushMatrix();
		modelStack.Translate(animal->pos.x, animal->pos.y, animal->pos.z);
		modelStack.Rotate(animal->m_fAngle, 0, 1, 0);
		modelStack.Scale(animal->scale.x, animal->scale.y, animal->scale.z);
		RenderMesh(meshList[GEO_CHICKEN], false);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}
void SP2::RenderParticles(ParticleObject * particle)
{
	switch (particle->type)
	{
	case ParticleObject_TYPE::P_WATER:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_WATER], false);
		modelStack.PopMatrix();
		break;
	case ParticleObject_TYPE::P_SNOWFLAKE:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_SNOWFLAKE], false);
		modelStack.PopMatrix();
		break;
	case ParticleObject_TYPE::P_LEAF:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_LEAF], false);
		modelStack.PopMatrix();
		break;
	case ParticleObject_TYPE::P_DEADLEAF:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_DEADLEAF], false);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}
void SP2::RenderText(Mesh* mesh, std::string text, Color color)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);

	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * .5f, 0, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);

	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderTextOnScreen(Mesh* mesh, std::string text, Color color, float size, float x, float y)
{
	if (!mesh || mesh->textureArray[0] <= 0)
		return;

	glDisable(GL_DEPTH_TEST);
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();

	modelStack.LoadIdentity();
	modelStack.Translate(Application::GetWindowWidth() / 2 + x ,Application::GetWindowHeight() / 2 + y, 0);
	modelStack.Scale(size, size, 1);
	
	glUniform1i(m_parameters[U_TEXT_ENABLED], 1);
	glUniform3fv(m_parameters[U_TEXT_COLOR], 1, &color.r);
	glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
	glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	for (unsigned i = 0; i < text.length(); ++i)
	{
		Mtx44 characterSpacing;
		characterSpacing.SetToTranslation(i * 0.5f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
		Mtx44 MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top() * characterSpacing;
		glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

		mesh->Render((unsigned)text[i] * 6, 6);
	}
	glBindTexture(GL_TEXTURE_2D, 0);
	glUniform1i(m_parameters[U_TEXT_ENABLED], 0);
	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
	glEnable(GL_DEPTH_TEST);
}

void SP2::RenderMeshIn2D(Mesh *mesh, bool enableLight, float size, float x, float y)
{
	Mtx44 ortho;
	ortho.SetToOrtho(-80, 80, -60, 60, -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();
	modelStack.Scale(size, size, size);
	modelStack.Translate(x, y, 0);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureArray[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureArray[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SP2::RenderMesh(Mesh *mesh, bool enableLight)
{
	Mtx44 MVP, modelView, modelView_inverse_transpose;

	if (m_renderPass == RENDER_PASS_PRE)
	{
		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP_GPASS], 1, GL_FALSE, &lightDepthMVP.a[0]);

		for (int i = 0; i < MAX_TEXTURES; ++i)
		{
			if (mesh->textureArray[i] > 0)
			{
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 1);
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE + i], i);
			}
			else
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED + i], 0);

		}
		mesh->Render();
		return;
	}

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);

	modelView = viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);

	if (enableLight && bLightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);

		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);

		Mtx44 lightDepthMVP = m_lightDepthProj * m_lightDepthView * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_LIGHT_DEPTH_MVP], 1, GL_FALSE, &lightDepthMVP.a[0]);

		// Load material
		glUniform3fv(m_parameters[U_MATERIAL_AMBIENT], 1, &mesh->material.kAmbient.r);
		glUniform3fv(m_parameters[U_MATERIAL_DIFFUSE], 1, &mesh->material.kDiffuse.r);
		glUniform3fv(m_parameters[U_MATERIAL_SPECULAR], 1, &mesh->material.kSpecular.r);
		glUniform1f(m_parameters[U_MATERIAL_SHININESS], mesh->material.kShininess);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 1);
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_2D, mesh->textureArray[i]);
			glUniform1i(m_parameters[U_COLOR_TEXTURE + i], i);
		}
		else
		{
			glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED + i], 0);
		}
	}
	mesh->Render();
	for (int i = 0; i < MAX_TEXTURES; ++i)
	{
		if (mesh->textureArray[i] > 0)
			glBindTexture(GL_TEXTURE_2D, 0);
	}
}

void SP2::RenderImageToScreen(Mesh *mesh, bool enableLight, float scaleX, float scaleY, float xPos, float yPos, float zPos)
{
	Mtx44 ortho;
	ortho.SetToOrtho(0, Application::GetWindowWidth(), 0, Application::GetWindowHeight(), -10, 10);
	projectionStack.PushMatrix();
	projectionStack.LoadMatrix(ortho);
	viewStack.PushMatrix();
	viewStack.LoadIdentity();
	modelStack.PushMatrix();
	modelStack.LoadIdentity();

	modelStack.Translate(xPos, yPos, zPos);
	modelStack.Scale(scaleX, scaleY, 1);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();

	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (mesh->textureArray[0] > 0)
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 1);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, mesh->textureArray[0]);
		glUniform1i(m_parameters[U_COLOR_TEXTURE], 0);
	}
	else
	{
		glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
	}
	mesh->Render();
	if (mesh->textureArray[0] > 0)
	{
		glBindTexture(GL_TEXTURE_2D, 0);
	}

	modelStack.PopMatrix();
	glUniform1f(m_parameters[U_FOG_ENABLED], 1);
	viewStack.PopMatrix();
	projectionStack.PopMatrix();
}

void SP2::RenderAnimation()
{
	modelStack.PushMatrix();
	modelStack.Translate(450, 200, -25);
	modelStack.Scale(100, 100, 100);
	modelStack.Rotate(-90, 0, 1, 0);
	RenderMesh(meshList[GEO_SPRITE_ANIMATION], false);
	modelStack.PopMatrix();

}
void SP2::RenderPassGPass()
{
	m_renderPass = RENDER_PASS_PRE;

	m_lightDepthFBO.BindForWriting();

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_gPassShaderID);

	//These matrics should change when light position or direction changes
	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
		m_lightDepthProj.SetToOrtho(-200, 200, -100, 100, -100, 200);
	else
		m_lightDepthProj.SetToPerspective(90, 1.f, 0.1, 200);

	m_lightDepthView.SetToLookAt(lights[0].position.x, lights[0].position.y, lights[0].position.z, 0, 0, 0, 0, 1, 0);

	RenderWorld();
}

void SP2::RenderGroundObjects()
{
	// each tile is a scale of x. load 50 blocks. aka 50 * x outwards.
	for (int i = minOutwardsFromPlayerX; i < maxOutwardsFromPlayerX; ++i)
	{
		if (i >= 0 && i <= 500)
		{
			for (int k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 500)
				{
					switch (world[i][k])
					{
					case 'T':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						
						switch (SP2_Seasons->getSeason())
						{
						case Season::TYPE_SEASON::SPRING:
							RenderMesh(meshList[GEO_TREE_SPRING], true);
							break;
						case Season::TYPE_SEASON::SUMMER:
							RenderMesh(meshList[GEO_TREE_SUMMER], true);
							break;
						case Season::TYPE_SEASON::FALL:
							RenderMesh(meshList[GEO_TREE_FALL], true);
							break;
						case Season::TYPE_SEASON::WINTER:
							RenderMesh(meshList[GEO_TREE_WINTER], true);
							break;
						default:
							break;
						}
						modelStack.PopMatrix();
						break;
					case 'O':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_GOLD_ORE], true);
						modelStack.PopMatrix();
						break;
					case 'C':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_COAL_ORE], true);
						modelStack.PopMatrix();
						break;
					case 'B':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_BERRY], true);
						modelStack.PopMatrix();
					default:
						break;
					}
				}
			}
		}
	}
}
 
void SP2::RenderGround()
{
	// each tile is a scale of x. load 50 blocks. aka 50 * x outwards.
	for (int i = minOutwardsFromPlayerX; i < maxOutwardsFromPlayerX; ++i)
	{
		if (i >= 0 && i <= 500)
		{
			for (int k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 500)
				{
					switch (world[i][k])
					{
					case 'W':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::WINTER)
							RenderMesh(meshList[GEO_WATER_WINTER], true);
						else
						RenderMesh(meshList[GEO_WATER], true);
						modelStack.PopMatrix();
						break;
					default:
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						switch (SP2_Seasons->getSeason())
						{
						case Season::TYPE_SEASON::SPRING:
							RenderMesh(meshList[GEO_GRASS_SPRING], true);
							break;
						case Season::TYPE_SEASON::SUMMER:
							RenderMesh(meshList[GEO_GRASS_SUMMER], true);
							break;
						case Season::TYPE_SEASON::FALL:
							RenderMesh(meshList[GEO_GRASS_FALL], true);
							break;
						case Season::TYPE_SEASON::WINTER:
							RenderMesh(meshList[GEO_GRASS_WINTER], true);
							break;
						default:
							break;
						}
						modelStack.PopMatrix();
						break;
					}
					
				}
			}
		}
	}
}

void SP2::RenderItem(float posX, float posY , float posZ , float scaleX, float scaleY , int ID)
{
	if (ID != 0)
		RenderImageToScreen(meshList[GEO_ITEMS_START + ID], false, scaleX, scaleY, posX, posY, posZ);
}

void SP2::RenderCrafting()
{
	//Crafting Interface
	if (player->getIsCrafting() == true)
	{
		
		RenderImageToScreen(meshList[GEO_CRAFTING_MENU], false, Application::GetWindowWidth() / 2, Application::GetWindowWidth() / 2,
			Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2, 0);

		//Left Slot
		RenderImageToScreen(meshList[GEO_EMPTY_CRAFTING], false, 175, 175,
			Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2, 1);

		if (player->getCraftingSlotOne() != -1)
			//Render the id of whatever belongs to the first crafting slot 
			RenderItem(Application::GetWindowWidth() / 2 - 150, Application::GetWindowHeight() / 2
				, 2, 150, 150, player->getItem(player->getCraftingSlotOne())->getID());
		//

		// Right slot
		RenderImageToScreen(meshList[GEO_EMPTY_CRAFTING], false, 175, 175,
			Application::GetWindowWidth() / 2 + 150, Application::GetWindowHeight() / 2, 1);

		if (player->getCraftingSlotTwo() != -1)
			//Render the id of whatever belongs to the second crafting slot 
			RenderItem(Application::GetWindowWidth() / 2 + 150, Application::GetWindowHeight() / 2
				, 2, 150, 150, player->getItem(player->getCraftingSlotTwo())->getID());
		//

		//Bottom Crafting
		RenderImageToScreen(meshList[GEO_EMPTY_CRAFTING], false, 175, 175,
			Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2 - 200, 1);

		if (player->getCraftingSlotTwo() != -1)
			RenderItem(Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2 - 200
				, 2, 150, 150, player->craft(player->getCraftingSlotOne(), player->getCraftingSlotTwo())->getID());
		//+
	}
}

void SP2::RenderWorld()
{

	//testing 3d item
	if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_WOODEN_SWORD)
	{
		Vector3 weaponoffset(1, 1, 2);
		Vector3 viewvector = (camera.target - camera.position);
		modelStack.PushMatrix();
		modelStack.Translate(camera.position.x + viewvector.x, camera.position.y + viewvector.y, camera.position.z + viewvector.z);
			modelStack.PushMatrix();
			// Rotate to correspond to the camera
			modelStack.Rotate(Math::RadianToDegree(atan2(-viewvector.x, -viewvector.z)), 0, 1, 0);
			modelStack.Rotate(Math::RadianToDegree(atan2(viewvector.y, camera.up.y)), 1, 0, 0);
			if (player->getcurtool()->GetCurSwing())
			{
				modelStack.Translate(player->getcurtool()->GetCurT(), -0.3, -0.8); // Offset to draw the object
				modelStack.Rotate(player->getcurtool()->GetCurR(), 0, 1, 0);
			}
			else if (player->getcurtool()->GetSide())
			{
				modelStack.Translate(player->getcurtool()->GetLeftRestT(), -0.2, -0.4);
				modelStack.Rotate(player->getcurtool()->GetLeftRestR(), 0, 1, 0);
			}
			else
			{
				modelStack.Translate(player->getcurtool()->GetRightRestT(), -0.2, -0.4);
				modelStack.Rotate(player->getcurtool()->GetRightRestR(), 0, 1, 0);
			}
			if (player->getcurtool()->GetCurSwing())
			{
				modelStack.Rotate(player->getcurtool()->GetAttackUpTilt(), 1, 0, 0);
			}
			else
			{
				modelStack.Rotate(player->getcurtool()->GetRestUpTilt(), 1, 0, 0);
			}
		
			modelStack.Scale(2, 2, 2);
			RenderMesh(meshList[GEO_STONE_SWORD_MODEL], false);
			modelStack.PopMatrix();
		modelStack.PopMatrix();
		//
	}

	RenderImageToScreen(meshList[GEO_INVENTORY], false, Application::GetWindowWidth(), Application::GetWindowHeight() / 10,
		Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2 - 360, 0);

	for (int i = 0; i < player->getTotalItems(); ++i)
	{
		//Frames for inventory
		if (i != player->getCurrentSlot())
			RenderImageToScreen(meshList[GEO_EMPTY_INVENTORY], false, 60, 60,
				180 + 60 + i * 60, Application::GetWindowHeight() / 2 - 360, 1);
		else
			RenderImageToScreen(meshList[GEO_HIGHLIGHT_INVENTORY], false, 60, 60,
				180 + 60 + i * 60, Application::GetWindowHeight() / 2 - 360, 1);

		RenderItem(180 + 60 + i * 60, Application::GetWindowHeight() / 2 - 360, 2 , 50, 50, player->getItem(i)->getID());
	}

	RenderGroundObjects();

	//Render Animals
	for (std::vector<CAnimal *>::iterator it = m_AnimalList.begin(); it != m_AnimalList.end(); ++it)
	{
		CAnimal *animal = (CAnimal *)*it;
		if (animal->m_bActive)
		{
			RenderAnimal(animal);
		}
	}


}

void SP2::RenderPassMain()
{
	m_renderPass = RENDER_PASS_MAIN;
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, Application::GetWindowWidth(), Application::GetWindowHeight());

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glUseProgram(m_programID);

	//pass light depth texture
	m_lightDepthFBO.BindForReading(GL_TEXTURE8);
	glUniform1i(m_parameters[U_SHADOW_MAP], 8);

	// Camera matrix
	viewStack.LoadIdentity();
	viewStack.LookAt(
		camera.position.x, camera.position.y, camera.position.z,
		camera.target.x, camera.target.y, camera.target.z,
		camera.up.x, camera.up.y, camera.up.z
	);
	modelStack.LoadIdentity();

	if (lights[0].type == Light::LIGHT_DIRECTIONAL)
	{
		Vector3 lightDir(lights[0].position.x, lights[0].position.y, lights[0].position.z);
		Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightDirection_cameraspace.x);
	}
	else if (lights[0].type == Light::LIGHT_SPOT)
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
		Vector3 spotDirection_cameraspace = viewStack.Top() * lights[0].spotDirection;
		glUniform3fv(m_parameters[U_LIGHT0_SPOTDIRECTION], 1, &spotDirection_cameraspace.x);
	}
	else
	{
		Position lightPosition_cameraspace = viewStack.Top() * lights[0].position;
		glUniform3fv(m_parameters[U_LIGHT0_POSITION], 1, &lightPosition_cameraspace.x);
	}

	RenderMesh(meshList[GEO_AXES], false);
	RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 5.0f);

	RenderWorld();


	//render light ball
	modelStack.PushMatrix();
	modelStack.Translate(lights[0].position.x, lights[0].position.y, lights[0].position.z);
	RenderMesh(meshList[GEO_LIGHTBALL], false);
	modelStack.PopMatrix();

	//render GEO_LIGHT_DEPTH_QUAD
	modelStack.PushMatrix();
	modelStack.Translate(0, 500, 0);
	//modelStack.Scale(100, 100, 100);
	RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false);
	modelStack.PopMatrix();

	RenderGround();
	RenderAnimation();

	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Scale(3500, 3500, 3500);

	switch (SP2_Seasons->getSeason())
	{
	case Season::TYPE_SEASON::SPRING:
		RenderMesh(meshList[GEO_SKYBOX_SPRING], false);
		break;
	case Season::TYPE_SEASON::SUMMER:
		RenderMesh(meshList[GEO_SKYBOX_SUMMER], false);
		break;
	case Season::TYPE_SEASON::FALL:
		RenderMesh(meshList[GEO_SKYBOX_FALL], false);
		break;
	case Season::TYPE_SEASON::WINTER:
		RenderMesh(meshList[GEO_SKYBOX_WINTER], false);
		break;
	default:
		break;
	}
	modelStack.PopMatrix();

	//	Render Particles
	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (particle->active)
		{
			RenderParticles(particle);
		}
	}

	RenderCrafting();
	RenderFurnace();
	RenderCrops();

	std::ostringstream ss;
	ss.precision(5);
	ss << "FPS: " << fps;
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 100, -600 , 300  );

	ss.str("");
	ss << std::to_string(player->getItem(player->getCurrentSlot())->getQuantity());
	RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 100, -20, -300);
}
void SP2::Render()
{
	RenderPassGPass();
	RenderPassMain();
}

void SP2::Exit()
{
	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_gPassShaderID);
}
