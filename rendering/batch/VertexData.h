#ifndef VERTEX_DATA_H
#define VERTEX_DATA_H

#include "../../pch.h"

struct VertexData
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::mat4 ModelTransform;
	glm::vec2 TexCoord;
	float TexDiffID;
	float TexSpecID; 
	unsigned int Lighting;
	unsigned int Color;
};

struct VertexDataOutline
{
	glm::vec4 Position;
	unsigned int Color;
};

struct VertexDataParticle
{
	glm::vec4 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord; //Cone Rim, Angle - Random offsets 
	glm::vec4 ParticleData; //Life, Speed, Size, MAX_Rotation; 
	unsigned int Color; 
};

struct VertexDataDepthmap
{
	glm::vec4 Position;
};

struct VertexDataInstancedFixed
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoord;
};

struct VertexDataInstancedVariable
{
	glm::mat4 ModelTransform;
	glm::vec2 TexScale;
	float TexDiffID;
	float TexSpecID; 
	unsigned int Lighting;
	unsigned int Color;
};

#endif //VERTEX_DATA_H