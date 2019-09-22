#ifndef MODEL_H
#define MODEL_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../loader/stb_image.h"

#include "mesh.h"
#include "../rendering/shader.h"

class Model
{
	public:
		std::vector<Mesh> meshes;
		std::string directory;

		std::vector<mTexture> textures_loaded;
		bool gammaCorrection;

		Model(char* path, bool gamma = false);
		void Draw(Shader shader);

	private:
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<mTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int loadTextureFromFile(const char* path, const std::string &directory, bool gamma = false);

};

#endif //MODEL_H