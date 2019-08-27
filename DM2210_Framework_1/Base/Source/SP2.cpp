#include "SP2.h"
#include "GL\glew.h"
#include "shader.hpp"
#include "MeshBuilder.h"
#include "Application.h"
#include "Utility.h"
#include "LoadTGA.h"
#include <sstream>

#include <string>
#include <fstream>
#include <iostream>
#include "SoundEngine.h"

SP2::SP2()
{
}

SP2::~SP2()
{
}

void SP2::SaveWorld()
{
	//By default , when open you clear it.
	std::ofstream worldFile("WorldSaveFile.txt");
	if (worldFile.is_open())
	{
		for (int i = 0; i < 250; ++i)
		{
			std::string tmp;
			for (int j = 0; j < 250; ++j)
			{
				tmp.push_back(world[i][j]);
			}
			worldFile << tmp << std::endl;
		}

		worldFile.close();
	}
	else
		std::cout << " cant save !" << std::endl;

	std::ofstream cropFile("CropSaveFile.txt");
	if (cropFile.is_open())
	{
		std::cout << "ppppp " << CropList.size() << std::endl;
		for (int i = 0; i < CropList.size(); ++i)
		{
			cropFile << CropList[i]->GetXTile() << std::endl;
			cropFile << CropList[i]->GetZTile() << std::endl;
			cropFile << CropList[i]->GetCropType() << std::endl;
			cropFile << CropList[i]->getTimeSincePlanted() << std::endl;
		}
		cropFile.close();
	}
	else
		std::cout << " cant save !" << std::endl;

	std::ofstream frunaceFile("FurnaceSaveFile.txt");
	if (frunaceFile.is_open())
	{
		for (int i = 0; i < FurnaceList.size(); ++i)
		{
			frunaceFile << FurnaceList[i]->GetXTile() << std::endl;
			frunaceFile << FurnaceList[i]->GetZTile() << std::endl;
			
			frunaceFile << FurnaceList[i]->GetFuelID() << std::endl;
			frunaceFile << FurnaceList[i]->GetFuelTotal() << std::endl;
			
			frunaceFile << FurnaceList[i]->GetSmeltingID() << std::endl;
			frunaceFile << FurnaceList[i]->GetSmeltingTotal() << std::endl;
			
			frunaceFile << FurnaceList[i]->GetResultID() << std::endl;
			frunaceFile << FurnaceList[i]->GetResultTotal() << std::endl;
			
			frunaceFile << FurnaceList[i]->GetSmeltingTime() << std::endl;
			frunaceFile << FurnaceList[i]->GetFuelTime() << std::endl;
		}
		frunaceFile.close();
	}
	else
		std::cout << " cant save !" << std::endl;
}

void SP2::LoadWorld()
{
	std::ifstream worldFile("WorldSaveFile.txt"); //Open text file to read
	std::string row;
	int level = 0;
	if (worldFile.is_open())
	{
		while (!worldFile.eof())
		{
			worldFile >> row;
			for (unsigned int i = 0; i < row.size(); ++i)
			{
				world[i][level] = row[i];
			}
			level += 1;
		}
		worldFile.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;
	
	//Count the lines in the text file
	int cropLines = 0;
	row = "";
	std::ifstream cropFileLines("CropSaveFile.txt"); //Open text file to read
	if (cropFileLines.is_open())
	{
		while (std::getline(cropFileLines, row))
			cropLines += 1;
		cropFileLines.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;
	//

	std::ifstream cropFile("CropSaveFile.txt"); //Open text file to read
	int count = 0;
	int xTile, zTile, cropType;
	double timeSince;

	if (cropFile.is_open())
	{
		while (!cropFile.eof())
		{
			std::cout << "x" << std::endl;
			count += 1;
			if (count == 1)
				cropFile >> xTile;
			else if (count == 2)
				cropFile >> zTile;
			else if (count == 3)
				cropFile >> cropType;
			else if (count == 4)
			{
				cropFile >> timeSince;
				CropList.push_back(new Crops(cropType, xTile, zTile, timeSince));
				count = 0;
				std::cout << "new crop" << std::endl;
			}
		}
		cropFile.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;

	int furnaceLines = 0;
	row = "";
	std::ifstream furnaceFileLines("FurnaceSaveFile.txt"); //Open text file to read
	if (furnaceFileLines.is_open())
	{
		while (std::getline(furnaceFileLines, row))
			cropLines += 1;
		furnaceFileLines.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;

	std::ifstream furnaceFile("FurnaceSaveFile.txt"); //Open text file to read
	int count2 = 0;
	int/* xTile , zTile , */smeltID , smeltCount , fuelID , fuelCount , resultID , resultCount;
	double smeltTime, fuelTime;
	if (furnaceFile.is_open())
	{
		while (!furnaceFile.eof())
		{
			count2 += 1;
			
			if (count2 == 1)
				furnaceFile >> xTile;
			else if (count2 == 2)
				furnaceFile >> zTile;
			else if (count2 == 3)
				furnaceFile >> fuelID;
			else if (count2 == 4)
				furnaceFile >> fuelCount;
			else if (count2 == 5)
				furnaceFile >> smeltID;
			else if (count2 == 6)
				furnaceFile >> smeltCount;
			else if (count2 == 7)
				furnaceFile >> resultID;
			else if (count2 == 8)
				furnaceFile >> resultCount;
			else if (count2 == 9)
				furnaceFile >> smeltTime;
			else if (count2 == 10)
			{
				count2 = 0;
				furnaceFile >> fuelTime;
				FurnaceList.push_back(new Furnace(xTile, zTile, fuelID, fuelCount, smeltID, smeltCount, resultID, resultCount, smeltTime , fuelTime));
			}
		}
		furnaceFile.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;
}

void SP2::InitGround()
{
	int x = 250, z = 250;

	for (int i = 0; i < x; ++i)
	{
		for (int j = 0; j < z; ++j)
		{
			float randVal = Math::RandFloatMinMax(0, 10);

			if (randVal < 0.01)
			{
				world[i][j] = 'G'; // Ore generation
			}
			else if (randVal < 0.02)
			{
				world[i][j] = 'C'; // Ore generation
			}
			else if (randVal < 0.04)
			{
				if (i > 1 && i < 249 && j > 1 && j < 249)
					world[i][j] = 'W'; // Water generation
			}
			else if (randVal < 0.05)
			{
				world[i][j] = 'B'; // Berry generation
			}
			else if (randVal < 0.3)
			{
				world[i][j] = 'T'; // Tree generation
			}
			else
				world[i][j] = 'G'; // Grass generation
		}
	}

	for (int i = 0; i < 250; ++i)
	{
		for (int k = 0; k < 250; ++k)
		{
			if (i > 0 && i < 249 && k > 0 && k < 249)
			{
				if (world[i][k] == 'W')
				{
					world[i - 1][k] = 'W';
					world[i][k - 1] = 'W';
				}
			}
		}
	}

	world[25][25] = 'D';

	int dungeonCentreX = 0;
	int dungeonCentreZ = 0;

	for (int i = 0; i < 250; ++i)
	{
		for (int j = 0; j < 250; ++j)
		{
			if (world[i][j] == 'D')
			{
				dungeonCentreX = i;
				dungeonCentreZ = j;
				break;
			}
		}
	}

	int dungeonSize = 10;

	for (int i = dungeonCentreX - dungeonSize; i < dungeonCentreX + dungeonSize; ++i)
	{
		for (int j = dungeonCentreZ - dungeonSize; j < dungeonCentreZ + dungeonSize; ++j)
		{
			world[i][j] = 'd';
		}
	}

	for (int i = dungeonCentreX - dungeonSize; i < dungeonCentreX + dungeonSize; ++i)
	{
		world[i][dungeonCentreX - dungeonSize] = 'D';
		world[i][dungeonCentreX + dungeonSize] = 'D';
	}

	for (int i = dungeonCentreZ - dungeonSize; i < dungeonCentreZ + dungeonSize; ++i)
	{
		world[dungeonCentreZ - dungeonSize][i] = 'D';
		world[dungeonCentreZ + dungeonSize][i] = 'D';
	}
	
	for (int i = dungeonCentreZ - dungeonSize; i < dungeonCentreZ + dungeonSize; ++i)
	{
		for (int j = dungeonCentreX - dungeonSize; j < dungeonCentreX + dungeonSize; j++)
		{
			int checker = Math::RandIntMinMax(0, 10);

			if (checker > 2)
			{
				if (i % 2 != 0)
					world[i][j] = 'D';
			}
		}
	}

	world[dungeonCentreX - dungeonSize][dungeonCentreZ] = 'd';
	world[dungeonCentreX + dungeonSize - 1][dungeonCentreZ] = 'L';
}

void SP2::Init()
{
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Interaction", "Image//Interaction.mp3");
	CSoundEngine::GetInstance()->AddSound("Tilling_Ground", "Image//Tilling_Ground.mp3");
	
	m_bMenu = true;
	m_bContinue = false;
	Math::InitRNG();
	m_fNavigatorY = 0;
	
	m_bRandLightning = true;

	m_bLightningStrike = false;
	m_bRandTimeTillLightning = true;
	m_fTimeTillLightning = 0; 
	m_fLightningDuration = 2;

	scale = 100;
	
	pX = camera.position.x / scale;
	pZ = camera.position.z / scale;

	outwards = 25;

	m_fAmbient = 0.6f;
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

	m_parameters[U_LIGHT1_TYPE] = glGetUniformLocation(m_programID, "lights[1].type");
	m_parameters[U_LIGHT1_POSITION] = glGetUniformLocation(m_programID, "lights[1].position_cameraspace");
	m_parameters[U_LIGHT1_COLOR] = glGetUniformLocation(m_programID, "lights[1].color");
	m_parameters[U_LIGHT1_POWER] = glGetUniformLocation(m_programID, "lights[1].power");
	m_parameters[U_LIGHT1_KC] = glGetUniformLocation(m_programID, "lights[1].kC");
	m_parameters[U_LIGHT1_KL] = glGetUniformLocation(m_programID, "lights[1].kL");
	m_parameters[U_LIGHT1_KQ] = glGetUniformLocation(m_programID, "lights[1].kQ");
	m_parameters[U_LIGHT1_SPOTDIRECTION] = glGetUniformLocation(m_programID, "lights[1].spotDirection");
	m_parameters[U_LIGHT1_COSCUTOFF] = glGetUniformLocation(m_programID, "lights[1].cosCutoff");
	m_parameters[U_LIGHT1_COSINNER] = glGetUniformLocation(m_programID, "lights[1].cosInner");
	m_parameters[U_LIGHT1_EXPONENT] = glGetUniformLocation(m_programID, "lights[1].exponent");

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
	lights[0].position.Set(12250, 100, 12250);
	lights[0].color.Set(1, 1, 1);
	lights[0].power = 1.f;
	lights[0].kC = 1.f;
	lights[0].kL = 0.01f;
	lights[0].kQ = 0.001f;
	lights[0].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[0].cosInner = cos(Math::DegreeToRadian(30));
	lights[0].exponent = 3.f;
	lights[0].spotDirection.Set(0.f, 1.f, 0.f);

	lights[1].type = Light::LIGHT_POINT;
	lights[1].position.Set(0 , 50, 0);
	lights[1].color.Set(1, 1, 1);
	lights[1].power = 500.f;
	lights[1].kC = 1.f;
	lights[1].kL = 0.01f;
	lights[1].kQ = 0.001f;
	lights[1].cosCutoff = cos(Math::DegreeToRadian(45));
	lights[1].cosInner = cos(Math::DegreeToRadian(30));
	lights[1].exponent = 3.f;
	lights[1].spotDirection.Set(0.f, 1.f, 0.f);

	glUniform1i(m_parameters[U_NUMLIGHTS], 2);
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

	glUniform1i(m_parameters[U_LIGHT1_TYPE], lights[1].type);
	glUniform3fv(m_parameters[U_LIGHT1_COLOR], 1, &lights[1].color.r);
	glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
	glUniform1f(m_parameters[U_LIGHT1_KC], lights[1].kC);
	glUniform1f(m_parameters[U_LIGHT1_KL], lights[1].kL);
	glUniform1f(m_parameters[U_LIGHT1_KQ], lights[1].kQ);
	glUniform1f(m_parameters[U_LIGHT1_COSCUTOFF], lights[1].cosCutoff);
	glUniform1f(m_parameters[U_LIGHT1_COSINNER], lights[1].cosInner);
	glUniform1f(m_parameters[U_LIGHT1_EXPONENT], lights[1].exponent);

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

	camera.Init(Vector3(0, 50, 0), Vector3(0, 200, -10), Vector3(0, 1, 0));

	player = new PlayerInformation();
	player->AttachCamera(&camera);
	
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		meshList[i] = NULL;
	}

	meshList[GEO_AXES] = MeshBuilder::GenerateAxes("reference");
	meshList[GEO_CROSSHAIR] = MeshBuilder::GenerateCrossHair("crosshair");
	meshList[GEO_QUAD] = MeshBuilder::GenerateQuad("quad", Color(1, 1, 1), 1.f);
	meshList[GEO_TEXT] = MeshBuilder::GenerateText("text", 16, 16);
	meshList[GEO_TEXT]->textureArray[0] = LoadTGA("Image//Font.tga");
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

	meshList[GEO_SKYBOX_WINTER] = MeshBuilder::GenerateOBJ("GEO_SKYBOX_WINTER", "OBJ//skybox.obj");
	meshList[GEO_SKYBOX_WINTER]->textureArray[0] = LoadTGA("Image//Skybox_Winter.tga");

	//
	meshList[GEO_TILLED_DIRT] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_TILLED_DIRT]->textureArray[0] = LoadTGA("Image//Tilled_Dirt.tga");

	meshList[GEO_GRASS_SPRING] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_SPRING]->textureArray[0] = LoadTGA("Image//Grass.tga");

	meshList[GEO_GRASS_SPRING] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_SPRING]->textureArray[0] = LoadTGA("Image//Grass.tga");
	meshList[GEO_GRASS_SPRING]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);

	meshList[GEO_GRASS_SUMMER] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_SUMMER]->textureArray[0] = LoadTGA("Image//Grass_summer.tga");
	meshList[GEO_GRASS_SPRING]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);

	meshList[GEO_GRASS_WINTER] = MeshBuilder::GenerateQuad("Grass", Color(1, 1, 1), 1.f);
	meshList[GEO_GRASS_WINTER]->textureArray[0] = LoadTGA("Image//Grass_winter.tga");
	meshList[GEO_GRASS_SPRING]->material.kAmbient.Set(0.3f, 0.3f, 0.3f);
	//
	meshList[GEO_STONE_BRICK_FLOOR] = MeshBuilder::GenerateQuad("GEO_STONE_BRICK_FLOOR", Color(1, 1, 1), 1.f);
	meshList[GEO_STONE_BRICK_FLOOR]->textureArray[0] = LoadTGA("Image//StoneBricks.tga");
	
	//Game Stuff
	meshList[GEO_WATER] = MeshBuilder::GenerateQuad("Water", Color(1, 1, 1), 1.f);
	meshList[GEO_WATER]->textureArray[0] = LoadTGA("Image//Water.tga");

	meshList[GEO_WATER_WINTER] = MeshBuilder::GenerateQuad("Water", Color(1, 1, 1), 1.f);
	meshList[GEO_WATER_WINTER]->textureArray[0] = LoadTGA("Image//iced_water.tga");

	meshList[GEO_TREE_SPRING] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_SPRING]->textureArray[0] = LoadTGA("Image//Tree_Spring.tga");

	meshList[GEO_TREE_SUMMER] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_SUMMER]->textureArray[0] = LoadTGA("Image//Tree_Summer.tga");

	meshList[GEO_TREE_WINTER] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_WINTER]->textureArray[0] = LoadTGA("Image//Tree_Winter.tga");

	meshList[GEO_TREE_WINTER] = MeshBuilder::GenerateOBJ("Tree", "OBJ//Tree.obj");
	meshList[GEO_TREE_WINTER]->textureArray[0] = LoadTGA("Image//Tree_Winter.tga");

	meshList[GEO_GOLD_ORE] = MeshBuilder::GenerateOBJ("Ore", "OBJ//Ore.obj");
	meshList[GEO_GOLD_ORE]->textureArray[0] = LoadTGA("Image//Gold_Ore.tga");

	meshList[GEO_COAL_ORE] = MeshBuilder::GenerateOBJ("Ore", "OBJ//Ore.obj");
	meshList[GEO_COAL_ORE]->textureArray[0] = LoadTGA("Image//Coal_Ore.tga");

	meshList[GEO_BERRY_BUSH] = MeshBuilder::GenerateOBJ("Berry", "OBJ//Bush.obj");
	meshList[GEO_BERRY_BUSH]->textureArray[0] = LoadTGA("Image//Bush.tga");

	meshList[GEO_NO_BERRY_BUSH] = MeshBuilder::GenerateOBJ("GEO_NO_BERRY", "OBJ//Bush.obj");
	meshList[GEO_NO_BERRY_BUSH]->textureArray[0] = LoadTGA("Image//Bush_Empty.tga");

	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("GEO_WALL", "OBJ//Wall.obj");
	meshList[GEO_WALL]->textureArray[0] = LoadTGA("Image//Wall.tga");
	//

	//Player
	meshList[GEO_PLAYER] = MeshBuilder::GenerateOBJ("GEO_PLAYER", "OBJ//Steve.obj");
	meshList[GEO_PLAYER]->textureArray[0] = LoadTGA("Image//Steve.tga");
	
	//Animals
	meshList[GEO_PIG] = MeshBuilder::GenerateOBJ("Pig", "OBJ//Pig.obj");
	meshList[GEO_PIG]->textureArray[0] = LoadTGA("Image//PIG.tga");

	meshList[GEO_CHICKEN] = MeshBuilder::GenerateOBJ("Chicken", "OBJ//chicken.obj");
	meshList[GEO_CHICKEN]->textureArray[0] = LoadTGA("Image//chicken.tga");

	meshList[GEO_COW] = MeshBuilder::GenerateOBJ("Cow", "OBJ//Cow2.obj");
	meshList[GEO_COW]->textureArray[0] = LoadTGA("Image//cow.tga");

	//GUI's
	//Enemy
	meshList[GEO_ZOMBIE] = MeshBuilder::GenerateOBJ("Zombie", "OBJ//zombie.obj");
	meshList[GEO_ZOMBIE]->textureArray[0] = LoadTGA("Image//zombie.tga");

	meshList[GEO_WITCH] = MeshBuilder::GenerateOBJ("Witch", "OBJ//zombie.obj");
	meshList[GEO_WITCH]->textureArray[0] = LoadTGA("Image//witch.tga");
	
	//
	meshList[GEO_INVENTORY] = MeshBuilder::GenerateQuad("GEO_INVENTORY", Color(1, 1, 1), 1.0f);
	meshList[GEO_INVENTORY]->textureArray[0] = LoadTGA("Image//Inventory.tga");

	meshList[GEO_CRAFTING_MENU] = MeshBuilder::GenerateQuad("GEO_CRAFTING_MENU", Color(1, 1, 1), 1.0f);
	meshList[GEO_CRAFTING_MENU]->textureArray[0] = LoadTGA("Image//Crafting.tga");

	meshList[GEO_EMPTY_INVENTORY] = MeshBuilder::GenerateQuad("GEO_EMPTY_INVENTORY", Color(1, 1, 1), 1.0f);
	meshList[GEO_EMPTY_INVENTORY]->textureArray[0] = LoadTGA("Image//Empty_Inventory.tga");

	meshList[GEO_HIGHLIGHT_INVENTORY] = MeshBuilder::GenerateQuad("GEO_HIGHLIGHT_INVENTORY", Color(1, 1, 1), 1.0f);
	meshList[GEO_HIGHLIGHT_INVENTORY]->textureArray[0] = LoadTGA("Image//Highlight_Inventory.tga");

	meshList[GEO_EMPTY_CRAFTING] = MeshBuilder::GenerateQuad("GEO_EMPTY", Color(1, 1, 1), 1.0f);
	meshList[GEO_EMPTY_CRAFTING]->textureArray[0] = LoadTGA("Image//Empty_Crafting.tga");

	meshList[GEO_SMELTING_MENU] = MeshBuilder::GenerateQuad("GEO_SMELTING_MENU", Color(1, 1, 1), 1.0f);
	meshList[GEO_SMELTING_MENU]->textureArray[0] = LoadTGA("Image//Furnace_Menu.tga");
	//

	//Inventory meshes
	meshList[GEO_MEAT] = MeshBuilder::GenerateQuad("GEO_MEAT", Color(1, 1, 1), 1.0f);
	meshList[GEO_MEAT]->textureArray[0] = LoadTGA("Image//Meat.tga");

	meshList[GEO_COOKED_MEAT] = MeshBuilder::GenerateQuad("GEO_COOKED_MEAT", Color(1, 1, 1), 1.0f);
	meshList[GEO_COOKED_MEAT]->textureArray[0] = LoadTGA("Image//Cooked_Meat.tga");

	meshList[GEO_ICE_CUBE] = MeshBuilder::GenerateQuad("GEO_ICE_CUBE", Color(1, 1, 1), 1.0f);
	meshList[GEO_ICE_CUBE]->textureArray[0] = LoadTGA("Image//Ice_Cube.tga");

	meshList[GEO_WATER_BOTTLE] = MeshBuilder::GenerateQuad("GEO_WATER_BOTTLE", Color(1, 1, 1), 1.0f);
	meshList[GEO_WATER_BOTTLE]->textureArray[0] = LoadTGA("Image//Water_Bottle.tga");

	meshList[GEO_WHEAT] = MeshBuilder::GenerateQuad("GEO_WHEAT", Color(1, 1, 1), 1.0f);
	meshList[GEO_WHEAT]->textureArray[0] = LoadTGA("Image//Wheat.tga");

	meshList[GEO_BREAD] = MeshBuilder::GenerateQuad("GEO_BREAD", Color(1, 1, 1), 1.0f);
	meshList[GEO_BREAD]->textureArray[0] = LoadTGA("Image//Bread.tga");

	meshList[GEO_CARROT] = MeshBuilder::GenerateQuad("GEO_CARROT", Color(1, 1, 1), 1.0f);
	meshList[GEO_CARROT]->textureArray[0] = LoadTGA("Image//Carrot.tga");

	meshList[GEO_SEED] = MeshBuilder::GenerateQuad("GEO_SEED", Color(1, 1, 1), 1.0f);
	meshList[GEO_SEED]->textureArray[0] = LoadTGA("Image//Seed.tga");

	meshList[GEO_BERRY] = MeshBuilder::GenerateQuad("GEO_SEED", Color(1, 1, 1), 1.0f);
	meshList[GEO_BERRY]->textureArray[0] = LoadTGA("Image//Berry.tga");

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

	meshList[GEO_WOODEN_PICKAXE] = MeshBuilder::GenerateQuad("GEO_WOODEN_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_WOODEN_PICKAXE]->textureArray[0] = LoadTGA("Image//Wooden_Pickaxe.tga");

	meshList[GEO_WOODEN_AXE] = MeshBuilder::GenerateQuad("GEO_WOODEN_AXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_WOODEN_AXE]->textureArray[0] = LoadTGA("Image//Wooden_Axe.tga");

	meshList[GEO_WOODEN_HOE] = MeshBuilder::GenerateQuad("GEO_WOODEN_HOE", Color(1, 1, 1), 1.0f);
	meshList[GEO_WOODEN_HOE]->textureArray[0] = LoadTGA("Image//Wooden_Hoe.tga");

	meshList[GEO_STONE_SWORD] = MeshBuilder::GenerateQuad("GEO_STONE_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE_SWORD]->textureArray[0] = LoadTGA("Image//Stone_Sword.tga");

	meshList[GEO_STONE_PICKAXE] = MeshBuilder::GenerateQuad("GEO_STONE_PICKAXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE_PICKAXE]->textureArray[0] = LoadTGA("Image//Stone_Pickaxe.tga");

	meshList[GEO_STONE_AXE] = MeshBuilder::GenerateQuad("GEO_STONE_AXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE_AXE]->textureArray[0] = LoadTGA("Image//Stone_Axe.tga");

	meshList[GEO_STONE_HOE] = MeshBuilder::GenerateQuad("GEO_STONE_HOE", Color(1, 1, 1), 1.0f);
	meshList[GEO_STONE_HOE]->textureArray[0] = LoadTGA("Image//Stone_Hoe.tga");

	meshList[GEO_GOLD_SWORD] = MeshBuilder::GenerateQuad("GEO_GOLD_SWORD", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_SWORD]->textureArray[0] = LoadTGA("Image//Gold_Sword.tga");

	meshList[GEO_GOLD_PICKAXE] = MeshBuilder::GenerateQuad("GEO_GOLD_PICKAXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_PICKAXE]->textureArray[0] = LoadTGA("Image//Gold_Pickaxe.tga");

	meshList[GEO_GOLD_AXE] = MeshBuilder::GenerateQuad("GEO_GOLD_AXE", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_AXE]->textureArray[0] = LoadTGA("Image//Gold_Axe.tga");

	meshList[GEO_GOLD_HOE] = MeshBuilder::GenerateQuad("GEO_STONE_HOE", Color(1, 1, 1), 1.0f);
	meshList[GEO_GOLD_HOE]->textureArray[0] = LoadTGA("Image//Gold_Hoe.tga");

	meshList[GEO_FURNACE] = MeshBuilder::GenerateQuad("GEO_FURNACE", Color(1, 1, 1), 1.0f);
	meshList[GEO_FURNACE]->textureArray[0] = LoadTGA("Image//Furnace.tga");

	//World Objects
	meshList[GEO_FURNACE_BLOCK] = MeshBuilder::GenerateOBJ("GEO_FURNACE_BLOCK", "OBJ//Cube.obj");
	meshList[GEO_FURNACE_BLOCK]->textureArray[0] = LoadTGA("Image//Furnace.tga");

	meshList[GEO_CHEST_BLOCK] = MeshBuilder::GenerateOBJ("GEO_FURNACE_BLOCK", "OBJ//Cube.obj");
	meshList[GEO_CHEST_BLOCK]->textureArray[0] = LoadTGA("Image//Ritual.tga");
	//

	//Yansons models
	meshList[GEO_WOODEN_SWORD_MODEL] = MeshBuilder::GenerateOBJ("GEO_WOODEN_SWORD_MODEL", "OBJ//sword.obj");
	meshList[GEO_WOODEN_SWORD_MODEL]->textureArray[0] = LoadTGA("Image//Wooden_Sword.tga");

	meshList[GEO_STONE_SWORD_MODEL] = MeshBuilder::GenerateOBJ("GEO_STONE_SWORD_MODEL", "OBJ//sword.obj");
	meshList[GEO_STONE_SWORD_MODEL]->textureArray[0] = LoadTGA("Image//Stone_Sword.tga");
	//wORLD
	meshList[GEO_CASTLE] = MeshBuilder::GenerateOBJ("GEO_CASTLE", "OBJ//Castle.obj");
	meshList[GEO_CASTLE]->textureArray[0] = LoadTGA("Image//StoneBricks.tga");

	//Lightning model
	meshList[GEO_LIGHTNING] = MeshBuilder::GenerateQuad("GEO_LIGHTNING", Color(1, 1, 1), 1.0f);
	
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
	meshList[GEO_PARTICLE_HEART] = MeshBuilder::GenerateQuad("GEO_PARTICLE_HEART", Color(1, 1, 1), 1.0f);
	meshList[GEO_PARTICLE_HEART]->textureArray[0] = LoadTGA("Image//particle_heart.tga");

	m_particleCount = 0;
	MAX_PARTICLE = 1000;
	//Sprite animation
	meshList[GEO_SPRITE_ANIMATION] = MeshBuilder::GenerateSpriteAnimation("cat", 1, 17);
	meshList[GEO_SPRITE_ANIMATION]->textureArray[0] = LoadTGA("Image//tabby.tga");
	//Shadow
	meshList[GEO_LIGHT_DEPTH_QUAD] = MeshBuilder::GenerateQuad("LIGHT_DEPTH_TEXTURE", Color(1, 1, 1), 1.f);
	meshList[GEO_LIGHT_DEPTH_QUAD]->textureArray[0] = m_lightDepthFBO.GetTexture();

	meshList[GEO_MENU] = MeshBuilder::GenerateQuad("GEO_MENU", Color(1, 1, 1), 1.f);
	meshList[GEO_MENU]->textureArray[0] = LoadTGA("Image//Menu.tga");

	meshList[GEO_PLAYER_SELECTION] = MeshBuilder::GenerateQuad("GEO_MENU", Color(1, 1, 1), 1.f);
	meshList[GEO_PLAYER_SELECTION]->textureArray[0] = LoadTGA("Image//Player_Selection.tga");

	meshList[GEO_NAVIGATOR] = MeshBuilder::GenerateQuad("GEO_NAVIGATOR", Color(1, 1, 1), 1.f);
	meshList[GEO_NAVIGATOR]->textureArray[0] = LoadTGA("Image//Navigator.tga");
	
	meshList[GEO_GAME_OVER] = MeshBuilder::GenerateQuad("GEO_GAME_OVER", Color(1, 1, 1), 1.f);
	meshList[GEO_GAME_OVER]->textureArray[0] = LoadTGA("Image//Game_Over.tga");

	//UI
		//HEALTH
		meshList[GEO_HEALTH_FULL] = MeshBuilder::GenerateQuad("GEO_HEALTH_FULL", Color(1, 1, 1), 1.0f);
		meshList[GEO_HEALTH_FULL]->textureArray[0] = LoadTGA("Image//health_full.tga");

		meshList[GEO_HEALTH_HALF] = MeshBuilder::GenerateQuad("GEO_HEALTH_HALF", Color(1, 1, 1), 1.0f);
		meshList[GEO_HEALTH_HALF]->textureArray[0] = LoadTGA("Image//health_half.tga");

		meshList[GEO_HEALTH_EMPTY] = MeshBuilder::GenerateQuad("GEO_HEALTH_EMPTY", Color(1, 1, 1), 1.0f);
		meshList[GEO_HEALTH_EMPTY]->textureArray[0] = LoadTGA("Image//health_empty.tga");
		//HUNGER
		meshList[GEO_HUNGER_FULL] = MeshBuilder::GenerateQuad("GEO_HUNGER_FULL", Color(1, 1, 1), 1.0f);
		meshList[GEO_HUNGER_FULL]->textureArray[0] = LoadTGA("Image//hunger_full.tga");

		meshList[GEO_HUNGER_HALF] = MeshBuilder::GenerateQuad("GEO_HUNGER_HALF", Color(1, 1, 1), 1.0f);
		meshList[GEO_HUNGER_HALF]->textureArray[0] = LoadTGA("Image//hunger_half.tga");

		meshList[GEO_HUNGER_EMPTY] = MeshBuilder::GenerateQuad("GEO_HUNGER_EMPTY", Color(1, 1, 1), 1.0f);
		meshList[GEO_HUNGER_EMPTY]->textureArray[0] = LoadTGA("Image//hunger_empty.tga");
		//THIRST
		meshList[GEO_THIRST_FULL] = MeshBuilder::GenerateQuad("GEO_THIRST_FULL", Color(1, 1, 1), 1.0f);
		meshList[GEO_THIRST_FULL]->textureArray[0] = LoadTGA("Image//thirst_full.tga");

		meshList[GEO_THIRST_EMPTY] = MeshBuilder::GenerateQuad("GEO_THIRST_EMPTY", Color(1, 1, 1), 1.0f);
		meshList[GEO_THIRST_EMPTY]->textureArray[0] = LoadTGA("Image//thirst_empty.tga");
		//UI
		meshList[GEO_INSTRUCTION1] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION1", Color(1, 1, 1), 1.0f);
		meshList[GEO_INSTRUCTION1]->textureArray[0] = LoadTGA("Image//instruction1.tga");
		meshList[GEO_INSTRUCTION2] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION2", Color(1, 1, 1), 1.0f);
		meshList[GEO_INSTRUCTION2]->textureArray[0] = LoadTGA("Image//instruction2.tga");
		meshList[GEO_INSTRUCTION3] = MeshBuilder::GenerateQuad("GEO_INSTRUCTION2", Color(1, 1, 1), 1.0f);
		meshList[GEO_INSTRUCTION3]->textureArray[0] = LoadTGA("Image//instruction3.tga");
		//
	//Projectile
	meshList[GEO_FIREBALL] = MeshBuilder::GenerateOBJ("GEO_FIREBALL", "OBJ//fireball.obj");
	//meshList[GEO_FIREBALL]->textureArray[0] = LoadTGA("Image//fireball.tga");

	SpriteAnimation *sa = dynamic_cast<SpriteAnimation*>(meshList[GEO_SPRITE_ANIMATION]);
	if (sa)
	{
		sa->m_anim = new Animation();
		sa->m_anim->Set(0, 17, 1, 1.f, true);
		//Startframe, endframe, repeat, time, active
	}

	// Projection matrix : 45� Field of View, 4:3 ratio, display range : 0.1 unit <-> 1000 units
	Mtx44 perspective;
	perspective.SetToPerspective(45.0f, 4.0f / 3.0f, 0.1f, 10000.0f);
	//perspective.SetToOrtho(-80, 80, -60, 60, -1000, 1000);
	projectionStack.LoadMatrix(perspective);
	bLightEnabled = true;
	translate_tex_coord = 0;


	//Test item stacking
	//Particle
	for (int i = 0; i < MAX_PARTICLE; ++i)
	{
		particleList.push_back(new ParticleObject(ParticleObject_TYPE::P_WATER));
	}
	//Projectile
	for (int i = 0; i < MAX_PROJECTILE; ++i)
	{
		ProjectileList.push_back(new ProjectileObject(PROJECTILE_TYPE::P_FIREBALL));
	}

	m_NumOfAnimal = 0;
	m_NumOfEnemy = 0;

	//Season
	SP2_Seasons = new Season;
	SP2_Seasons->setSeason(0);
	WindAffecting = false;
	m_fWindBounceTime = 0;
	m_bTexChange = false;

	//Temp furnace for testing , delete when raytrace is introduced.
	//FurnaceList.push_back(new Furnace);
	//FurnaceList[0]->SetStatus(true);

	//Crops assign to where the player plants it. so get the tile position and multiply by scale.
	//CropList.push_back(new Crops(0 , 12250 , 12250));

	//CropList.push_back(new Crops(0 , 125, 125));

	//instructions
	instructionorder = 0;
	instructiontimer = 0.f;
	//Projectile
	MAX_PROJECTILE = 500;
	m_iProjectileCount = 0;
	m_swingcount = 0;

	//Sound
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Pig_Resting", "Image//Pig_Resting.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Pig_Dying", "Image//Pig_Dying.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Chicken_Resting", "Image//Chicken_Resting.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Chicken_Dying", "Image//Chicken_Dying.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Cow_Resting", "Image//Cow_Resting.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Cow_Dying", "Image//Cow_Dying.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Zombie_Resting", "Image//Zombie_Resting.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Zombie_Dying", "Image//Zombie_Dying.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Witch_Resting", "Image//Witch_Resting.mp3");
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Witch_Dying", "Image//Villager_Dying.mp3");
	//Background
	CSoundEngine::GetInstance()->Init();
	CSoundEngine::GetInstance()->AddSound("Spring", "Image//Spring.mp3");
	CSoundEngine::GetInstance()->PlayASound2DLoop("Spring", true);

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

char SP2::GetPlayerCurrentTile(float xPos , float yPos)
{
	int x = (xPos + scale / 2) / scale;
	int y = (yPos + scale / 2) / scale;
	return world[x][y];
}

void SP2::Update(double dt)
{
	m_dBounceTime -= 1 * dt;
	m_fConstantRotate += 20 * dt;

	if (m_bMenu == true)
	{
		if (Application::IsKeyPressed(VK_DOWN) && m_dBounceTime <= 0)
		{
			m_fNavigatorY -= 100;
			if (m_fNavigatorY < -100)
				m_fNavigatorY = 0;
			m_dBounceTime = 0.2;
		}
		else if (Application::IsKeyPressed(VK_UP) && m_dBounceTime <= 0)
		{
			m_fNavigatorY += 100;
			if (m_fNavigatorY > 0)
				m_fNavigatorY = -100;
			m_dBounceTime = 0.2;
		}

	
			if (Application::IsKeyPressed(VK_RETURN) && m_dBounceTime <= 0)
			{
				if (m_bContinue == false)
				{
					if (m_fNavigatorY == 0)
					{
						m_bContinue = true;
					}
					else
					{
						m_bMenu = false;
					}
					m_dBounceTime = 0.2;
				}
				else
				{
					if (m_fNavigatorY == 0)
					{
						player->LoadData();
						LoadWorld();
						LoadAnimalData();
						LoadEnemyData();
						m_bMenu = false;
					}
					else
					{
						for (unsigned int i = 0; i < 100; ++i)
						{
							m_AnimalList.push_back(new CAnimal(CAnimal::GO_PIG));
							m_EnemyList.push_back(new CEnemy(CEnemy::GO_ZOMBIE));
						}
						m_bMenu = false;
					}
					m_dBounceTime = 0.2;
				}
			}
	}
	else if (m_bMenu == false)
	{
		if (Application::IsKeyPressed(VK_RETURN))
		{
			int x = camera.position.x / scale;
			int z = camera.position.z / scale;
		}
		
		if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_TORCH)
		{
			std::cout << "x" << std::endl;
			lights[1].position.x = camera.position.x;
			lights[1].position.y = camera.position.y;
			lights[1].position.z = camera.position.z;
			lights[1].power = 10;
			glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
		}
		else
		{
			lights[1].power = 0;
			glUniform1f(m_parameters[U_LIGHT1_POWER], lights[1].power);
		}


		// Duration of lightning strike = 0.12
		if (m_fTimeTillLightning <= 0.2)
		{
			m_bLightningStrike = false;
			if (m_bRandLightning == true)
			{
				lightningX = Math::RandFloatMinMax(camera.position.x - 500, camera.position.x + 500);
				lightningZ = Math::RandFloatMinMax(camera.position.z - 500, camera.position.z + 500);
				m_bRandLightning = false;

				lights[0].position.x = lightningX;
				lights[0].position.z = lightningZ;
			}
			if (m_fTimeTillLightning >= 0)
			{
				m_bLightningStrike = true;
				//Flashing effect.
				float x = Math::RandFloatMinMax(0, 1);

				if (x > 0.1)
					lights[0].power = 10;
				else
					lights[0].power = 1;
				glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
			}
			m_fTimeTillLightning += 0.1 * dt;
		}
		else
		{
			lights[0].power = 1;
			glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
			m_fTimeTillLightning = -0.3;
			m_bRandLightning = true;
		}

		if (Application::IsKeyPressed('H') && m_dBounceTime <= 0)
		{
			/*std::cout << static_cast<int>(camera.position.x / 100) << " " << static_cast<int>(camera.position.z / 100) << std::endl;
			player->addItem(new Item(Item::ITEM_WOOD, 1));
			player->addItem(new Item(Item::ITEM_FURNACE, 1));

			player->addItem(new Item(Item::ITEM_WOODEN_SWORD, 1));
			player->addItem(new Item(Item::ITEM_STONE, 1));
			player->addItem(new Item(Item::ITEM_GOLD_NUGGET, 1));
			player->addItem(new Item(Item::ITEM_COAL, 100));
			player->addItem(new Item(Item::ITEM_MEAT, 100));
			player->addItem(new Item(Item::ITEM_CARROT, 10));
			player->addItem(new Item(Item::ITEM_WHEAT, 10));
			player->addItem(new Item(Item::ITEM_SEED, 10));
			player->addItem(new Item(Item::ITEM_STONE, 10));*/

		
			player->addItem(new Item(Item::ITEM_ICE_CUBE, 1));
			player->addItem(new Item(Item::ITEM_GOLD_HOE, 1));
			player->addItem(new Item(Item::ITEM_SEED, 1));
			player->addItem(new Item(Item::ITEM_CARROT, 1));




			//world[int((camera.position.x + scale / 2) / scale)][int((camera.position.z + scale / 2) / scale)] = 'F';

		/*	world[125][125] = 'D';

			world[int((camera.position.x + scale / 2) / scale)][int((camera.position.z + scale / 2) / scale)] = 'c';*/

			m_dBounceTime = 0.5;
		}

		if (m_fAmbient >= 0.1 && m_iDayNight == 1)
		{
			m_fAmbient -= 0.002 * dt;

			for (int i = GEO_LIGHT_AFFECTED + 1; i < GEO_LIGHT_AFFECTED_END; ++i)
			{
				meshList[i]->material.kAmbient.Set(m_fAmbient, m_fAmbient, m_fAmbient);

				if (m_bLightningStrike == false)
				{
					lights[0].power = m_fAmbient;
					glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
				}
			}
		}
		else if (m_iDayNight == -1)
		{
			//How long day or night lasts
			m_fDayNightDuration -= 1 * dt;

			//Can start transitioning to day
			if (m_fDayNightDuration <= 0)
			{
				m_iDayNight *= -1;
				m_fDayNightDuration = 120;
			}
		}

		if (m_fAmbient <= 0.6 && m_iDayNight == -1)
		{
			m_fAmbient += 0.002 * dt;

			for (int i = GEO_LIGHT_AFFECTED + 1; i < GEO_LIGHT_AFFECTED_END; ++i)
			{
				meshList[i]->material.kAmbient.Set(m_fAmbient, m_fAmbient, m_fAmbient);
				if (m_bLightningStrike == false)
				{
					lights[0].power = m_fAmbient;
					glUniform1f(m_parameters[U_LIGHT0_POWER], lights[0].power);
				}
			}
		}
		else if (m_iDayNight == -1)
		{
			//How long day or night lasts
			m_fDayNightDuration -= 1 * dt;

			//Can start transitioning to day
			if (m_fDayNightDuration <= 0)
			{
				m_iDayNight *= -1;
				m_fDayNightDuration = 120;
			}
		}

		if (Application::IsKeyPressed('1'))
			glEnable(GL_CULL_FACE);
		if (Application::IsKeyPressed('2'))
			glDisable(GL_CULL_FACE);
		if (Application::IsKeyPressed('3'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		if (Application::IsKeyPressed('4'))
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

		if (Application::IsKeyPressed('F') && m_dBounceTime <= 0)
		{
			if (FurnaceList.size() > 0)
				FurnaceList[0]->SetStatus(true);
			
			m_dBounceTime = 0.2;
		}


	UpdateWorldVars();
	UpdateParticles(dt);
	UpdateProjectile(dt);

	char PlayerTile[9];
	float minx = camera.position.x - 100;
	if (minx < 0)
	{
		minx = 0;
	}
	float maxx = camera.position.x + 100;
	if (maxx > 25000)
	{
		maxx = 25000;
	}
	float minz = camera.position.z - 100;
	if (minz < 0)
	{
		minz = 0;
	}
	float maxz = camera.position.x + 100;
	if (maxz > 25000)
	{
		maxz = 25000;
	}
	PlayerTile[0] = GetPlayerCurrentTile(camera.position.x, camera.position.z);
	PlayerTile[1] = GetPlayerCurrentTile(minx, minz);
	PlayerTile[2] = GetPlayerCurrentTile(minx, camera.position.z);
	PlayerTile[3] = GetPlayerCurrentTile(minx, maxz);
	PlayerTile[4] = GetPlayerCurrentTile(camera.position.x, minz);
	PlayerTile[5] = GetPlayerCurrentTile(camera.position.x, maxz);
	PlayerTile[6] = GetPlayerCurrentTile(maxx, minz);
	PlayerTile[7] = GetPlayerCurrentTile(maxx, camera.position.z);
	PlayerTile[8] = GetPlayerCurrentTile(maxx, maxz);
	
	std::vector<char> FurnaceX;
	std::vector<char> FurnaceZ;

	for (int i = 0; i < FurnaceList.size(); i++)
	{
		FurnaceX.push_back(FurnaceList[i]->GetXTile());
		FurnaceZ.push_back(FurnaceList[i]->GetZTile());
	}
	player->update(dt, m_AnimalList,m_EnemyList, PlayerTile, FurnaceX, FurnaceZ);
	
	//Code for block placing based off item in player inventory
	if (player->getcurtool()->GetFurnaceID() >= 0 && m_dBounceTime <= 0)
	{
		m_currentfurnace = player->getcurtool()->GetFurnaceID();
		FurnaceList[player->getcurtool()->GetFurnaceID()]->SetStatus(true);
		player->SetFurnaceStatus(true);
		m_dBounceTime = 0.2;
	}
	if (player->getcurtool()->GetBerryClick())
	{
		int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
		int y = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;
		world[x][y] = 'b';
		int randVal = Math::RandIntMinMax(1, 3);
		player->addItem(new Item(Item::ITEM_BERRY, randVal));
		HarvestedBushList.push_back(new Bush(x, y));
	}
	if (player->getcurtool()->GetWaterClick())
	{
		int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
		int y = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;
		player->SetThirst(player->getThirst() + 10);
	}
	if (player->getcurtool()->GetAnimalID() >= 0)
	{
		m_AnimalList[player->getcurtool()->GetAnimalID()]->SetFed(true);
		player->getItem(player->getCurrentSlot())->addQuantity(-1);
	}
	if (Application::IsMousePressed(1) && m_dBounceTime <= 0 && player->GetFurnaceStatus() == true)
	{
		CSoundEngine::GetInstance()->PlayASound2D("Interaction");
		FurnaceList[m_currentfurnace]->SetStatus(false);
		player->SetFurnaceStatus(false);
		m_dBounceTime = 0.2;
	}
	if (player->GetPlaceDown())
	{
		if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_FURNACE)
		{
			int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
			int y = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;
			world[x][y] = 'F';
			player->getItem(player->getCurrentSlot())->addQuantity(-1);
			FurnaceList.push_back(new Furnace(x, y));

			CSoundEngine::GetInstance()->PlayASound2D("Interaction");
		}
		else if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_WOODEN_HOE ||
			player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_STONE_HOE ||
			player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_GOLD_HOE)
		{
			int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
			int y = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;
			if (world[x][y] != 'F')
			{
				world[x][y] = 't';
			}

			CSoundEngine::GetInstance()->PlayASound2D("Tilling_Ground");

		}
		else if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_SEED)
		{
			int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
			int y = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;

			if (world[x][y] == 't')
			{
				std::cout << "planted something" << std::endl;
				world[x][y] = 'w';
				CropList.push_back(new Crops(1, x, y , 0));
				player->getItem(player->getCurrentSlot())->addQuantity(-1);
				CSoundEngine::GetInstance()->PlayASound2D("Tilling_Ground");

			}
		}
		else if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_CARROT)
		{
			int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
			int y = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;

			if (world[x][y] == 't')
			{
				std::cout << "planted something" << std::endl;
				world[x][y] = 'c';
				CropList.push_back(new Crops(0, x, y , 0));
				player->getItem(player->getCurrentSlot())->addQuantity(-1);
				CSoundEngine::GetInstance()->PlayASound2D("Tilling_Ground");
			}
		}
	}
	//Break blocks
	if (player->GetBreakBlock())
	{
		int x = (player->getcurtool()->GetBlockPlacement().x + scale / 2) / scale;
		int z = (player->getcurtool()->GetBlockPlacement().z + scale / 2) / scale;

		if (world[x][z] == 'c') // Furnace
		{
			world[x][z] = 't';

			for (int i = 0; i < CropList.size(); ++i)
			{
				if (x == CropList[i]->GetXTile() && z == CropList[i]->GetZTile())
				{
					if (CropList[i]->GetState() == 0)
						player->addItem(new Item(Item::ITEM_CARROT, 1));
					else
						player->addItem(new Item(Item::ITEM_CARROT, 3));

					CropList.erase(CropList.begin() + i);
					break;
				}
			}
		}
		else if (world[x][z] == 'w') // Furnace
		{
			world[x][z] = 't';

			for (int i = 0; i < CropList.size(); ++i)
			{
				if (x == CropList[i]->GetXTile() && z == CropList[i]->GetZTile())
				{
					if (CropList[i]->GetState() == 0)
						player->addItem(new Item(Item::ITEM_WHEAT, 1));
					else
						player->addItem(new Item(Item::ITEM_WHEAT, 3));

					CropList.erase(CropList.begin() + i);
					break;
				}
			}
		}


		// Item held
		if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_WOODEN_PICKAXE ||
		player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_STONE_PICKAXE ||
		player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_GOLD_PICKAXE) 
		{
			CSoundEngine::GetInstance()->PlayASound2D("Interaction");

			if (world[x][z] == 'F') // Furnace
			{
				world[x][z] = 'G';
				player->addItem(new Item(Item::ITEM_FURNACE, 1));

				for (int i = 0; i < FurnaceList.size(); ++i)
				{
					if (FurnaceList[i]->GetXTile() == x && FurnaceList[i]->GetZTile() == z)
					{
						//Add items to player inventory 
						player->addItem(new Item(FurnaceList[i]->GetFuelID(), FurnaceList[i]->GetFuelTotal()));
						player->addItem(new Item(FurnaceList[i]->GetResultID(), FurnaceList[i]->GetResultTotal()));
						player->addItem(new Item(FurnaceList[i]->GetSmeltingID(), FurnaceList[i]->GetSmeltingTotal()));

						delete FurnaceList[i];
						break;
					}
				}
			}
			else if (world[x][z] == 'O') // Gold Ore
			{
				world[x][z] = 'G';
				player->addItem(new Item(Item::ITEM_GOLD_NUGGET, 1));
			}
			else if (world[x][z] == 'W') // Ice Ore
			{
				if (SP2_Seasons->getSeason() == 3)
					player->addItem(new Item(Item::ITEM_ICE_CUBE, 1));
			}
			else if (world[x][z] == 'C') // Coal
			{
				world[x][z] = 'G';
				int randVal = Math::RandIntMinMax(1, 3);
				player->addItem(new Item(Item::ITEM_COAL, randVal));
			}
			else
			{

			}
		}
		else if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_WOODEN_AXE ||
			player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_STONE_AXE ||
			player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_GOLD_AXE)
		{
			if (world[x][z] == 'T') // Tree
			{
				if (m_swingcount > player->getcurtool()->GetIntMaxSwings())
				{
					// Tile to transform
					world[x][z] = 'G';
					// Item player receives from breaking tile
					player->addItem(new Item(Item::ITEM_FURNACE, 1));
					m_swingcount = 0;
				}
				m_swingcount++;
			}
			else if (world[x][z] == 'O') // Gold Ore
			{
				world[x][z] = 'G';
				player->addItem(new Item(Item::ITEM_GOLD_NUGGET, 1));
			}
			else if (world[x][z] == 'C') // Coal
			{
				world[x][z] = 'G';
				int randVal = Math::RandIntMinMax(1, 3);
				player->addItem(new Item(Item::ITEM_COAL, randVal));
				player->addItem(new Item(Item::ITEM_WOOD, 1));
			}
			else if (world[x][z] == 'B')
			{
				world[x][z] = 'G';
				player->addItem(new Item(Item::ITEM_BERRY, 3));
			}
		}
	}
		//Update all crops present in the world.
		for (unsigned int i = 0; i < CropList.size(); ++i)
			CropList[i]->update(dt);

		//Update all the furnaces present in the level.
		for (unsigned int i = 0; i < FurnaceList.size(); ++i)
			FurnaceList[i]->update(dt, player);

		//Update all the bushes that have been harvested
		for (unsigned int i = 0; i < HarvestedBushList.size(); ++i)
		{
			HarvestedBushList[i]->Update(dt);

			//After updating , check if the bush is ready to respawn.
			if (HarvestedBushList[i]->GetRespawnYet() == true)
			{
				//If so , set the tile back to a BERRY obj and delete the dead bush from the list.
				world[HarvestedBushList[i]->GetXTile()][HarvestedBushList[i]->GetZTile()] = 'B';
				HarvestedBushList.erase(HarvestedBushList.begin() + i);
			}
		}

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
		EnemyChecker(dt);

		instructiontimer += dt;
		if (instructiontimer > 4)
		{
			instructionorder++;
			instructiontimer = 0;
		}
	}

	glUniform1f(m_parameters[U_FOG_ENABLED], 0);

}
void SP2::EnemyChecker(double dt)
{
	const float m_cfMAXDISTANCE = 200;
	const float m_cfMINDISTANCE = 50;

	float MinX = camera.position.x - 1000;
	float MaxX = camera.position.x + 1000;
	float MinZ = camera.position.z - 1000;
	float MaxZ = camera.position.z + 1000;

	if (m_NumOfEnemy < 1)
		SpawningEnemy();

	m_NumOfEnemy = 0;

	for (std::vector<CEnemy *>::iterator it = m_EnemyList.begin(); it != m_EnemyList.end(); ++it)
	{
		CEnemy *go = (CEnemy *)*it;
		if (go->GetSpawned())
		{
			if (go->GetPosition().x > MinX && go->GetPosition().x < MaxX && go->GetPosition().z > MinZ && go->GetPosition().z < MaxZ)
			{
				go->SetActive(true);
			}
			else
			{
				go->SetActive(false);
			}
			if (go->GetActive())
			{
				if (go->GetCurrentBehaviour() != 4)
				{
					if ((go->GetPosition() - camera.position).Length() < m_cfMAXDISTANCE)
					{
						switch (go->type)
						{
						case CEnemy::ENEMY_TYPE::GO_ZOMBIE:
							if ((go->GetPosition() - camera.position).Length() > m_cfMINDISTANCE)
							{
								go->SetTargetPos(Vector3(camera.position.x, 0, camera.position.z));
								go->SetBehaviour(2);
							}
							else
							{
								go->SetBehaviour(3);
								if (go->GetAttackedPlayer())
								{
									player->SetHP(player->getHP() - go->GetStrength());
									//Knockback
									//camera.position.x += (camera.position.x - go->GetPosition().x);
									//camera.position.z += (camera.position.z - go->GetPosition().z);
								
									//Vector3 viewVector = camera.target - camera.position;
									//camera.target = camera.position - viewVector;
								}
							}
							break;
						case CEnemy::ENEMY_TYPE::GO_WITCH:
							if ((go->GetPosition() - camera.position).Length() > 100)
							{
								go->SetTargetPos(Vector3(camera.position.x, 0, camera.position.z));
								go->SetBehaviour(2);
							}
							else
							{
								go->SetBehaviour(3);
								if (go->GetAttackedPlayer())
								{
									player->SetHP(player->getHP() - go->GetStrength());
								}
							}
							if (go->GetAttackedPlayer())
							{
								//shoot projectile at player
								if (m_iProjectileCount < MAX_PROJECTILE)
								{
									ProjectileObject* Projectile = GetProjectile();
									Projectile->SetType(PROJECTILE_TYPE::P_FIREBALL);
									Projectile->SetScale(Vector3(4, 4, 4));
									Projectile->SetVelocity(Vector3(1, 1, 1));
									Projectile->SetRotationSpeed(Math::RandFloatMinMax(20.f, 40.f));
									Projectile->SetPos(Vector3(go->GetPosition().x, 30, go->GetPosition().z));
									Projectile->SetTargetPos(Vector3(camera.position.x - go->GetPosition().x, 0, camera.position.z - go->GetPosition().z));
									Projectile->SetGotPlayersPos(true);
									Projectile->SetActive(true);
									Projectile->SetTimeTravelled(0.f);

								}
							}
							break;
						default:
							break;
						}
					}
				}
				go->Update(dt, WorldObjectPositionList);
				m_NumOfEnemy++;
			}
		}
	}
}
void SP2::AnimalChecker(double dt)
{
	const float m_cfMAXDISTANCE = 200;
	const float m_cfMINDISTANCE = 50;
	float MinX = camera.position.x - 1000;
	float MaxX = camera.position.x + 1000;
	float MinZ = camera.position.z - 1000;
	float MaxZ = camera.position.z + 1000;

	if (m_NumOfAnimal < 1)
		SpawningAnimal(dt);

	m_NumOfAnimal = 0;
	for (std::vector<CAnimal *>::iterator it = m_AnimalList.begin(); it != m_AnimalList.end(); ++it)
	{
		CAnimal *go = (CAnimal *)*it;

		if (go->GetSpawned())
		{
			if (go->GetPosition().x > MinX && go->GetPosition().x < MaxX && go->GetPosition().z > MinZ && go->GetPosition().z < MaxZ)
			{
				go->SetActive(true);
			}
			else
				go->SetActive(false);

			if (go->GetActive())
			{
				switch (go->type)
				{
				case CAnimal::ANIMAL_TYPE::GO_PIG:
					if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_CARROT)
					{
						if ((go->GetPosition() - camera.position).Length() < m_cfMAXDISTANCE)
						{
							if ((go->GetPosition() - camera.position).Length() > m_cfMINDISTANCE)
							{
								go->SetTargetPos(Vector3(camera.position.x , 0, camera.position.z));
								go->SetBehaviour(2);
							}
							else
							{
								go->SetBehaviour(3);
							}
						}
					}
					else
						//If player switches item, have a cooldown system before they follow the player again.
						if (go->GetCurrentBehaviour() == CAnimal::Behaviour::WANTFOOD)
							go->SetDistracted(true);
					break;
				case CAnimal::ANIMAL_TYPE::GO_CHICKEN:
					if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_SEED)
					{
						if ((go->GetPosition() - camera.position).Length() < m_cfMAXDISTANCE)
						{
							if ((go->GetPosition() - camera.position).Length() > m_cfMINDISTANCE)
							{
								go->SetTargetPos(Vector3(camera.position.x , 0, camera.position.z));
								go->SetBehaviour(2);
							}
							else
							{
								go->SetBehaviour(3);
							}
						}
					}
					else
						//If player switches item, have a cooldown system before they follow the player again.
						if (go->GetCurrentBehaviour() == CAnimal::Behaviour::WANTFOOD)
							go->SetDistracted(true);
					break;
				case CAnimal::ANIMAL_TYPE::GO_COW:
					if (player->getItem(player->getCurrentSlot())->getID() == Item::ITEM_WHEAT)
					{
						if ((go->GetPosition() - camera.position).Length() < m_cfMAXDISTANCE)
						{
							if ((go->GetPosition() - camera.position).Length() > m_cfMINDISTANCE)
							{
								go->SetTargetPos(Vector3(camera.position.x, 0, camera.position.z));
								go->SetBehaviour(2);
							}
							else
							{
								go->SetBehaviour(3);
							}
						}
					}
					else
						//If player switches item, have a cooldown system before they follow the player again.
						if (go->GetCurrentBehaviour() == CAnimal::Behaviour::WANTFOOD)
							go->SetDistracted(true);
					break;
				default:
					break;
				}
				if (go->GetCurrentBehaviour() == CAnimal::Behaviour::FOLLOWING && (go->GetPosition() - camera.position).Length() > m_cfMAXDISTANCE)
				{
					go->SetDistracted(true);
					go->SetBehaviour(3);
				}
				go->Update(dt, WorldObjectPositionList);
				m_NumOfAnimal++;

				//Breeding
				if (go->GetFed() && !go->GetBreeded())
					for (std::vector<CAnimal *>::iterator it2 = m_AnimalList.begin(); it2 != m_AnimalList.end(); ++it2)
					{
						CAnimal *go2 = (CAnimal *)*it2;
						if (go2 != go && go2->type == go->type)
							if (go2->GetActive())
								if (go2->GetFed() && !go2->GetBreeded())
								{
									if ((go->GetPosition() - go2->GetPosition()).Length() < 5)
									{
										CAnimal *go3 = AnimalFetchGO();
										go3->type = go->type;
										go3->SetPosition((go->GetPosition() + go2->GetPosition()) * 0.5f);
										go3->SetTargetPos(go->GetPosition());
										go3->SetSpawned(true);
										go3->SetIsBaby(true);
										go3->SetScale(Vector3(3, 3, 3));
										go->SetBreeded(true);
										go2->SetBreeded(true);
										break;
									}
									else
									{
										go->SetTargetPos((go->GetPosition() + go2->GetPosition()) * 0.5f);
										go->SetBehaviour(1);
										go2->SetTargetPos((go->GetPosition() + go2->GetPosition()) * 0.5f);
										go2->SetBehaviour(1);

									}
								}
					}
			}
		}
	}
}

CEnemy* SP2::EnemyFetchGO()
{
	for (auto go : m_EnemyList)
	{
		if (!go->GetSpawned())
		{
			go->SetSpawned(true);
			return go;
		}
	}
	for (unsigned int i = 0; i < 5; ++i)
	{
		CEnemy *go = new CEnemy(CEnemy::GO_ZOMBIE);
		m_EnemyList.push_back(go);
	}

	CEnemy *go = m_EnemyList.back();
	go->SetSpawned(true);
	return go;
}

CAnimal* SP2::AnimalFetchGO()
{
	for (auto go : m_AnimalList)
	{
		if (!go->GetSpawned())
		{
			go->SetSpawned(true);
			return go;
		}
	}
	for (unsigned int i = 0; i < 5; ++i)
	{
		CAnimal *go = new CAnimal(CAnimal::GO_PIG);
		m_AnimalList.push_back(go);
	}

	CAnimal *go = m_AnimalList.back();
	go->SetSpawned(true);
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
					modelStack.Translate(static_cast<float>(CropList[i]->GetXTile() * scale - 25 + j * 25), 25, static_cast<float>(CropList[i]->GetZTile() * scale - 25 + k * 25));
					modelStack.Scale(25, 50, 25);
					modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (CropList[i]->GetXTile()  * scale  - 25 + j * 25), camera.position.z - (CropList[i]->GetZTile() * scale - 25 + k * 25))), 0, 1, 0);
					RenderMesh(meshList[GEO_SPROUT_CROP], false);
					modelStack.PopMatrix();
				}
				else
				{
					if (CropList[i]->GetCropType() == 0)
					{
						modelStack.PushMatrix();
						modelStack.Translate(static_cast<float>(CropList[i]->GetXTile()  * scale - 25 + j * 25), 25, static_cast<float>( CropList[i]->GetZTile() * scale - 25 + k * 25));
						modelStack.Scale(25, 50, 25);
						modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (CropList[i]->GetXTile() * scale - 25 + j * 25), camera.position.z - (CropList[i]->GetZTile() * scale - 25 + k * 25))), 0, 1, 0);
						RenderMesh(meshList[GEO_CARROT_CROP], false);
						modelStack.PopMatrix();
					}
					else
					{
						modelStack.PushMatrix();
						modelStack.Translate(static_cast<float>(CropList[i]->GetXTile()  * scale - 25 + j * 25), 25, static_cast<float>(CropList[i]->GetZTile() * scale - 25 + k * 25));
						modelStack.Scale(25, 50, 25);
						modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - (CropList[i]->GetXTile()  * scale - 25 + j * 25), camera.position.z - (CropList[i]->GetZTile() * scale - 25 + k * 25))), 0, 1, 0);
						RenderMesh(meshList[GEO_WHEAT_CROP], false);
						modelStack.PopMatrix();
					}
				}
			}
		}
	}
}

void SP2::SpawningEnemy()
{
	// each tile is a scale of x. load 50 blocks. aka 50 * x outwards.
	for (int i = minOutwardsFromPlayerX; i < maxOutwardsFromPlayerX; ++i)
	{
		if (i >= 0 && i <= 250)
		{
			for (int k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 250)
				{
					if (world[i][k] == 'd')
					{
						int choice = Math::RandIntMinMax(0, 20);
						if (choice == 1 && (Vector3(0 + i * scale, 0, 0 + k * scale) - camera.position).Length() > 100) //spawn in if it is 1
						{
							CEnemy *go = EnemyFetchGO();
							go->type = CEnemy::GO_WITCH;
							go->SetPosition(Vector3(0 + i * scale, 0, 0 + k * scale));
							go->SetTargetPos(Vector3(Math::RandFloatMinMax(go->GetPosition().x - 400.f, go->GetPosition().x + 400.f), 0, Math::RandFloatMinMax(go->GetPosition().z - 400.f, go->GetPosition().z + 400.f)));
							go->SetSpawned(true);
							if (m_iDayNight == -1)
							{
								go->SetStrength(go->GetStrength() * 1.5f);
								go->SetHP(go->GetHP() * 1.5f);
								go->SetSpeed(Math::RandFloatMinMax(0.1f, 0.5f) * 1.5f);
							}
							if (go->GetPosition() == camera.position)
								go->SetSpawned(false);
						}
					}
					if (world[i][k] == 'd' || world[i][k] == 'G')
					{
						int choice = Math::RandIntMinMax(0, 20);
						if (choice == 0 && (Vector3(0 + i * scale, 0, 0 + k * scale) - camera.position).Length() > 100) //spawn in if it is 1
						{
							CEnemy *go = EnemyFetchGO();
							go->type = CEnemy::GO_ZOMBIE;
							go->SetPosition(Vector3(0 + i * scale, 0, 0 + k * scale));
							go->SetTargetPos(Vector3(Math::RandFloatMinMax(go->GetPosition().x - 400.f, go->GetPosition().x + 400.f), 0, Math::RandFloatMinMax(go->GetPosition().z - 400.f, go->GetPosition().z + 400.f)));
							go->SetSpawned(true);
							if (m_iDayNight == -1)
							{
								go->SetStrength(go->GetStrength() * 1.5f);
								go->SetHP(go->GetHP() * 1.5f);
								go->SetSpeed(Math::RandFloatMinMax(0.1f, 0.5f) * 1.5f);
							}
							if (go->GetPosition() == camera.position)
								go->SetSpawned(false);
						}
					}
				}
			}
		}
	}
}
void SP2::SpawningAnimal(double dt)
{
	// each tile is a scale of x. load 50 blocks. aka 50 * x outwards.
	for (int i = minOutwardsFromPlayerX; i < maxOutwardsFromPlayerX; ++i)
	{
		if (i >= 0 && i <= 250)
		{
			for (int k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 250)
				{
					if (world[i][k] == 'G')
					{
						int choice = Math::RandIntMinMax(0, 20 + 5 * dt);

						if (choice == 1) //spawn in if it is 1
						{

							CAnimal *go = AnimalFetchGO();

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
							go->SetPosition(Vector3(0 + i * scale, 0, 0 + k * scale));
							go->SetTargetPos(Vector3(Math::RandFloatMinMax(go->GetPosition().x - 400.f, go->GetPosition().x + 400.f), 0, Math::RandFloatMinMax(go->GetPosition().z - 400.f, go->GetPosition().z + 400.f)));
							go->SetSpawned(true);
						}
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
			m_bTexChange = true;
		
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::SUMMER)
		{
			glUniform1f(m_parameters[U_FOG_DENSITY], 0.00001f);
			m_bTexChange = true;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::WINTER)
		{
			glUniform1f(m_parameters[U_FOG_DENSITY], 0.0001f);
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
			particle->scale.Set(1, 10, 1);
			particle->vel.Set(1, -100, 1);
			particle->m_gravity.Set(0, -9.8f, 0);
			particle->rotationSpeed = Math::RandFloatMinMax(20.f, 40.f);
			particle->pos.Set(Math::RandFloatMinMax(camera.position.x - 1000, camera.position.x + 1700), 1200.f, Math::RandFloatMinMax(camera.position.z - 1700, camera.position.z + 1700));
			particle->wind = m_wind;
		}
		else if (SP2_Seasons->getSeason() == Season::TYPE_SEASON::SUMMER)
		{
			ParticleObject* particle = GetParticle();
			particle->type = ParticleObject_TYPE::P_LEAF;
			particle->scale.Set(1, 1, 1);
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
		for (std::vector<CAnimal *>::iterator it = m_AnimalList.begin(); it != m_AnimalList.end(); ++it)
		{
			CAnimal *go = (CAnimal *)*it;
			if (go->GetFed() && !go->GetBreeded() && go->GetActive())
			{
				ParticleObject* particle = GetParticle();
				particle->type = ParticleObject_TYPE::P_HEART;
				particle->scale.Set(10, 10, 10);
				particle->vel.Set(1, 1, 1);
				particle->m_gravity.Set(0, 9.8f, 0);
				particle->pos.Set(Math::RandFloatMinMax(go->GetPosition().x - 10, go->GetPosition().x + 10), 10, Math::RandFloatMinMax(go->GetPosition().z - 10, go->GetPosition().z + 10));
			}
		}
	}

	for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
	{
		ParticleObject *particle = (ParticleObject *)*it;
		if (particle->active)
		{
			particle->Update(dt);
			if (particle->type == ParticleObject_TYPE::P_HEART)
			{
				if (particle->pos.y > 50.f)
				{
					particle->active = false;
					m_particleCount--;
				}
			}
			else
			{
				if (particle->pos.y < -1)
				{
					particle->active = false;
					m_particleCount--;
				}
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
	for (int i = 0; i < MAX_PARTICLE; ++i)
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
		modelStack.Translate(animal->GetPosition().x, animal->GetPosition().y, animal->GetPosition().z);
		modelStack.Rotate(animal->GetAngle(), 0, 1, 0);
		modelStack.Scale(animal->GetScale().x, animal->GetScale().y, animal->GetScale().z);
		RenderMesh(meshList[GEO_PIG], true);
		modelStack.PopMatrix();
		break;
	case CAnimal::GO_COW:
		modelStack.PushMatrix();
		modelStack.Translate(animal->GetPosition().x, animal->GetPosition().y, animal->GetPosition().z );
		modelStack.Rotate(animal->GetAngle(), 0, 1, 0);
		modelStack.Scale(animal->GetScale().x, animal->GetScale().y, animal->GetScale().z);
		RenderMesh(meshList[GEO_COW], true);
		modelStack.PopMatrix();

		//for testing collision boxes
		/*modelStack.PushMatrix();
		modelStack.Translate(animal->GetPosition().x, animal->GetPosition().y + 14, animal->GetPosition().z+3);
		modelStack.Rotate(animal->GetAngle(), 0, 1, 0);
		modelStack.Scale(15, 24, 30);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();*/
		//

		break;
	case CAnimal::GO_CHICKEN:
		modelStack.PushMatrix();
		modelStack.Translate(animal->GetPosition().x, animal->GetPosition().y, animal->GetPosition().z);
		modelStack.Rotate(animal->GetAngle(), 0, 1, 0);
		modelStack.Scale(animal->GetScale().x, animal->GetScale().y, animal->GetScale().z);
		RenderMesh(meshList[GEO_CHICKEN], true);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}
void SP2::RenderEnemy(CEnemy* enemy)
{
	switch (enemy->type)
	{
	case CEnemy::GO_ZOMBIE:
		modelStack.PushMatrix();
		modelStack.Translate(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
		modelStack.Rotate(enemy->GetAngle(), 0, 1, 0);
		modelStack.Scale(enemy->GetScale().x, enemy->GetScale().y, enemy->GetScale().z);
		RenderMesh(meshList[GEO_ZOMBIE], true);
		modelStack.PopMatrix();

		/*modelStack.PushMatrix();
		modelStack.Translate(enemy->GetPosition().x, enemy->GetPosition().y + 20, enemy->GetPosition().z);
		modelStack.Rotate(enemy->GetAngle(), 0, 1, 0);
		modelStack.Scale(15, 48, 15);
		RenderMesh(meshList[GEO_CUBE], true);
		modelStack.PopMatrix();*/
		break;
	case CEnemy::GO_WITCH:
		modelStack.PushMatrix();
		modelStack.Translate(enemy->GetPosition().x, enemy->GetPosition().y, enemy->GetPosition().z);
		modelStack.Rotate(enemy->GetAngle(), 0, 1, 0);
		modelStack.Scale(enemy->GetScale().x, enemy->GetScale().y, enemy->GetScale().z);
		RenderMesh(meshList[GEO_WITCH], false);
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
	case ParticleObject_TYPE::P_HEART:
		modelStack.PushMatrix();
		modelStack.Translate(particle->pos.x, particle->pos.y, particle->pos.z);
		modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - particle->pos.x, camera.position.z - particle->pos.z)), 0, 1, 0);
		modelStack.Rotate(particle->rotation, 0, 0, 1);
		modelStack.Scale(particle->scale.x, particle->scale.y, particle->scale.z);
		RenderMesh(meshList[GEO_PARTICLE_HEART], false);
		modelStack.PopMatrix();
		break;
	default:
		break;
	}
}
//Projectile
ProjectileObject* SP2::GetProjectile(void)
{
	for (std::vector<ProjectileObject *>::iterator it = ProjectileList.begin(); it != ProjectileList.end(); ++it)
	{
		ProjectileObject *Projectile = (ProjectileObject *)*it;
		if (!Projectile->GetActive())
		{
			Projectile->SetActive(true);
			m_iProjectileCount++;
			return Projectile;
		}
	}
	for (unsigned i = 0; i < MAX_PROJECTILE; ++i)
	{
		ProjectileObject *Projectile = new ProjectileObject(PROJECTILE_TYPE::P_FIREBALL);
		ProjectileList.push_back(Projectile);
	}

	ProjectileObject *Projectile = ProjectileList.back();
	Projectile->SetActive(true);
	m_iProjectileCount++;
	return Projectile;
}
void SP2::UpdateProjectile(double dt)
{
	for (std::vector<ProjectileObject *>::iterator it = ProjectileList.begin(); it != ProjectileList.end(); ++it)
	{
		ProjectileObject *Projectile = (ProjectileObject *)*it;
		if (Projectile->GetActive())
		{
			Projectile->Update(dt);

			for (std::vector<Vector3*>::iterator it2 = WorldObjectPositionList.begin(); it2 != WorldObjectPositionList.end(); ++it2)
			{
				Vector3* WorldObjectPos = (Vector3*)*it2;

				if (Projectile->GetPos().x > WorldObjectPos->x - 50 && Projectile->GetPos().x < WorldObjectPos->x + 50)
				{
					if (Projectile->GetPos().z > WorldObjectPos->z - 50 && Projectile->GetPos().z < WorldObjectPos->z + 50)
					{
						Projectile->SetActive(false);
						m_iProjectileCount--;
						break;
					}
				}
			}
			if ((Projectile->GetPos() - camera.position).Length() < 10)
			{
				Projectile->SetActive(false);
				m_iProjectileCount--;
				player->SetHP(player->getHP() - 5.f);
				//add knockback
				//camera.position.x += (camera.position.x - Projectile->GetPos().x);
				//camera.position.z += (camera.position.z - Projectile->GetPos().z);
			}
			else if (Projectile->GetTimeTravelled() > 5)
			{
				Projectile->SetActive(false);
				m_iProjectileCount--;
			}
		}

	}
}
void SP2::RenderProjectile(ProjectileObject * Projectile)
{
	switch (Projectile->GetType())
	{
	case PROJECTILE_TYPE::P_FIREBALL:
		modelStack.PushMatrix();
		modelStack.Translate(Projectile->GetPos().x, Projectile->GetPos().y, Projectile->GetPos().z);
		//modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - Projectile->GetPos().x, camera.position.z - Projectile->GetPos().z)), 0, 1, 0);
		modelStack.Scale(Projectile->GetScale().x, Projectile->GetScale().y, Projectile->GetScale().z);
		RenderMesh(meshList[GEO_FIREBALL], false);
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
		characterSpacing.SetToTranslation(i * 0.8f + 0.5f, 0.5f, 0); //1.0f is the spacing of each character, you may change this value
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
				glUniform1i(m_parameters[U_SHADOW_COLOR_TEXTURE_ENABLED
					+ i], 1);
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
	WorldObjectPositionList.clear();
	// each tile is a scale of x. load 50 blocks. aka 50 * x outwards.
	for (int i = minOutwardsFromPlayerX; i < maxOutwardsFromPlayerX; ++i)
	{
		if (i >= 0 && i <= 250)
		{
			for (int k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 250)
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
						WorldObjectPositionList.push_back(new Vector3(i * scale, 0, k * scale));

						break;
					case 'C':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_COAL_ORE], true);
						modelStack.PopMatrix();
						WorldObjectPositionList.push_back(new Vector3(i * scale, 0, k * scale));

						break;
					case 'B':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_BERRY_BUSH], true);
						modelStack.PopMatrix();
						WorldObjectPositionList.push_back(new Vector3(i * scale, 0, k * scale));

						break;
					case 'b':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_NO_BERRY_BUSH], true);
						modelStack.PopMatrix();
						break;
					case 'D':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						RenderMesh(meshList[GEO_WALL], true);
						modelStack.PopMatrix();
						WorldObjectPositionList.push_back(new Vector3(i * scale, 0, k * scale));

						break;
					case 'F':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale / 3, scale / 3, scale / 3);
						RenderMesh(meshList[GEO_FURNACE_BLOCK], true);
						modelStack.PopMatrix();
						WorldObjectPositionList.push_back(new Vector3(i * scale, 0, k * scale));

						break;
					case 'L':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale / 3, scale / 3, scale / 3);
						RenderMesh(meshList[GEO_CHEST_BLOCK], true);
						modelStack.PopMatrix();
						WorldObjectPositionList.push_back(new Vector3(i * scale, 0, k * scale));

						break;
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
		if (i >= 0 && i <= 250)
		{
			for (int k = minOutwardsFromPlayerZ; k < maxOutwardsFromPlayerZ; ++k)
			{
				if (k >= 0 && k <= 250)
				{
					switch (world[i][k])
					{
					case 't':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						RenderMesh(meshList[GEO_TILLED_DIRT], true);
						modelStack.PopMatrix();
						break;
					case 'c':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						RenderMesh(meshList[GEO_TILLED_DIRT], true);
						modelStack.PopMatrix();
						break;
					case 'w':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						RenderMesh(meshList[GEO_TILLED_DIRT], true);
						modelStack.PopMatrix();
						break;
					case 'd':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						RenderMesh(meshList[GEO_STONE_BRICK_FLOOR], true);
						modelStack.PopMatrix();
						break;
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
					case 'L':
						modelStack.PushMatrix();
						modelStack.Translate(0 + i * scale, 0, 0 + k * scale);
						modelStack.Scale(scale, scale, scale);
						modelStack.Rotate(270, 1, 0, 0);
						RenderMesh(meshList[GEO_STONE_BRICK_FLOOR], true);
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

void SP2::RenderInventory()
{
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

		RenderItem(180 + 60 + i * 60, Application::GetWindowHeight() / 2 - 360, 2, 50, 50, player->getItem(i)->getID());
	}
}
void SP2::RenderPlayerInfo()
{
	//Health
	for (int i = 0; i < static_cast<float>(player->getMaxHP()) * 0.1f; ++i)
	{
		if (i < static_cast<int>(player->getHP() * 0.1))
		{
			RenderImageToScreen(meshList[GEO_HEALTH_FULL], false, 40, 40,
				120 + 40 + i * 40, Application::GetWindowHeight() * 0.2f - 20, 0);
		}
		else
		{
			if (static_cast<int>(player->getHP()) % 2 == 1 && i * 10 < static_cast<int>(player->getHP()))
				RenderImageToScreen(meshList[GEO_HEALTH_HALF], false, 40, 40,
					120 + 40 + i * 40, Application::GetWindowHeight() * 0.2f - 20, 0);
			else
				RenderImageToScreen(meshList[GEO_HEALTH_EMPTY], false, 40, 40,
					120 + 40 + i * 40, Application::GetWindowHeight() * 0.2f - 20, 0);
		}
	}
	//Hunger
	for (int i = 0; i < 10; ++i)
	{
		if (i < static_cast<int>(player->getHunger() * 0.1f))
		{
			RenderImageToScreen(meshList[GEO_HUNGER_FULL], false, 40, 40,
				650 + 40 + i * 40, Application::GetWindowHeight() * 0.2f - 40, 0);
		}
		else
		{
			if (static_cast<int>(player->getHunger()) % 2 == 1 && i * 10 < static_cast<int>(player->getHunger()))
				RenderImageToScreen(meshList[GEO_HUNGER_HALF], false, 40, 40,
					650 + 40 + i * 40, Application::GetWindowHeight() * 0.2f - 40, 0);
			else
				RenderImageToScreen(meshList[GEO_HUNGER_EMPTY], false, 40, 40,
					650 + 40 + i * 40, Application::GetWindowHeight() * 0.2f - 40, 0);
		}
	}
	//Thirst
	for (int i = 0; i < 10; ++i)
	{
		if (i < static_cast<int>(player->getThirst() * 0.1f))
		{
			RenderImageToScreen(meshList[GEO_THIRST_FULL], false, 40, 40,
				650 + 40 + i * 40, Application::GetWindowHeight() * 0.2f + 10, 0);
		}
		else
		{
			RenderImageToScreen(meshList[GEO_THIRST_EMPTY], false, 40, 40,
					650 + 40 + i * 40, Application::GetWindowHeight() * 0.2f + 10, 0);
		}
	}
}
void SP2::RenderWorld()
{
	modelStack.PushMatrix();
	modelStack.Translate(12500, 0, 12500);
	modelStack.Scale(25150, 250, 25150);
	RenderMesh(meshList[GEO_CASTLE], false);
	modelStack.PopMatrix();

	if (m_bMenu == false)
	{
		for (int i = 0; i < player->getTotalDropItems(); ++i)
		{
			modelStack.PushMatrix();
			modelStack.Translate(player->getDroppedItem(i)->getXPos(), 10, player->getDroppedItem(i)->getZPos());
			modelStack.Rotate(m_fConstantRotate, 0, 1, 0);
			modelStack.Scale(10, 10, 10);
			RenderMesh(meshList[GEO_ITEMS_START + player->getDroppedItem(i)->getID()], false);
			modelStack.PopMatrix();
		}

		modelStack.PushMatrix();
		modelStack.Translate(12500, 0, 12500);
		modelStack.Scale(6, 6, 6);
		RenderMesh(meshList[GEO_PLAYER], false);
		modelStack.PopMatrix();

		for (int i = 0; i < 15; ++i)
		{
			//Interval between drawing of each line that makes up the lightning
			if (m_fTimeTillLightning >= i * 0.008)
			{
				modelStack.PushMatrix();
				modelStack.Translate(lightningX + 50, 1440 - i * 96, lightningZ);
				modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - lightningX, camera.position.z - lightningZ)), 0, 1, 0);
				if (i % 2 == 0)
					modelStack.Rotate(15, 0, 0, 1);
				else
					modelStack.Rotate(345, 0, 0, 1);
				modelStack.Scale(1, 100, 1);
				RenderMesh(meshList[GEO_LIGHTNING], false);
				modelStack.PopMatrix();
			}
		}

		RenderGroundObjects();

		//Render Animals
		for (std::vector<CAnimal *>::iterator it = m_AnimalList.begin(); it != m_AnimalList.end(); ++it)
		{
			CAnimal *animal = (CAnimal *)*it;
			if (animal->GetActive())
			{
				RenderAnimal(animal);
			}
		}
		//Render Enemys
		for (std::vector<CEnemy *>::iterator it = m_EnemyList.begin(); it != m_EnemyList.end(); ++it)
		{
			CEnemy *enemy = (CEnemy *)*it;
			if (enemy->GetActive())
			{
				RenderEnemy(enemy);
			}
		}
	}
}

void SP2::RenderSkyBox()
{
	modelStack.PushMatrix();
	modelStack.Translate(camera.position.x, camera.position.y, camera.position.z);
	modelStack.Scale(3250, 3250, 3250);

	switch (SP2_Seasons->getSeason())
	{
	case Season::TYPE_SEASON::SPRING:
		RenderMesh(meshList[GEO_SKYBOX_SPRING], false);
		break;
	case Season::TYPE_SEASON::SUMMER:
		RenderMesh(meshList[GEO_SKYBOX_SUMMER], false);
		break;
	case Season::TYPE_SEASON::WINTER:
		RenderMesh(meshList[GEO_SKYBOX_WINTER], false);
		break;
	default:
		break;
	}
	modelStack.PopMatrix();
}

void SP2::Render3DHandHeld()
{
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

	for (int i = 0; i < 2; ++i)
	{
		if (lights[i].type == Light::LIGHT_DIRECTIONAL)
		{
			Vector3 lightDir(lights[i].position.x, lights[i].position.y, lights[i].position.z);
			Vector3 lightDirection_cameraspace = viewStack.Top() * lightDir;
			glUniform3fv(m_parameters[U_LIGHT0_POSITION + i * 11], 1, &lightDirection_cameraspace.x);
		}
		else if (lights[1].type == Light::LIGHT_SPOT)
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION + i * 11], 1, &lightPosition_cameraspace.x);
			Vector3 spotDirection_cameraspace = viewStack.Top() * lights[1].spotDirection;
			glUniform3fv(m_parameters[U_LIGHT1_SPOTDIRECTION + i * 11], 1, &spotDirection_cameraspace.x);
		}
		else
		{
			Position lightPosition_cameraspace = viewStack.Top() * lights[1].position;
			glUniform3fv(m_parameters[U_LIGHT1_POSITION + i * 11], 1, &lightPosition_cameraspace.x);
		}
	}
	
	if (m_bMenu == true)
	{
		if (m_bContinue == false)
			RenderImageToScreen(meshList[GEO_PLAYER_SELECTION], false, Application::GetWindowWidth(), Application::GetWindowHeight(), Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2, 1);
		else
			RenderImageToScreen(meshList[GEO_MENU], false, Application::GetWindowWidth(), Application::GetWindowHeight(), Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2, 1);
		RenderImageToScreen(meshList[GEO_NAVIGATOR], false, Application::GetWindowWidth() / 10, Application::GetWindowHeight() / 10 , Application::GetWindowWidth() / 2 - 400 , Application::GetWindowHeight() / 2 + m_fNavigatorY, 2);
	}
	
	if (m_bMenu == false)
	{
		if (player->getHP() <= 0)
		{
			RenderImageToScreen(meshList[GEO_GAME_OVER], false, Application::GetWindowWidth(), Application::GetWindowHeight(), Application::GetWindowWidth() / 2, Application::GetWindowHeight() / 2, 1);
		}
		else
		{
			RenderMesh(meshList[GEO_AXES], false);
			RenderMeshIn2D(meshList[GEO_CROSSHAIR], false, 1.0f);

			RenderCrafting();
			RenderFurnace();

			Render3DHandHeld();
			RenderInventory();

			RenderSkyBox();
			RenderGround();

			modelStack.PushMatrix();
			modelStack.Translate(12550, 10, 12550);
			modelStack.Scale(10, 10, 10);
			modelStack.Rotate(Math::RadianToDegree(atan2(camera.position.x - 12550, camera.position.z - 12550)), 0, 1, 0);
			RenderMesh(meshList[GEO_LIGHT_DEPTH_QUAD], false);
			modelStack.PopMatrix();

			RenderWorld();

			RenderAnimation();

			//	Render Particles
			for (std::vector<ParticleObject *>::iterator it = particleList.begin(); it != particleList.end(); ++it)
			{
				ParticleObject *particle = (ParticleObject *)*it;
				if (particle->active)
				{
					RenderParticles(particle);
				}
			}
			for (std::vector<ProjectileObject *>::iterator it = ProjectileList.begin(); it != ProjectileList.end(); ++it)
			{
				ProjectileObject *Projectile = (ProjectileObject *)*it;
				if (Projectile->GetActive())
				{
					RenderProjectile(Projectile);
				}
			}
			RenderCrops();

			RenderPlayerInfo();
	if (instructionorder < 4)
	RenderInstructions();

			std::ostringstream ss;
			ss.precision(5);
			ss << "FPS: " << fps;
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 50, -600, 300);

			ss.str("");
			ss << std::to_string(player->getItem(player->getCurrentSlot())->getQuantity());
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 50, -20, -300);

			ss.str("");
			ss << std::to_string(int((camera.position.x + scale / 2) / scale)) << " " << std::to_string(int((camera.position.z + scale / 2) / scale));
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 50, -600, 200);

			ss.str("");
			ss << std::to_string(m_swingcount) << "/" << std::to_string(player->getcurtool()->GetIntMaxSwings());
			RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 50, -600, 100);
		}
	}
}
void SP2::RenderInstructions()
{
	std::ostringstream ss;
	//Instructions
	switch (instructionorder)
	{
	case 0:
		RenderImageToScreen(meshList[GEO_INSTRUCTION1], false, Application::GetWindowWidth() * 0.7f, Application::GetWindowWidth() * 0.5f,
			Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f + 100, 0);
		break;
	case 1:
		RenderImageToScreen(meshList[GEO_INSTRUCTION2], false, Application::GetWindowWidth() * 0.7f, Application::GetWindowWidth() * 0.5f,
			Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f + 100, 0);
		break;
	case 2:
		RenderImageToScreen(meshList[GEO_INSTRUCTION3], false, Application::GetWindowWidth() * 0.7f, Application::GetWindowWidth() * 0.5f,
			Application::GetWindowWidth() * 0.5f, Application::GetWindowHeight() * 0.5f + 100, 0);
		break;
	case 3:
		ss << "Becareful while venturing!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 50, -500, 150);
		ss.str("");
		ss << "Have Fun!";
		RenderTextOnScreen(meshList[GEO_TEXT], ss.str(), Color(0, 1, 0), 50, -500, 100);
		break;
	default:
		break;
	}
}
void SP2::Render()
{
	RenderPassGPass();
	RenderPassMain();
}

void SP2::SaveAnimalData()
{
	std::ofstream saveFile("AnimalSaveFile.txt");

	if (saveFile.is_open())
	{
		for (int i = 0; i < m_AnimalList.size(); ++i)
		{
			if (m_AnimalList[i]->GetActive())
			{
				saveFile << m_AnimalList[i]->m_iAnimalType << std::endl;

				saveFile << m_AnimalList[i]->GetPosition().x << std::endl;
				saveFile << m_AnimalList[i]->GetPosition().y << std::endl;
				saveFile << m_AnimalList[i]->GetPosition().z << std::endl;

				saveFile << m_AnimalList[i]->GetScale().x << std::endl;
				saveFile << m_AnimalList[i]->GetScale().y << std::endl;
				saveFile << m_AnimalList[i]->GetScale().z << std::endl;

				saveFile << m_AnimalList[i]->GetSpawned() << std::endl;
				saveFile << m_AnimalList[i]->GetIsABaby() << std::endl;
				saveFile << m_AnimalList[i]->GetGrowUpTimer() << std::endl;
			}
		}
		saveFile.close();
	}
	else
	{
		std::cout << " cant save !" << std::endl;
	}
}
void SP2::LoadAnimalData()
{
	std::ifstream saveFile("AnimalSaveFile.txt"); //Open text file to read
	int animalType;
	
	float posX;
	float posY;
	float posZ;

	float scaleX;
	float scaleY;
	float scaleZ;

	bool spawned;
	bool baby;
	double timer;

	if (saveFile.is_open())
	{
		while (!saveFile.eof())
		{
			saveFile >> animalType;

			saveFile >> posX;
			saveFile >> posY;
			saveFile >> posZ;

			saveFile >> scaleX;
			saveFile >> scaleY;
			saveFile >> scaleZ;

			saveFile >> spawned;
			saveFile >> baby;
			saveFile >> timer;
			m_AnimalList.push_back(new CAnimal(animalType, Vector3(posX, posY, posZ), Vector3(scaleX, scaleY, scaleZ), spawned, baby, timer));
		}
		saveFile.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;
}


void SP2::SaveEnemyData()
{
	std::ofstream saveFile("EnemySaveFile.txt");

	if (saveFile.is_open())
	{
		for (int i = 0; i < m_EnemyList.size(); ++i)
		{
			if (m_EnemyList[i]->GetActive())
			{
				saveFile << m_EnemyList[i]->type << std::endl;
				saveFile << m_EnemyList[i]->GetPosition().x << std::endl;
				saveFile << m_EnemyList[i]->GetPosition().y << std::endl;
				saveFile << m_EnemyList[i]->GetPosition().z << std::endl;
				saveFile << m_EnemyList[i]->GetHP() << std::endl;
				saveFile << m_EnemyList[i]->GetStrength() << std::endl;
				saveFile << m_EnemyList[i]->GetSpawned() << std::endl;
			}
		}
		saveFile.close();
	}
	else
	{
		std::cout << " cant save !" << std::endl;
	}
}
void SP2::LoadEnemyData()
{
	std::ifstream saveFile("EnemySaveFile.txt"); //Open text file to read
	int enemyType;
	float posX;
	float posY;
	float posZ;
	float HP;
	float Strength;
	bool spawned;

	if (saveFile.is_open())
	{
		while (!saveFile.eof())
		{
			saveFile >> enemyType;
			saveFile >> posX;
			saveFile >> posY;
			saveFile >> posZ;
			saveFile >> HP;
			saveFile >> Strength;
			saveFile >> spawned;
			m_EnemyList.push_back(new CEnemy(enemyType, Vector3(posX, posY, posZ), HP, Strength, spawned));
		}
		saveFile.close();
	}
	else
		std::cout << "Impossible to open save file!" << std::endl;
}
void SP2::Exit()
{
	SaveAnimalData();
	SaveEnemyData();

	// Cleanup VBO
	for (int i = 0; i < NUM_GEOMETRY; ++i)
	{
		if (meshList[i])
			delete meshList[i];
	}

	for (int i = 0; i < CropList.size(); ++i)
	{
		if (CropList[i] != nullptr)
			delete CropList[i];
	}

	for (int i = 0; i < FurnaceList.size(); ++i)
	{
		if (FurnaceList[i] != nullptr)
			delete FurnaceList[i];
	}

	for (int i = 0; i < WorldObjectPositionList.size(); ++i)
	{
		if (WorldObjectPositionList[i])
			delete WorldObjectPositionList[i];
	}
	for (int i = 0; i < m_AnimalList.size(); ++i)
	{
		if (m_AnimalList[i])
			delete m_AnimalList[i];
	}
	for (int i = 0; i < m_EnemyList.size(); ++i)
	{
		if (m_EnemyList[i])
			delete m_EnemyList[i];
	}

	SaveWorld();

	delete player;
	
	glDeleteProgram(m_programID);
	glDeleteVertexArrays(1, &m_vertexArrayID);

	glDeleteProgram(m_gPassShaderID);
}
