#ifndef INSTANCED_LAYER_H
#define INSTANCED_LAYER_H

#include "scenelayer.h"
#include "../batch/batchinstanced.h"

class InstancedLayer: public SceneLayer
{
	public:
		InstancedLayer(Camera* camera, Shader* shader);
		~InstancedLayer();
};

#endif //INSTANCED_LAYER_H