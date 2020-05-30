#ifndef BATCH_OUTLINE_H
#define BATCH_OUTLINE_H

#include "batchabstract.h"

#define OUTLINE_VERTEX_TYPE VertexDataOutline

#define OUTLINE_MAX_TILES 10000
#define OUTLINE_MAX_VERTICES (4 * OUTLINE_MAX_TILES) 
#define OUTLINE_VERTEX_SIZE sizeof(OUTLINE_VERTEX_TYPE)
#define OUTLINE_BUFFER_SIZE (OUTLINE_MAX_VERTICES * OUTLINE_VERTEX_SIZE)
#define OUTLINE_INDICES_SIZE sizeof(unsigned int)
#define OUTLINE_MAX_INDICES (OUTLINE_MAX_VERTICES * 3 * OUTLINE_INDICES_SIZE)
#define OUTLINE_TYPE_INDICES GL_UNSIGNED_INT

#define OUTLINE_SHADER_VERTEX_INDEX 0
#define OUTLINE_SHADER_COLOR_INDEX 1


class BatchOutline: public BatchAbstract
{
	public:
		BatchOutline();
		~BatchOutline();
		void Begin();
		void End();
		void Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, std::vector<glm::vec2>& texture_scalings);//, glm::mat4 model_tr);
		void Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms);
		void Flush();
		void FlushKeep();
		void Clear();
		
	private:
		OUTLINE_VERTEX_TYPE* m_VertexBuffer;
		
		//DEBUG
		void PrintIndexBuffer();
};

#endif //BATCH_OUTLINE_H