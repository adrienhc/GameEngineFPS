#include "model.h"

//#define STB_IMAGE_IMPLEMENTATION


Model::Model(char* path, bool gamma)
{
	gammaCorrection = gamma;
	culling = true;
	loadModel(path);

	min_bb = glm::vec4(10000.0f, 10000.0f, 10000.0f, 1.0f);
	max_bb = glm::vec4(-10000.0f, -10000.0f, -10000.0f, 1.0f);

	for (int i =0; i < textures_loaded.size(); i++)
		std::cout << textures_loaded[i].path << " " << textures_loaded[i].type << std::endl;

	//Compute Minimum and Maximum possible vertices, for BB
	for(int i = 0; i < meshes.size(); i++)
	{
		for(int j = 0; j < meshes[i].vertices.size(); j++)
		{
			glm::vec3 p = meshes[i].vertices[j].Position;
			
			min_bb.x = min_bb.x < p.x ? min_bb.x : p.x;
			min_bb.y = min_bb.y < p.y ? min_bb.y : p.y;
 			min_bb.z = min_bb.z < p.z ? min_bb.z : p.z;

			max_bb.x = max_bb.x < p.x ? p.x : max_bb.x;
			max_bb.y = max_bb.y < p.y ? p.y : max_bb.y;
			max_bb.z = max_bb.z < p.z ?	p.z	: max_bb.z;

		}
	}
	
}

Model::~Model()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	for(int i = 0; i < textures_loaded.size(); i++)
	{
		glDeleteTextures(1, &textures_loaded[i].id);
	}
}

void Model::Draw(Shader shader)
{
	for(unsigned int i = 0; i < meshes.size(); i++)
	{
		meshes[i].Draw(shader);
	}
}

void Model::loadModel(std::string path) 
{
	//importing model, the main scene node, then going through tree to load the rest 
	Assimp::Importer import;
	const aiScene* scene = import.ReadFile(path, aiProcess_OptimizeMeshes | aiProcess_Triangulate | aiProcess_FlipUVs); // | aiProcess_CalcTangentSpace);

	if( !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode )
	{
		std::cout << "ERROR::ASSIMP::" << import.GetErrorString() << std::endl;
		return;
	}

	directory = path.substr(0, path.find_last_of('/'));
	std::cout << "DIR = " << directory << std::endl;
	processNode(scene->mRootNode, scene);
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
	//create all meshes, going through tree recursively
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh, scene));
	}

	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		processNode(node->mChildren[i], scene);
	}
}


Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<mVertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<mTexture> textures;

	//load vertices
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		mVertex vertex;

		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);

		if(mesh->mTextureCoords[0])
			vertex.TexCoords = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y); //up to 8 tex coords per vertex! only use [0] th one!
		else
			vertex.TexCoords = glm::vec2(-1.0f, -1.0f);
		
		//vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
		//vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);

		vertices.push_back(vertex);
	}

	//load indices
	for(unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];

		for(unsigned int j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	//load textures
	if(mesh->mMaterialIndex >= 0)
	{
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		
		//std::cout << "Model Material at Index " <<  <<  std::endl;

		std::vector<mTexture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

		std::vector<mTexture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular"); 
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

		std::vector<mTexture> normalMaps = loadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal"); //_NORMALS or aiTextureType_HEIGHT? 
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());

		std::vector<mTexture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, "texture_height"); //_HEIGHT or aiTextureType_AMBIENT?
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());
	}

	return Mesh(vertices, indices, textures);
}


std::vector<mTexture> Model::loadMaterialTextures(aiMaterial *mat, aiTextureType type, std::string typeName)
{
	//std::cout << "Load Material " << typeName << " -- There are " << mat->GetTextureCount(type) << std::endl;

	std::vector<mTexture> textures;
	for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);

		//std::cout << "NAME = " << str.C_Str() << std::endl;

		bool loaded = false;

		for(unsigned int j = 0; j < textures_loaded.size(); j++)
		{
			if(std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
			{
				textures.push_back(textures_loaded[j]);
				loaded = true;
				break;
			}
		}

		if(!loaded)
		{
			mTexture texture;
			texture.id = loadTextureFromFile(str.C_Str(), this->directory);
			texture.type = typeName;
			texture.path = str.C_Str();

			textures.push_back(texture);
			textures_loaded.push_back(texture);
		}
	}

	return textures;
}

unsigned int Model::loadTextureFromFile(const char* path, const std::string &directory, bool gamma)
{
	std::string filename = directory + "/" + std::string(path);
	//std::cout << "FULL PATH = " << filename << std::endl;
	unsigned int textureID;
	glGenTextures(1, &textureID);

	int width, height, nrChannels;
	//stbi_set_flip_vertically_on_load(true); //set true if image loads upside down
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrChannels, 0); 

	if(data)
	{
		//std::cout << "Loaded Texture at path " << filename << std::endl << std::endl << std::endl;
		GLenum format;
		if(nrChannels == 1)
			format = GL_RED;
		else if (nrChannels == 3)
			format = GL_RGB;
		else if (nrChannels == 4)
			format = GL_RGBA;
		
		glBindTexture(GL_TEXTURE_2D, textureID);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	}
	else
	{
		std::cout << "Model: Failed to load texture at " << filename << std::endl;
	}

	stbi_image_free(data);
	return textureID;
}

BB Model::getBoundingBox(glm::mat4 &model_transform)
{
	glm::vec4 min = model_transform * min_bb;; 
	glm::vec4 max = model_transform * max_bb;;

	glm::vec3 tr_min = glm::vec3(min.x, min.y, min.z) / min.w;
	glm::vec3 tr_max = glm::vec3(max.x, max.y, max.z) / max.w;

	BB BoundingBox;
	BoundingBox.InitFromMinMax(tr_min, tr_max);

	return BoundingBox;
}

void Model::cullingON()
{
	culling = true;
}

void Model::cullingOFF()
{
	culling = false;
}

bool Model::getCullingStatus()
{
	return culling;
}