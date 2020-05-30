#ifndef BATCH_INSTANCED_H
#define BATCH_INSTANCED_H

#include "batchabstract.h"


//DIVIDE INTO FIXED DATA -- SET ONLY ONCE 
//AND CHANGING DATA WITH A PER ATTRIBUTE THING OF ONE!!

#define INSTANCED_VERTEX_TYPE_FIXED VertexDataInstancedFixed
#define INSTANCED_VERTEX_TYPE_VARIABLE VertexDataInstancedVariable
#define INSTANCED_INDICES_TYPE unsigned short
#define INSTANCED_GL_INDICES_TYPE GL_UNSIGNED_SHORT

#define INSTANCED_MAX_INSTANCES 15000 
#define INSTANCED_VERTEX_SIZE_FIXED sizeof(INSTANCED_VERTEX_TYPE_FIXED)
#define INSTANCED_BUFFER_SIZE_FIXED (2000 * sizeof(INSTANCED_VERTEX_TYPE_FIXED))
#define INSTANCED_VERTEX_SIZE_VARIABLE sizeof(INSTANCED_VERTEX_TYPE_VARIABLE)
#define INSTANCED_BUFFER_SIZE_VARIABLE (INSTANCED_MAX_INSTANCES * INSTANCED_VERTEX_SIZE_VARIABLE)

#define INSTANCED_INDICES_SIZE sizeof(INSTANCED_INDICES_TYPE)
#define INSTANCED_BUFFER_SIZE_INDICES (3000 * INSTANCED_INDICES_SIZE)

#define INSTANCED_SHADER_VERTEX_INDEX 0
#define INSTANCED_SHADER_NORMAL_INDEX 1
#define INSTANCED_SHADER_TEXCOORD_INDEX 2
#define INSTANCED_SHADER_MODTR_INDEX_0 3
#define INSTANCED_SHADER_MODTR_INDEX_1 (INSTANCED_SHADER_MODTR_INDEX_0+1)
#define INSTANCED_SHADER_MODTR_INDEX_2 (INSTANCED_SHADER_MODTR_INDEX_0+2)
#define INSTANCED_SHADER_MODTR_INDEX_3 (INSTANCED_SHADER_MODTR_INDEX_0+3)
#define INSTANCED_SHADER_TEXSCALE_INDEX 7
#define INSTANCED_SHADER_TEXDIFFID_INDEX 8
#define INSTANCED_SHADER_TEXSPECID_INDEX 9
#define INSTANCED_SHADER_LIGHTING_INDEX 10
#define INSTANCED_SHADER_COLOR_INDEX 11

class BatchInstanced: public BatchAbstract
{
	public:
		BatchInstanced();
		~BatchInstanced();
		void Begin();
		void End();
		void Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, std::vector<glm::vec2>& texture_scalings);
		void Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms);
		void Flush();
		void FlushKeep();
		void Clear();
		
	private:
		INSTANCED_VERTEX_TYPE_FIXED* m_VertexBufferFixed;
		INSTANCED_VERTEX_TYPE_VARIABLE* m_VertexBufferVariable;
		INSTANCED_INDICES_TYPE* m_IndexBuffer = NULL;
		INSTANCED_INDICES_TYPE m_Offset = 0;
		INSTANCED_INDICES_TYPE m_IndexCount = 0;

		unsigned int m_VBO_VAR; //Use glMapBuffer
		INSTANCED_INDICES_TYPE m_InstancedCount = 0;

		bool First = true;

		std::vector<unsigned int> m_TextureID;
		//DEBUG
		void PrintIndexBuffer();
};

#endif //BATCH_INSTANCED_H