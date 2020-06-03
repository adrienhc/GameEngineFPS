#include "group.h"


Group::Group()
{
	MatrixStack.push(glm::mat4(1.0f));	
}

Group::~Group()
{

}

Group::Group(nNode* Root)
{
	Traverse(Root, eRoot);
}

Group::Group(std::vector<nNode*> Roots)
{
	for(int i = 0; i < Roots.size(); i++)
		Traverse(Roots[i], eRoot);
}

std::vector<PointLight*>& Group::GetLights()
{
	return m_PointLights;
}

std::vector<int>& Group::GetLightsRoomIndex()
{
	return m_PointLightsRoomIndex;
}

void Group::Add(Asset* asset, glm::mat4& model_transform)
{
	m_Assets.push_back(asset);
	m_AssetTransforms.push_back(model_transform);
	m_AssetTextureScalings.push_back(glm::vec2(1.0f));
	m_AssetBoundingBoxes.push_back(asset->getBoundingBox(model_transform));
}

void Group::Add(Asset* asset, glm::mat4& model_transform, glm::vec2& texture_scaling)
{
	m_Assets.push_back(asset);
	m_AssetTransforms.push_back(model_transform);
	m_AssetTextureScalings.push_back(texture_scaling);
	m_AssetBoundingBoxes.push_back(asset->getBoundingBox(model_transform));
}

void Group::Add(Model* model, glm::mat4& model_transform)
{
	m_Models.push_back(model);
	m_ModelTransforms.push_back(model_transform);
	m_ModelBoundingBoxes.push_back(model->getBoundingBox(model_transform));
}

void Group::Submit(BatchAbstract* renderer)
{
	renderer->Submit(m_Assets, m_AssetTransforms, m_AssetTextureScalings);
	renderer->Submit(m_Models, m_ModelTransforms);
}

void Group::Submit(BatchAbstract* renderer, Camera* camera) //Culling Version
{
	//BARE IMPLEMENTATION, TOO MUCH COPYING??, DO MARKING INSTEAD??, CHECK IN RENDERER?? TOO ANNOYING, NEED CHANGE ALL RENDERERS, CHECK AND SEE ...
	std::vector<Asset*> NonCulled_Assets;
	std::vector<glm::mat4> NonCulled_AssetTransforms;
	std::vector<glm::vec2> NonCulled_AssetTextureScalings;
	for(int i = 0; i < m_Assets.size(); i++)
	{
		if(!camera->IsCulled(m_AssetBoundingBoxes[i]))
		{
			NonCulled_Assets.push_back(m_Assets[i]);
			NonCulled_AssetTransforms.push_back(m_AssetTransforms[i]);
			NonCulled_AssetTextureScalings.push_back(m_AssetTextureScalings[i]);
		}
	}
	renderer->Submit(NonCulled_Assets, NonCulled_AssetTransforms, NonCulled_AssetTextureScalings);


	std::vector<Model*> NonCulled_Models;
	std::vector<glm::mat4> NonCulled_ModelTransforms;
	for(int i = 0; i < m_Models.size(); i++)
	{
		if(!m_Models[i]->getCullingStatus() || !camera->IsCulled(m_ModelBoundingBoxes[i]) )
		{
			NonCulled_Models.push_back(m_Models[i]);
			NonCulled_ModelTransforms.push_back(m_ModelTransforms[i]);
		}
	}
	renderer->Submit(NonCulled_Models, NonCulled_ModelTransforms);
}

void Group::Submit(ParticleSystem* particle_system, pShape type)
{
	particle_system->Add(m_Assets, m_AssetTransforms, type);
	particle_system->Add(m_Models, m_ModelTransforms, type);
}


void Group::Traverse(nNode* Root, eType type)
{
	std::list<nNode*> nChildren = Root->GetChildren();

	if(type == eRoot) //ALWAYS GO THERE FIRST
	{
		while(!MatrixStack.empty())
			MatrixStack.pop();
		MatrixStack.push(glm::mat4(1.0f));

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}
	}

	else if (type == eTranslate)
	{
		//std::cout << "TR" << std::endl;
		nTranslate* Tr = dynamic_cast<nTranslate*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Tr->Get();
		MatrixStack.push( currMat * prevMat); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eRotate)
	{
		nRotate* Rot = dynamic_cast<nRotate*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Rot->Get();
		MatrixStack.push( currMat * prevMat); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

		Rot->Update(); // Keep Rotating for next update

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eScale)
	{
		nScale* Sc = dynamic_cast<nScale*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Sc->Get();
		MatrixStack.push( prevMat * currMat ); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eAsset)
	{	
		nAsset* Ast = dynamic_cast<nAsset*> (Root);

		glm::mat4 model_transform;

		if(Ast->HasTransform()) //IF RENDER MODEL ON ITS OWN -- sometimes want to render without being part of a tree
		{
			model_transform = Ast->GetTransform();
		}
		else
			 model_transform = MatrixStack.top();

		Add(Ast->GetAsset(), model_transform, Ast->GetTextureScaling());

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

	}

	else if (type == eModel)
	{		
		nModel* Mdl = dynamic_cast<nModel*> (Root);
		
		glm::mat4 model_transform;

		if(Mdl->HasTransform()) //IF RENDER MODEL ON ITS OWN -- sometimes want to render without being part of a tree
			model_transform = Mdl->GetTransform();
		else
			 model_transform = MatrixStack.top();

		//Since shared Node for different groups, need Push Outlines First!
		if (Mdl->GetShader() == eExplode)
		{
			if(Mdl->HasOutline())
				Add(Mdl->GetSmoothModel(), model_transform);
			else
				Add(Mdl->GetModel(), model_transform);
		}
		else if(Mdl->HasOutline() && Mdl->RenderOutline())
		{
			Add(Mdl->GetSmoothModel(), model_transform);
			Mdl->DisableOutline();
		}
		else
		{
			Add(Mdl->GetModel(), model_transform);
		}

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}
	}

	else if (type == ePointLight)
	{
		//std::cout << "LIGHT" << std::endl;
		nPointLight* Lt = dynamic_cast<nPointLight*> (Root);
		PointLight* light = Lt->GetLight();
		glm::mat4 model_transform = MatrixStack.top();
		light->setTransform(model_transform, Lt->GetIndex());

		m_PointLights.push_back(light);
		m_PointLightsRoomIndex.push_back(Lt->GetIndex());

		model_transform = glm::scale(model_transform, glm::vec3(2.0f, 0.4f, 1.0f)); //change light cube dimensions
		Add(Lt->GetAsset(), model_transform);


		//shadowLights.push_back(*light); //Add this light to the lights to compute depth map for
		//shadowLightsIndex.push_back(Lt->GetIndex()); //so that have index of light in shader, can bind correct cubeMap to correct Texture slot

		//myShader.setPointLight(light, Lt->GetIndex()); //INDEX FROM SCENEGRAPH
		//instancedShader.setPointLight(light, Lt->GetIndex()); //INSTANCED SHADER TOO
		//modelShader.setPointLight(light, Lt->GetIndex()); //MODEL SHADER
		//modelExplodeShader.setPointLight(light, Lt->GetIndex()); //MODEL EXPLODE CUBE SHADER

		//DRAWING LIGHT CUBE
		//lightShader.setTransform(model_transform);
		//lightShader.setVec3("lightColor", glm::vec3(1.0f));
		//Cube.Render();
 		

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}
	}

	else
		std::cout << " -- UNKNOWN TYPE -- " << std::endl;
}