#ifndef DEPTH_MAP_LAYER_H
#define DEPTH_MAP_LAYER_H

#include "scenelayer.h"
#include "../batch/batchdepthmap.h"

class DepthmapLayer: public SceneLayer
{
	public:
		DepthmapLayer(Camera* camera, Shader* shader);
		~DepthmapLayer();
		void Render();
		void RenderKeep();

	private:
		Shader* m_CopyCubemap = new Shader("shaders/utils/copyCubemap.vs", "shaders/utils/copyCubemap.gs", "shaders/utils/copyCubemap.fs");
		Shader* m_GausBlurCubemap = new Shader("shaders/utils/blurCubemap.vs", "shaders/utils/blurCubemap.gs", "shaders/utils/gausBlurCubemap.fs");
		Shader* m_BilateralFilterCubemap = new Shader("shaders/utils/blurCubemap.vs", "shaders/utils/blurCubemap.gs", "shaders/utils/bilateralFilterCubemap.fs");
		void BlurShadowMaps();
};

#endif //DEPTH_MAP_LAYER_H