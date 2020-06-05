#ifndef SCENE_GRAPH
#define SCENE_GRAPH

#include "../pch.h"

#include "../utils/outline.h"
#include "../material_system/asset.h"
#include "../geometry/geometry.h"
#include "../geometry/model.h"
#include "../rendering/light.h"




enum eType {eRoot, eTranslate, eRotate, eScale, eAsset, eModel, ePointLight};
enum eShader {eObject, eInstanced, eLights, eModelshdr, eExplode, eNULL};


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

	nAsset(Asset* pAsset, glm::vec2 texture_scaling, eShader type)
	{
		asset = pAsset;
		this->texture_scaling = texture_scaling;
		shader_type = type; 
		render = true;
	}

	Asset* GetAsset()
	{
		return asset;
	}

	void SetTransform(glm::mat4 transform)
	{
		model_transform = transform;
		transform_set = true;
	}

	glm::mat4 GetTransform()
	{
		return model_transform;
	}

	bool HasTransform()
	{
		return transform_set;
	}

	glm::vec2& GetTextureScaling()
	{
		return texture_scaling;
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
	bool transform_set = false;
	glm::mat4 model_transform;
	glm::vec2 texture_scaling = glm::vec2(1.0f);
	eShader shader_type;
};


class nModel: public nNode
{
public:
	nModel(Model* pModel, eShader type)
	{
		model = pModel;
		shader_type = type; 
		render = true;
	}

	nModel(Model* pModel, Model* pSmoothModel, Outline* pOutline, eShader type)
	{
		model = pModel;
		smoothModel = pSmoothModel;
		outline = pOutline;
		shader_type = type;
		render = true;
	}

	Model* GetModel()
	{
		return model;
	}

	Model* GetSmoothModel()
	{
		return smoothModel;
	}

	eShader GetShader()
	{
		return shader_type;
	}

	Outline* GetOutline()
	{
		return outline;
	}

	void DisableOutline()
	{
		render_outline = false;
	}

	void EnableOutline()
	{
		render_outline = true;
	}

	bool RenderOutline()
	{
		return render_outline;
	}

	bool HasOutline()
	{
		return (outline != NULL);
	}

	void SetTransform(glm::mat4 transform)
	{
		model_transform = transform;
		transform_set = true;
	}

	glm::mat4 GetTransform()
	{
		return model_transform;
	}

	bool HasTransform()
	{
		return transform_set;
	}

	virtual eType GetType()
	{
		return eModel;
	}

	void Explode()
	{
		shader_type = eExplode;
		//if(outline)
		//{
		//	outline = NULL; //Legacy simple Renderer
		//}
	}

	void Instanced()
	{
		render = false;
	}

private:
	Model* model = NULL;
	Model* smoothModel = NULL;
	Outline* outline = NULL;
	bool transform_set = false;
	bool render_outline = false;
	glm::mat4 model_transform;
	eShader shader_type;
};



class nPointLight: public nNode
{
public:
	nPointLight(PointLight* pLight, int id)
	{
		light = pLight;
		index = id; //INDEX WITHIN ROOM -- FrameBuffer And ShadowMapBuffer Index
		render = true;
		float lightShininess = 2.0f;
		asset = new Asset(eInvertedCube, "lightBulb", glm::vec4(1.0f), glm::vec4(light->diffuse, 1.0f), glm::vec4(light->specular, 1.0f), lightShininess, glm::vec4(light->diffuse, 1.0f), false, "");
	}

	PointLight* GetLight()
	{
		return light;
	}

	int GetIndex()
	{
		return index; //INDEX WITHIN ROOM -- FrameBuffer And ShadowMapBuffer Index
	}

	Asset* GetAsset()
	{
		return asset;
	}

	virtual eType GetType()
	{
		return ePointLight;
	}

	void Instanced()
	{
		render = false;
	}

	~nPointLight()
	{
		delete asset;
		Destroy();
	}
private:
	PointLight* light;
	int index;
	Asset* asset = NULL;
};


#endif //SCENE_GRAPH