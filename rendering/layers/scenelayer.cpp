#include "scenelayer.h"

SceneLayer::SceneLayer(Camera* camera, Shader* shader)
:AbstractLayer(new BatchRenderer(), camera, shader)
{}

SceneLayer::SceneLayer(BatchAbstract* renderer, Camera* camera, Shader* shader)
:AbstractLayer(renderer, camera, shader)
{}

SceneLayer::~SceneLayer()
{}

void SceneLayer::Render()
{
	ShaderSetup();
	AbstractLayer::Render();
}

void SceneLayer::RenderKeep()
{
	ShaderSetup();
	AbstractLayer::RenderKeep();	
}

void SceneLayer::ShaderSetup()
{
	//SETUP LIGHTS FOR SCENE
	if(!m_UpdateLights)
		return;

	Shader* m_Shader = GetShader();
	m_Shader->use();
	int lightCount = 0;

	for(int i = 0; i < m_SceneLights.size(); i++)
	{
		//Light Shader Setup
		std::vector<PointLight*> m_PointLights = m_SceneLights[i]->GetLights();
		std::vector<int> m_PointLightsRoomIndex = m_SceneLights[i]->GetLightsRoomIndex();
		for(int j = 0; j < m_PointLights.size(); j++)
		{
			m_Shader->setPointLight(m_PointLights[j], lightCount); //Set Light information
			m_PointLights[j]->bindShadowMapBatch(m_Shader, m_PointLightsRoomIndex[j], lightCount); //Set Light Depth Map
			lightCount++;
		}	
	}
	m_Shader->setLightInfo(lightCount);

	//Bind Unused Depth Map with first one
	std::vector<PointLight*> m_PointLights = m_SceneLights[0]->GetLights();
	for(int i = lightCount; i < SHADER_MAX_LIGHTS; i++)
	{
		m_PointLights[0]->bindShadowMapBatch(m_Shader, 0, i);
	}

	m_UpdateLights = false;	
}

void SceneLayer::AddLight(Group* lightGroup, bool renderable)
{
	m_UpdateLights = true;
	m_SceneLights.push_back(lightGroup);

	if(renderable)
	{
		//Add lights to main Rendering Groups to draw light containers when rendering whole scene 
		Add(lightGroup);
	}
}

void SceneLayer::ClearLight()
{
	m_SceneLights.clear();
	m_UpdateLights = true;
}

void SceneLayer::Clear()
{
	ClearLight();
	AbstractLayer::Clear();
}