#ifndef DEPTH_MAP_LAYER_H
#define DEPTH_MAP_LAYER_H

#include "scenelayer.h"
#include "../batch/batchdepthmap.h"

extern const unsigned int WINDOW_WIDTH; //defined in main.h 
extern const unsigned int WINDOW_HEIGHT; //used to reset viewport after shadow pass

class DepthmapLayer: public SceneLayer
{
	public:
		DepthmapLayer(Camera* camera, Shader* shader);
		~DepthmapLayer();
		void Render();
		void RenderKeep();
};

#endif //DEPTH_MAP_LAYER_H