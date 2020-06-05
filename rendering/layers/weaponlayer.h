#ifndef WEAPONLAYER_H
#define WEAPONLAYER_H

#include "layer.h"
#include "batchrenderer.h"
#include "camera.h"
#include "shader.h"
#include "light.h"
#include "group.h"
#include "../scene/scenegraph.h"
#include "abstractlayer.h"

class WeaponLayer: public Layer
{
	//ADD LIGHT GEOMETRY TO GROUPS AS NEW GROUP
	public:
		WeaponLayer(Camera* camera, Shader* shader);
		~WeaponLayer();
		void Render();
		void AddLight(Group* light);
	private:
		std::vector<Group*> m_SceneLights;
		
};

#endif //SCENELAYER_H