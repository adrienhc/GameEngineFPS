#ifndef PARTICLE_LAYER_H
#define PARTICLE_LAYER_H

#include "scenelayer.h"
#include "../batch/batchparticle.h"

class ParticleLayer: public SceneLayer
{
	public:
		ParticleLayer(Camera* camera, Shader* shader);
		~ParticleLayer();
		//void Render();
};

#endif //PARTICLE_LAYER_H