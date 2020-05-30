#ifndef BATCH_PARTICLE_H
#define BATCH_PARTICLE_H

#include "batchabstract.h"
#include "../../particle_system/particle.h"

#define PARTICLE_VERTEX_TYPE VertexDataParticle

#define PARTICLE_MAX_VERTICES 10000 
#define PARTICLE_VERTEX_SIZE sizeof(PARTICLE_VERTEX_TYPE)
#define PARTICLE_BUFFER_SIZE (PARTICLE_MAX_VERTICES * PARTICLE_VERTEX_SIZE)

#define PARTICLE_SHADER_VERTEX_INDEX 0
#define PARTICLE_SHADER_NORMAL_INDEX 1
#define PARTICLE_SHADER_TEXCOORD_INDEX 2
#define PARTICLE_SHADER_PARTICLEDATA_INDEX 3
#define PARTICLE_SHADER_COLOR_INDEX 4

class Particle; 

class BatchParticle: public BatchAbstract
{
	public:
		BatchParticle();
		~BatchParticle();
		void Begin();
		void End();
		void Submit(Particle* particle);
		void Flush();
		void FlushKeep();
		void Clear();
		
	private:
		PARTICLE_VERTEX_TYPE* m_VertexBuffer;

		//CANNOT CALL, ONLY TO RESPECT INHERITANCE CONSTRAINT
		void Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, std::vector<glm::vec2>& texture_scalings) {};
		void Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms) {};

		//DEBUG
		void PrintIndexBuffer(){};
};

#endif //BATCH_PARTICLE_H