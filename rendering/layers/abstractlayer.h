#ifndef ABSTRACT_LAYER_H
#define ABSTRACT_LAYER_H

#include "../../pch.h"
#include "../batch/batchabstract.h"
#include "../shader.h"
#include "../camera.h"
#include "../group.h"
#include "../../geometry/bounding_box.h"

class AbstractLayer
{
	public:
		virtual ~AbstractLayer();
		virtual void Add(Group* group);
		virtual void Render(bool cull = true);
		virtual void RenderKeep();
		virtual void Clear();
		virtual bool IsCulled(BB &bounding_box);
		
	protected:
		AbstractLayer(BatchAbstract* renderer, Camera* camera, Shader* shader);
		Shader* GetShader();
		Camera* GetCamera();
		BatchAbstract* GetRenderer();
		Shader* m_Shader;
		BatchAbstract* m_Renderer;
		Camera* m_Camera;
		std::vector<Group*> m_Groups; 
};

#endif //ABSTRACT_LAYER_H