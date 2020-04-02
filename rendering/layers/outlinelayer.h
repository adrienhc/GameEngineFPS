#ifndef OUTLINE_LAYER_H
#define OUTLINE_LAYER_H

#include "abstractlayer.h"
#include "../batch/batchoutline.h"

class OutlineLayer: public AbstractLayer
{
	public:
		OutlineLayer(Camera* camera, Shader* shader);
		~OutlineLayer();
};

#endif //OUTLINE_LAYER_H