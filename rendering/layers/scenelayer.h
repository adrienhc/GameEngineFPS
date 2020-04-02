#ifndef SCENE_LAYER_H
#define SCENE_LAYER_H

#include "abstractlayer.h"
#include "../batch/batchrenderer.h"
#include "../light.h"

#define SHADER_MAX_LIGHTS 9

class SceneLayer: public AbstractLayer
{
	public:
		SceneLayer(Camera* camera, Shader* shader);
		~SceneLayer();
		void Render();
		void RenderKeep();
		void AddLight(Group* light, bool renderable = false);
		void ClearLight();
		void Clear();

	protected:
		void ShaderSetup();
		SceneLayer(BatchAbstract* renderer, Camera* camera, Shader* shader);
		std::vector<Group*> m_SceneLights;
		bool m_UpdateLights = true;
};

#endif //SCENE_LAYER_H