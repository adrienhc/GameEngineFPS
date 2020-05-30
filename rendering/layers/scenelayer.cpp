#include "scenelayer.h"

SceneLayer::SceneLayer(Camera* camera, Shader* shader)
:AbstractLayer(new BatchRenderer(), camera, shader)
{
	shader->use();
	shader->setFloat("radiusImpact", 0.03f);

	//If Create SceneLayer directly!
	for(int i = 0; i < SHADER_MAX_BULLET_HOLES; i++)
    {
    	//m_BulletHoles.push_back(glm::vec3(1000.0f));
    	std::string str_index = std::to_string(i);
		shader->setVec3("bulletHoles[" + str_index + "]", glm::vec3(1000.0f));
    }
}

SceneLayer::SceneLayer(BatchAbstract* renderer, Camera* camera, Shader* shader)
:AbstractLayer(renderer, camera, shader)
{
	shader->use();
	shader->setFloat("radiusImpact", 0.03f);

	//If Create SceneLayer directly!
	for(int i = 0; i < SHADER_MAX_BULLET_HOLES; i++)
    {
    	// m_BulletHoles.push_back(glm::vec3(1000.0f));
    	std::string str_index = std::to_string(i);
		shader->setVec3("bulletHoles[" + str_index + "]", glm::vec3(1000.0f));
    }
}

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
	
	glm::vec3 playerPosition = GetCamera()->Position;
	int lightCount = 0;
	bool max_reached = false;


	//std::vector<PointLight*> m_ShadowBoundPointLights;

	for(int i = 0; i < m_SceneLights.size(); i++)
	{
		if(max_reached)
			break;
		//Light Shader Setup
		std::vector<PointLight*> m_PointLights = m_SceneLights[i]->GetLights();
		std::vector<int> m_PointLightsRoomIndex = m_SceneLights[i]->GetLightsRoomIndex();

		for(int j = 0; j < m_PointLights.size(); j++)
		{	
			bool active_shadow = ( glm::distance(playerPosition, m_PointLights[j]->position[m_PointLightsRoomIndex[j]]) < 30.0f);
			m_Shader->setPointLight(m_PointLights[j], lightCount, m_PointLightsRoomIndex[j], active_shadow); //Set Light information
			m_PointLights[j]->bindShadowMapBatch(m_Shader, m_PointLightsRoomIndex[j], lightCount); //Set Light Depth Map
			lightCount++;

			if(lightCount >= SHADER_MAX_LIGHTS)
			{
				max_reached = true;
				break;
			}
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

void SceneLayer::AddBulletHole(glm::vec3 bullet_hole)
{
	//std::cout << "BHI " << m_BulletHolesIndex << std::endl;	
	Shader* m_Shader = GetShader();
	m_Shader->use();
	std::string str_index = std::to_string(m_BulletHolesIndex);
	m_Shader->setVec3("bulletHoles[" + str_index + "]", bullet_hole);
	
	m_BulletHolesIndex++;
	if(m_BulletHolesIndex > SHADER_MAX_BULLET_HOLES)
		m_BulletHolesIndex = 0;
}

void SceneLayer::Clear()
{
	ClearLight();
	AbstractLayer::Clear();
}