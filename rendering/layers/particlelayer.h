#ifndef PARTICLE_LAYER_H
#define PARTICLE_LAYER_H

#include "scenelayer.h"
#include "../batch/batchparticle.h"
#include "../../material_system/texture.h"
#include "../../particle_system/particle_system.h"

class ParticleLayer: public SceneLayer
{
	public:
		ParticleLayer(Camera* camera, Shader* shader, ParticleSystem* particle_system, pShape type);
		~ParticleLayer();
		void Render();
		void RenderKeep();

	private:
		ParticleSystem* m_ParticleSystem;
		pShape m_type;
		Texture* noiseRGB = new Texture("./textures/noiseRGB-2.png");
		Texture* smokeTexture = new Texture("./textures/smoke.png");
};

#endif //PARTICLE_LAYER_H