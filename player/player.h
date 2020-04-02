#ifndef PLAYER_H
#define PLAYER_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp> //rotate vector to align weapon with camera 

#include "../rendering/camera.h"
#include "../geometry/weapon.h"
#include "../rendering/layers/abstractlayer.h"
#include "../rendering/group.h"
#include "../scene/scenegraph.h"

class Player
{
	public:
		Player(Camera* camera, Weapon* weapon);
		void Update(float deltaTime);
		void addLayer(AbstractLayer* layer);

	private:
		Camera* camera = NULL;
		Weapon* weapon = NULL;
};


#endif //PLAYER