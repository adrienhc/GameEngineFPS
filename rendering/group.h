#ifndef GROUP_H
#define GROUP_H

#include "../scene/scenegraph.h"
#include "batch/batchabstract.h"
#include "light.h"
#include "../material_system/asset.h"
#include "../geometry/model.h"
#include <stack>
#include <vector>
#include <glm/glm.hpp>

class Group
{
	public:
		Group();
		Group(nNode* Root);
		Group(std::vector<nNode*> Roots);
		~Group();
		void Submit(BatchAbstract* renderer);
		std::vector<PointLight*>& GetLights();
		std::vector<int>& GetLightsRoomIndex();

	private:
		//GENERAL
		void Traverse(nNode* Root, eType type);
		std::stack<glm::mat4> MatrixStack;
		
		//GEOMETRY
		void Add(Asset* asset, glm::mat4& model_transform);
		std::vector<Asset*> m_Assets;
		std::vector<glm::mat4> m_AssetTransforms;
		void Add(Model* model, glm::mat4& model_transform);
		std::vector<Model*> m_Models;
		std::vector<glm::mat4> m_ModelTransforms;

		//LIGHT
		std::vector<PointLight*> m_PointLights;
		std::vector<int> m_PointLightsRoomIndex;
};
#endif //GROUP_H