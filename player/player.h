#ifndef PLAYER_H
#define PLAYER_H

#include "../pch.h"

#include "../rendering/camera.h"
#include "../geometry/weapon.h"
#include "../particle_system/particle.h"
#include "../particle_system/particle_system.h"
#include "../rendering/layers/abstractlayer.h"
#include "../rendering/group.h"
#include "../scene/scenegraph.h"

class Player
{
	public:
		Player(Camera* camera, Weapon* weapon);
		void Update(float deltaTime);
		void addLayer(AbstractLayer* layer, bool flash = false);
		void addParticle(ParticleSystem* particle_system);

	private:
		Camera* camera = NULL;
		Weapon* weapon = NULL;
};


#endif //PLAYER