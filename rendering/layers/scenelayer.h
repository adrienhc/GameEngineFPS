#ifndef SCENE_LAYER_H
#define SCENE_LAYER_H

#include "abstractlayer.h"
#include "../batch/batchrenderer.h"
#include "../light.h"

#define SHADER_MAX_LIGHTS 5
#define SHADER_MAX_BULLET_HOLES 5

class SceneLayer: public AbstractLayer
{
	public:
		SceneLayer(Camera* camera, Shader* shader);
		~SceneLayer();
		void Render();
		void RenderKeep();
		void AddLight(Group* light, bool renderable = false);
		void ClearLight();
		void AddBulletHole(glm::vec3 bullet_hole);
		void Clear();

	protected:
		void ShaderSetup();
		SceneLayer(BatchAbstract* renderer, Camera* camera, Shader* shader);
		
		//LIGHT
		std::vector<Group*> m_SceneLights;
		bool m_UpdateLights = true;
		
		//BULLET HOLES
		int m_BulletHolesIndex = 0;
		//std::vector<glm::vec3> m_BulletHoles;
};

#endif //SCENE_LAYER_H