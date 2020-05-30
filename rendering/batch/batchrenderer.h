#ifndef BATCH_RENDERER_H
#define BATCH_RENDERER_H

#include "batchabstract.h"

#define RENDERER_VERTEX_TYPE VertexData
#define RENDERER_INDICES_TYPE unsigned short
#define RENDERER_GL_INDICES_TYPE GL_UNSIGNED_SHORT

#define RENDERER_MAX_VERTICES 20000 
#define RENDERER_VERTEX_SIZE sizeof(RENDERER_VERTEX_TYPE)
#define RENDERER_BUFFER_SIZE (RENDERER_MAX_VERTICES * RENDERER_VERTEX_SIZE)
#define RENDERER_INDICES_SIZE sizeof(RENDERER_INDICES_TYPE)
#define RENDERER_MAX_INDICES (RENDERER_MAX_VERTICES * 3 * RENDERER_INDICES_SIZE)

#define RENDERER_SHADER_VERTEX_INDEX 0
#define RENDERER_SHADER_NORMAL_INDEX 1
#define RENDERER_SHADER_TEXCOORD_INDEX 2
#define RENDERER_SHADER_MODTR_INDEX_0 3
#define RENDERER_SHADER_MODTR_INDEX_1 (RENDERER_SHADER_MODTR_INDEX_0+1)
#define RENDERER_SHADER_MODTR_INDEX_2 (RENDERER_SHADER_MODTR_INDEX_0+2)
#define RENDERER_SHADER_MODTR_INDEX_3 (RENDERER_SHADER_MODTR_INDEX_0+3)
#define RENDERER_SHADER_TEXDIFFID_INDEX 7
#define RENDERER_SHADER_TEXSPECID_INDEX 8
#define RENDERER_SHADER_LIGHTING_INDEX 9
#define RENDERER_SHADER_COLOR_INDEX 10

class BatchRenderer: public BatchAbstract
{
	public:
		BatchRenderer();
		~BatchRenderer();
		void Begin();
		void End();
		void Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, std::vector<glm::vec2>& texture_scalings);
		void Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms);
		void Flush();
		void FlushKeep();
		void Clear();
		
	private:
		RENDERER_VERTEX_TYPE* m_VertexBuffer;
		RENDERER_INDICES_TYPE* m_IndexBuffer = NULL;
		RENDERER_INDICES_TYPE m_Offset = 0;
		RENDERER_INDICES_TYPE m_IndexCount = 0;

		std::vector<unsigned int> m_TextureID;
		//DEBUG
		void PrintIndexBuffer();
};

#endif //BATCH_RENDERER_H