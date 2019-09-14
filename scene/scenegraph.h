#ifndef SCENE_GRAPH
#define SCENE_GRAPH

#include <iostream>
#include <list>
#include <cmath>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../material_system/asset.h"
#include "../geometry/geometry.h"
#include "../rendering/light.h"




enum eType {eRoot, eTranslate, eRotate, eScale, eAsset, ePointLight};
enum eShader {eObject, eInstanced, eLights};


class nNode
{
public:
	nNode() {}
	virtual ~nNode()
	{
		Destroy();
	}

	void Release()
	{
		delete this;
	}

	void Destroy()
	{
		for(std::list<nNode*>::iterator it = Children.begin(); it != Children.end(); it++)
		{
			(*it)->Release();
		}
		Children.clear();
	}

	void AddChildren(nNode* pNode)
	{
		Children.push_back(pNode);
	}

	nNode* AddChildrenRecursive(nNode* pNode)
	{
		Children.push_back(pNode);
		return Children.back();
	}

	std::list<nNode*> GetChildren()
	{
		return Children;
	}

	virtual eType GetType()
	{
		return eRoot;
	}

	bool RenderStatus()
	{
		return render;
	}

protected:
	std::list<nNode*> Children; 
	bool render = false;

// private:
// 	eType type = eRoot;
};



class nTranslate: public nNode
{
public:
	nTranslate(glm::vec3 vec)
	{
		Transform = glm::translate(glm::mat4(1.0f), vec);
	}

	void Update(glm::mat4 trsf)
	{
		Transform *= trsf;
	}

	void Set(glm::mat4 trsf)
	{
		Transform = trsf;
	}

	glm::mat4 Get()
	{
		return Transform;
	}

	virtual eType GetType()
	{
		return eTranslate;
	}

private:
	glm::mat4 Transform;
	//eType type = eTransform;
};



class nRotate: public nNode
{
public:
	nRotate(float stp, glm::vec3 axe, bool fix)
	{
		axis = axe;
		step = stp;
		Transform = glm::rotate(glm::mat4(1.0f), step, axis);
		fixed = fix;

	}

	void Update()
	{
		if(!fixed)
			Transform = glm::rotate(Transform, step, axis);
	}

	void Set(glm::mat4 trsf)
	{
		Transform = trsf;
	}

	glm::mat4 Get()
	{
		return Transform;
	}

	virtual eType GetType()
	{
		return eRotate;
	}

private:
	glm::mat4 Transform;
	bool fixed;
	glm::vec3 axis;
	float step;
	//eType type = eTransform;
};

class nScale: public nNode
{
public:
	nScale(glm::vec3 vec)
	{
		Transform = glm::scale(glm::mat4(1.0f), vec);
	}

	void Update(glm::mat4 trsf)
	{
		Transform *= trsf;
	}

	void Set(glm::mat4 trsf)
	{
		Transform = trsf;
	}

	glm::mat4 Get()
	{
		return Transform;
	}

	virtual eType GetType()
	{
		return eScale;
	}

private:
	glm::mat4 Transform;
	//eType type = eTransform;Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
};

class nAsset: public nNode
{
public:
	nAsset(Asset* pAsset, eShader type)
	{
		asset = pAsset;
		shader_type = type; 
		render = true;
	}

	Asset* GetAsset()
	{
		return asset;
	}

	eShader GetShader()
	{
		return shader_type;
	}

	virtual eType GetType()
	{
		return eAsset;
	}

	void Instanced()
	{
		render = false;
	}

private:
	Asset* asset;
	eShader shader_type;
};


class nPointLight: public nNode
{
public:
	nPointLight(PointLight* pLight, int id)
	{
		light = pLight;
		index = id;
		render = true;
	}

	PointLight* GetLight()
	{
		return light;
	}

	int GetIndex()
	{
		return index;
	}

	virtual eType GetType()
	{
		return ePointLight;
	}

	void Instanced()
	{
		render = false;
	}


private:
	PointLight* light;
	int index;
};


#endif //SCENE_GRAPH