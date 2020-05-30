#ifndef BATCH_DEPTHMAP_H
#define BATCH_DEPTHMAP_H

#include "batchabstract.h"

#define DEPTHMAP_VERTEX_TYPE VertexDataDepthmap

#define DEPTHMAP_MAX_TILES 50000
#define DEPTHMAP_MAX_VERTICES (4 * DEPTHMAP_MAX_TILES) 
#define DEPTHMAP_VERTEX_SIZE sizeof(DEPTHMAP_VERTEX_TYPE)
#define DEPTHMAP_BUFFER_SIZE (DEPTHMAP_MAX_VERTICES * DEPTHMAP_VERTEX_SIZE)
#define DEPTHMAP_INDICES_SIZE sizeof(unsigned int)
#define DEPTHMAP_MAX_INDICES (DEPTHMAP_MAX_VERTICES * 3 * DEPTHMAP_INDICES_SIZE)
#define DEPTHMAP_TYPE_INDICES GL_UNSIGNED_INT

#define DEPTHMAP_SHADER_VERTEX_INDEX 0

class BatchDepthmap: public BatchAbstract
{
	public:
		BatchDepthmap();
		~BatchDepthmap();
		void Begin();
		void End();
		void Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, std::vector<glm::vec2>& texture_scalings);//, glm::mat4 model_tr);
		void Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms);
		void Flush();
		void FlushKeep();
		void Clear();

	private:
		DEPTHMAP_VERTEX_TYPE* m_VertexBuffer;
		
		//DEBUG
		void PrintIndexBuffer();
};

#endif //BATCH_DEPTHMAP_H