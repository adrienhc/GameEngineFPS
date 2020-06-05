#ifndef MESH_H
#define MESH_H

#include "../pch.h"
#include "../rendering/shader.h"

struct mVertex
{
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 TexCoords;
	//glm::vec3 Tangent;
	//glm::vec3 Bitangent;
};

struct mTexture
{
	unsigned int id;
	std::string type;	
	std::string path;
};

class Mesh
{
public:
	Mesh(std::vector<mVertex> vert, std::vector<unsigned int> ind, std::vector<mTexture> tex);
	~Mesh();
	std::vector<mVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<mTexture> textures;
	
	void Draw(Shader shader);

private:
	unsigned int VAO;
	unsigned int VBO;
	unsigned int EBO;

	void setupMesh();
};


#endif //MESH_H