#include "abstractlayer.h"


AbstractLayer::AbstractLayer(BatchAbstract* renderer, Camera* camera, Shader* shader)
{
	m_Renderer = renderer;
	m_Camera = camera;
	m_Shader = shader;
}

AbstractLayer::~AbstractLayer()
{
	//for(int i = 0; i < m_Groups.size(); i++)
	//	delete m_Groups[i];	
}

void AbstractLayer::Add(Group* group)
{
	m_Groups.push_back(group);
}

//Function to Add light information and cube maps as well! -- up to derived class to do 

Shader* AbstractLayer::GetShader()
{
	return m_Shader;
}

Camera* AbstractLayer::GetCamera()
{
	return m_Camera;
}

void AbstractLayer::Render()
{
	m_Shader->use();
	m_Shader->setCamera(m_Camera);
	
	m_Renderer->Begin();
	for(int i = 0; i < m_Groups.size(); i++)
	{
		//std::cout << "Submit Group" << std::endl;
		m_Groups[i]->Submit(m_Renderer);
	}
	m_Renderer->End();
	m_Renderer->Flush();

	m_Groups.clear(); //Flush Groups as Well -- Can Stream Groups to Layer
	// std::cout << "DONE RENDERING \n" << std::endl;
}

void AbstractLayer::RenderKeep()
{	
	if(!m_Groups.empty())
	{
		m_Renderer->Begin();
		for(int i = 0; i < m_Groups.size(); i++)
		{
			//std::cout << "Submit Group" << std::endl;
			m_Groups[i]->Submit(m_Renderer);
		}
		m_Renderer->End();	
		m_Groups.clear(); //OK since only need upload Data Once
	}

	m_Shader->setCamera(m_Camera);
	m_Renderer->FlushKeep();
}

void AbstractLayer::Clear()
{
	m_Groups.clear();
	m_Renderer->Clear();
}
