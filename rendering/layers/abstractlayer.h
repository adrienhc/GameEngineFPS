#ifndef ABSTRACT_LAYER_H
#define ABSTRACT_LAYER_H

#include "../batch/batchabstract.h"
#include "../shader.h"
#include "../camera.h"
#include "../group.h"
#include <vector>

class AbstractLayer
{
	public:
		virtual ~AbstractLayer();
		virtual void Add(Group* group);
		virtual void Render();
		virtual void RenderKeep();
		virtual void Clear();
		
	protected:
		AbstractLayer(BatchAbstract* renderer, Camera* camera, Shader* shader);
		Shader* GetShader();
		Camera* GetCamera();
		Shader* m_Shader;
		BatchAbstract* m_Renderer;
		Camera* m_Camera;
		std::vector<Group*> m_Groups; 
};

#endif //ABSTRACT_LAYER_H