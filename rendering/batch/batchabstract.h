#ifndef BATCH_ABSTRACT_H
#define BATCH_ABSTRACT_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>
#define GLM_FORCE_SWIZZLE 
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include "VertexData.h"
#include "../../geometry/geometry.h"
#include "../../geometry/model.h"
#include "../../material_system/asset.h"

#define MAX_TEXTURE_SLOTS 20 //32

class BatchAbstract
{
	public:
		virtual void Begin() = 0;
		virtual void End() = 0;
		virtual void Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms) = 0;
		virtual void Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms) = 0;
		//virtual void Submit(Asset* asset, glm::mat4* model_transform) = 0;
		//virtual void Submit(Model* model, glm::mat4* model_transform) = 0;
		virtual void Flush() = 0;
		virtual void FlushKeep() = 0;
		virtual void Clear() = 0;

	protected:
		//ALSO HAS A m_VertexData Pointer to VertexData Buffer
		unsigned int m_VAO;
		unsigned int m_VBO; //Use glMapBuffer
		unsigned int m_IBO; //Use glBufferSubData - want to draw multiple kinds of objects! -- LATER when merge game object 
		unsigned int* m_IndexBuffer = NULL;
		unsigned int m_Offset = 0;
		unsigned int m_IndexCount = 0;

		bool m_Keep = false;

		//Basic Geometry Info -- make the class static?? 
		batchSquare bSquare;
		batchCube bCube;

		//DEBUG
		virtual void PrintIndexBuffer() = 0;
};

#endif //BATCH_ABSTRACT_H