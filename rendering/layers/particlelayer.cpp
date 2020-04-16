#include "particlelayer.h"

ParticleLayer::ParticleLayer(Camera* camera, Shader* shader, ParticleSystem* particle_system, pShape type)
:SceneLayer(new BatchParticle(), camera, shader)
{
	shader->use();
	shader->setInt("noiseRGB", 0);

	m_ParticleSystem = particle_system;
	m_type = type;
}

ParticleLayer::~ParticleLayer()
{}


void ParticleLayer::Render()
{
	//Setup Shader
	Shader* m_Shader = GetShader();
	m_Shader->use();
	m_Shader->setCamera(GetCamera());
	m_Shader->setFloat("Time", glfwGetTime());
	noiseRGB->Bind();

	if(m_type == pCube)
		ShaderSetup(); //SETUP LIGHTS

	//Submit Geometry to Particle System
	if(!m_Groups.empty())
	{
		for(int i = 0; i < m_Groups.size(); i++)
		{
			m_Groups[i]->Submit(m_ParticleSystem, m_type);
		}
		m_Groups.clear();
	}

	BatchParticle* m_Renderer = dynamic_cast<BatchParticle*> (GetRenderer());
	m_Renderer->Begin();
	//Submit Particle System to Renderer
	m_ParticleSystem->Submit(m_Renderer, m_type);
	m_Renderer->End();
	m_Renderer->Flush();

	noiseRGB->Unbind();
}

void ParticleLayer::RenderKeep()
{
	Render();
}

