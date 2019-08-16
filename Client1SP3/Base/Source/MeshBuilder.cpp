#include "MeshBuilder.h"
#include <GL\glew.h>
#include <vector>
#include "Vertex.h"
#include "MyMath.h"
#include "LoadOBJ.h"
#include <iostream>
using namespace std;

SpriteAnimation* MeshBuilder::GenerateSpriteAnimation(const std::string&meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for (unsigned i = 0; i < numRow; ++i)
	{
		for (unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);

			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}

	SpriteAnimation *mesh = new SpriteAnimation(meshName, numRow, numCol);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}
Vector3 getV(std::vector<Vertex> &vertex_buffer_data, long int x, long int z, long int chungus)
{
	long int index = x + z * chungus;
	Vector3 buff = Vector3(vertex_buffer_data[index].pos.x, vertex_buffer_data[index].pos.y, vertex_buffer_data[index].pos.z);
	return buff;
}
Mesh* MeshBuilder::GenerateTerrain(const std::string &meshName, const std::string &file_path, std::vector<unsigned char> &heightMap)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	//~~~~~~~~~~~~~~~~~
	// to be completed

	//Set a scale factor to size your terrain
	const float SCALE_FACTOR = 256.0f; //Use for normalising (0-255 RAW file)
	const unsigned tile = 8;
	//Load/ Read information from the heightmap
	if (!LoadHeightMap(file_path.c_str(), heightMap))
		return NULL;
	//calculate the terrainSize
	unsigned terrainSize = (unsigned)sqrt((double)heightMap.size());
	//Based on heightmap, calculate a scaledHeight
	for (unsigned z = 0; z < terrainSize; ++z)
	{
		for (unsigned x = 0; x < terrainSize; ++x)
		{
			float scaleHeight = (float)heightMap[z * terrainSize + x] / SCALE_FACTOR;

			v.pos.Set(static_cast<float>(x) / terrainSize - 0.5f, scaleHeight, static_cast<float>(z) / terrainSize - 0.5f);
			v.color.Set(scaleHeight, scaleHeight, scaleHeight);
			v.texCoord.Set(static_cast<float>(x) / terrainSize * static_cast<float>(tile), 1.f - static_cast<float>(z) / terrainSize * static_cast<float>(tile));
			//NORMAL 
		//	v.normal.Set(0, 1, 0);
			//Push into Vertex buffer 
			vertex_buffer_data.push_back(v);
		}		
	}
	//Indices
	for (unsigned z = 0; z < terrainSize - 1; ++z)
	{
		for (unsigned x = 0; x < terrainSize - 1; ++x)
		{
			//Tri 1
			index_buffer_data.push_back(z * terrainSize + x);
			index_buffer_data.push_back((z + 1) * terrainSize + x);
			index_buffer_data.push_back(z * terrainSize + (x + 1));
			//Tri 2
			index_buffer_data.push_back((z + 1) * terrainSize + (x + 1));		
			index_buffer_data.push_back(z * terrainSize + (x + 1));
			index_buffer_data.push_back((z + 1) * terrainSize + x);
		}
	}
	//calculate x, y and z of each vertices
	//Set color or normal if necessary
	for (unsigned int z = 0; z < terrainSize; z++)
	{
		for (unsigned int x = 0; x < terrainSize; x++)
		{
			Vector3 sum(0.f, 0.f, 0.f);
			Vector3 C = getV(vertex_buffer_data, z, x, terrainSize);
			Vector3 out(0.f, 0.f, 0.f);
			Vector3 in(0.f, 0.f, 0.f);
			Vector3 left(0.f, 0.f, 0.f);
			Vector3 right(0.f, 0.f, 0.f);
			Vector3 buff;
			if (z > 0)
			{
				buff = getV(vertex_buffer_data, x, z - 1, terrainSize) - C;
				out = Vector3(0.f, buff.y, -1.f);
			}
			if (z < terrainSize - 1)
			{
				buff = getV(vertex_buffer_data, x, z + 1, terrainSize) - C;
				in = Vector3(0.f, buff.y, 1.f);
			}
			if (x > 0)
			{
				buff = getV(vertex_buffer_data, x - 1, z, terrainSize) - C;
				left = Vector3(-1.f, buff.y, 0.f);
			}
			if (x < terrainSize - 1) {
				buff = getV(vertex_buffer_data, x + 1, z, terrainSize) - C;
				right = Vector3(1.0f, buff.y, 0.0f);
			}
			if (x > 0 && z > 0)
			{
				sum += out.Cross(left).Normalized();
			}
			if (x > 0 && z < terrainSize - 1)
			{
				sum += left.Cross(in).Normalized();
			}
			if (x < terrainSize - 1 && z < terrainSize - 1)
			{
				sum += in.Cross(right).Normalized();
			}
			if (x < terrainSize - 1 && z > 0)
			{
				sum += right.Cross(out).Normalized();
			}
			vertex_buffer_data[z * terrainSize + x].normal = sum;
		}
	}
	//Calculate the vertex coordintes
	//Assign to vertex buffer

	//Calculate the indices
	//~~~~~~~~~~~~~~~~~~~~~
	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);
	
	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES; //not trianglestrip

	return mesh;
}
Mesh* MeshBuilder::GenerateSkyPlane(const std::string &meshName, Color color, int slices, float PlanetRadius, float AtmosphereRadius, float hTile, float vTile)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;

	// write sky plane code here
	slices = Math::Clamp(slices, 1, 256); // limit between 1-256

	float planeSize = 2.0f * (float)sqrtf((AtmosphereRadius * AtmosphereRadius) + (PlanetRadius * PlanetRadius)); // get the size of the whole plane

																												  // get values for travelling
	float delta = planeSize / (float)slices;
	float texDelta = 2.0f / (float)slices;

	// get the vertices of the planes
	// tranverse the Z plane
	for (int z = 0; z <= slices; ++z)
	{
		// tranverse the X plane
		for (int x = 0; x <= slices; ++x)
		{
			// the X - Z position based on the current X - Z value
			float xDist = (-0.5f * planeSize) + ((float)x * delta);
			float zDist = (-0.5f * planeSize) + ((float)z * delta);

			// get the height
			float xHeight = (xDist * xDist) / AtmosphereRadius;
			float zHeight = (zDist * zDist) / AtmosphereRadius;
			float height = xHeight + zHeight;

			Vertex v;
			v.pos.x = xDist;
			v.pos.y = 0.0f - height;
			v.pos.z = zDist;

			// vertice UV
			v.texCoord.u = hTile * ((float)x * texDelta * 0.5f);
			v.texCoord.v = vTile * (1.0f - (float)z * texDelta * 0.5f);
			// color
			v.color = color;

			// push vertex in vertex buffer array
			vertex_buffer_data.push_back(v);
		}
	
	}

	// calculate the indices
	int index = 0;
	for (int i = 0; i < slices; ++i)
	{
		for (int j = 0; j < slices; ++j)
		{
			// triangle 1
			int startVert = (i * (slices + 1) + j);
			index_buffer_data.push_back(startVert);
			index_buffer_data.push_back(startVert + 1);
			index_buffer_data.push_back(startVert + slices + 1);
			// triangle 2
			index_buffer_data.push_back(startVert + 1);
			index_buffer_data.push_back(startVert + slices + 2);
			index_buffer_data.push_back(startVert + slices + 1);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES; //not trianglestrip

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a reference Axes; Use red for x-axis, green for y-axis, blue for z-axis
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateAxes(const std::string &meshName, float lengthX, float lengthY, float lengthZ)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	v.pos.Set(-1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(1000, 0, 0);
	v.color.Set(1, 0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, -1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 1000, 0);
	v.color.Set(0, 1, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, -1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0, 0, 1000);
	v.color.Set(0, 0, 1);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(5);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a crosshair;
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - x-axis should start at -lengthX / 2 and end at lengthX / 2
\param lengthY - y-axis should start at -lengthY / 2 and end at lengthY / 2
\param lengthZ - z-axis should start at -lengthZ / 2 and end at lengthZ / 2

\return Pointer to mesh storing VBO/IBO of reference axes
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCrossHair(const std::string &meshName, float colour_r, float colour_g, float colour_b, float length)
{
	// Declare the variable to store a vertex and the buffer for storing vertices
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	// Vertex #1
	v.pos.Set(-length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #2
	v.pos.Set(length, 0, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #3
	v.pos.Set(0, -length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);
	// Vertex #4
	v.pos.Set(0, length, 0);
	v.color.Set(colour_r, colour_g, colour_b);
	vertex_buffer_data.push_back(v);

	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(3);

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_LINES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a quad; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of quad
\param lengthY - height of quad

\return Pointer to mesh storing VBO/IBO of quad
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateQuad(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	v.pos.Set(-0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 0);
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(1.0f, 1.0f);
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,0);
	v.color = color;
	v.normal.Set(0, 0, 1);
	v.texCoord.Set(0, 1.0f);
	vertex_buffer_data.push_back(v);
	
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(0);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

/******************************************************************************/
/*!
\brief
Generate the vertices of a cube; Use random color for each vertex
Then generate the VBO/IBO and store them in Mesh object

\param meshName - name of mesh
\param lengthX - width of cube
\param lengthY - height of cube
\param lengthZ - depth of cube

\return Pointer to mesh storing VBO/IBO of cube
*/
/******************************************************************************/
Mesh* MeshBuilder::GenerateCube(const std::string &meshName, Color color, float length)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;

	v.pos.Set(-0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length,-0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length,-0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	v.pos.Set(-0.5f * length, 0.5f * length, 0.5f * length);
	v.color = color;
	vertex_buffer_data.push_back(v);
	
	std::vector<GLuint> index_buffer_data;
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(6);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(2);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(3);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(7);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(1);
	index_buffer_data.push_back(5);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(4);
	index_buffer_data.push_back(0);
	index_buffer_data.push_back(5);
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = 36;
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

Mesh* MeshBuilder::GenerateRing(const std::string &meshName, Color color, unsigned numSlice, float outerR, float innerR)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	Vertex v;

	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		float theta = slice * degreePerSlice;
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(outerR * cos(Math::DegreeToRadian(theta)), 0, outerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
		
		v.color = color;
		v.normal.Set(0, 1, 0);
		v.pos.Set(innerR * cos(Math::DegreeToRadian(theta)), 0, innerR * sin(Math::DegreeToRadian(theta)));
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(2 * slice + 0);
		index_buffer_data.push_back(2 * slice + 1);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 4);
		//index_buffer_data.push_back(2 * slice + 3);
		//index_buffer_data.push_back(2 * slice + 2);
	}

	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

float sphereX(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * cos(Math::DegreeToRadian(theta));
}
float sphereY(float phi, float theta)
{
	return sin(Math::DegreeToRadian(phi));
}
float sphereZ(float phi, float theta)
{
	return cos(Math::DegreeToRadian(phi)) * sin(Math::DegreeToRadian(theta));
}

Mesh* MeshBuilder::GenerateSphere(const std::string &meshName, Color color, unsigned numStack, unsigned numSlice, float radius)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float degreePerStack = 180.f / numStack;
	float degreePerSlice = 360.f / numSlice;
	//float radianPerSlice = Math::DegreeToRadian(360.f) / numSlice;

	for(unsigned stack = 0; stack < numStack + 1; ++stack) //stack //replace with 180 for sphere
	{
		float phi = -90.f + stack * degreePerStack;
		for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
		{
			float theta = slice * degreePerSlice;
			Vertex v;
			v.pos.Set(radius * sphereX(phi, theta), radius * sphereY(phi, theta), radius * sphereZ(phi, theta));
			v.color = color;
			v.normal.Set(sphereX(phi, theta), sphereY(phi, theta), sphereZ(phi, theta));
			vertex_buffer_data.push_back(v);
		}
	}
	for(unsigned stack = 0; stack < numStack; ++stack)
	{
		for(unsigned slice = 0; slice < numSlice + 1; ++slice)
		{
			index_buffer_data.push_back((numSlice + 1) * stack + slice + 0);
			index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * stack + slice + 1);
			//index_buffer_data.push_back((numSlice + 1) * (stack + 1) + slice + 0);
		}
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateCone(const std::string &meshName, Color color, unsigned numSlice, float radius, float height)
{
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	Vertex v;
	float degreePerSlice = 360.f / numSlice;

	for(unsigned slice = 0; slice < numSlice + 1; ++slice) //slice
	{
		float theta = slice * degreePerSlice;
		Vector3 normal(height * cos(Math::DegreeToRadian(theta)), radius, height * sin(Math::DegreeToRadian(theta)));
		normal.Normalize();

		v.pos.Set(radius * cos(Math::DegreeToRadian(theta)), 0, radius * sin(Math::DegreeToRadian(theta)));
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
		
		v.pos.Set(0, height, 0);
		v.color = color;
		v.normal = normal;
		vertex_buffer_data.push_back(v);
	}
	for(unsigned slice = 0; slice < numSlice + 1; ++slice)
	{
		index_buffer_data.push_back(slice * 2 + 0);
		index_buffer_data.push_back(slice * 2 + 1);
	}

	Mesh *mesh = new Mesh(meshName);

	mesh->mode = Mesh::DRAW_TRIANGLE_STRIP;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateOBJ(const std::string &meshName, const std::string &file_path)
{
	std::vector<Position> vertices;
	std::vector<TexCoord> uvs;
	std::vector<Vector3> normals;
	bool success = LoadOBJ(file_path.c_str(), vertices, uvs, normals);
	if(!success)
		return NULL;

	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	IndexVBO(vertices, uvs, normals, index_buffer_data, vertex_buffer_data);

	Mesh *mesh = new Mesh(meshName);
	
	mesh->mode = Mesh::DRAW_TRIANGLES;
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();

	return mesh;
}

Mesh* MeshBuilder::GenerateText(const std::string &meshName, unsigned numRow, unsigned numCol)
{
	Vertex v;
	std::vector<Vertex> vertex_buffer_data;
	std::vector<GLuint> index_buffer_data;
	
	float width = 1.f / numCol;
	float height = 1.f / numRow;
	int offset = 0;
	for(unsigned i = 0; i < numRow; ++i)
	{
		for(unsigned j = 0; j < numCol; ++j)
		{
			float u1 = j * width;
			float v1 = 1.f - height - i * height;
			v.pos.Set(-0.5f, -0.5f, 0);
			v.texCoord.Set(u1, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, -0.5f, 0);
			v.texCoord.Set(u1 + width, v1);
			vertex_buffer_data.push_back(v);

			v.pos.Set(0.5f, 0.5f, 0);
			v.texCoord.Set(u1 + width, v1 + height);
			vertex_buffer_data.push_back(v);

			v.pos.Set(-0.5f, 0.5f, 0);
			v.texCoord.Set(u1, v1 + height);
			vertex_buffer_data.push_back(v);
	
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 1);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 0);
			index_buffer_data.push_back(offset + 2);
			index_buffer_data.push_back(offset + 3);
			offset += 4;
		}
	}
	
	Mesh *mesh = new Mesh(meshName);
	
	glBindBuffer(GL_ARRAY_BUFFER, mesh->vertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, vertex_buffer_data.size() * sizeof(Vertex), &vertex_buffer_data[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->indexBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_buffer_data.size() * sizeof(GLuint), &index_buffer_data[0], GL_STATIC_DRAW);

	mesh->indexSize = index_buffer_data.size();
	mesh->mode = Mesh::DRAW_TRIANGLES;

	return mesh;
}

