#include "depthmaplayer.h"

DepthmapLayer::DepthmapLayer(Camera* camera, Shader* shader)
:SceneLayer(new BatchDepthmap(), camera, shader)
{}

DepthmapLayer::~DepthmapLayer()
{}

void DepthmapLayer::Render()
{
	//SETUP LIGHTS FOR SCENE
	if(!m_UpdateLights)
		return;

	Shader* m_Shader = GetShader();

	//Submit Data Once??? What if Overflow and Flushes while submitting??
	//i index of Light Groups, One Group Per Room 
	for(int i = 0; i <  m_SceneLights.size(); i++)
	{
		//Depth Shader Setup
		std::vector<PointLight*> m_PointLights = m_SceneLights[i]->GetLights();
		//index of Lights within Room they are in, to know which FBO and CubeMap to bind  
		std::vector<int> m_PointLightsRoomIndex = m_SceneLights[i]->GetLightsRoomIndex();
		for(int j = 0; j < m_PointLights.size(); j++)
		{
			m_PointLights[j]->shadowPassSetup(m_Shader, m_PointLightsRoomIndex[j]); //detpthShader is active

			m_Renderer->Begin();
			for(int i = 0; i < m_Groups.size(); i++)
			{
				m_Groups[i]->Submit(m_Renderer);
			}
			m_Renderer->End();
			m_Renderer->Flush();	
		}	
	}

	m_Groups.clear();
	m_UpdateLights = false;

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}


void DepthmapLayer::RenderKeep()
{
	if(!m_UpdateLights)
		return;

	//SETUP LIGHTS FOR SCENE
	Shader* m_Shader = GetShader();

	if(!m_Groups.empty())
	{
		m_Renderer->Begin();
		for(int i = 0; i < m_Groups.size(); i++)
		{
			m_Groups[i]->Submit(m_Renderer);
		}
		m_Renderer->End();
		m_Groups.clear();//OK SINCE ONLY SUBMIT ONCE
	}
	
	//Submit Data Once??? What if Overflow and Flushes while submitting??
	//i index of Light Groups, One Group Per Room 
	for(int i = 0; i <  m_SceneLights.size(); i++)
	{
		//Depth Shader Setup
		std::vector<PointLight*> m_PointLights = m_SceneLights[i]->GetLights();
		//index of Lights within Room they are in, to know which FBO and CubeMap to bind  
		std::vector<int> m_PointLightsRoomIndex = m_SceneLights[i]->GetLightsRoomIndex();
		for(int j = 0; j < m_PointLights.size(); j++)
		{
			m_PointLights[j]->shadowPassSetup(m_Shader, m_PointLightsRoomIndex[j]); //detpthShader is active
			m_Renderer->FlushKeep();	
		}	
	}

	m_UpdateLights = false;

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
