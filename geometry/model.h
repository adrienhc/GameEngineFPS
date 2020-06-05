#ifndef MODEL_H
#define MODEL_H

#include "../pch.h"
#include "../lib/stb_image.h"
#include "mesh.h"
#include "bounding_box.h"
#include "../rendering/shader.h"



class Model
{
	public:
		std::vector<Mesh> meshes;
		std::string directory;

		std::vector<mTexture> textures_loaded;
		bool gammaCorrection;

		Model(char* path, bool gamma = false);
		~Model();
		void Draw(Shader shader);
		void cullingON();
		void cullingOFF();
		bool getCullingStatus();
		BB getBoundingBox(glm::mat4 &model_transform);

	private:
		void loadModel(std::string path);
		void processNode(aiNode* node, const aiScene* scene);
		Mesh processMesh(aiMesh* mesh, const aiScene* scene);
		std::vector<mTexture> loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName);
		unsigned int loadTextureFromFile(const char* path, const std::string &directory, bool gamma = false);

		glm::vec4 min_bb;
		glm::vec4 max_bb;

		bool culling;

};

#endif //MODEL_H