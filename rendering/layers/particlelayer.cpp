#include "particlelayer.h"

ParticleLayer::ParticleLayer(Camera* camera, Shader* shader, ParticleSystem* particle_system, pShape type)
:SceneLayer(new BatchParticle(), camera, shader)
{
	shader->use();
	shader->setInt("noiseRGB", 0);
	shader->setInt("smokeTexture", 1);
	m_ParticleSystem = particle_system;
	m_type = type;
}

ParticleLayer::~ParticleLayer()
{
	delete noiseRGB;
	delete smokeTexture;
}


void ParticleLayer::Render()
{
   glEnable(GL_BLEND);
   switch(m_type)
   {
   		case pPoint:
   			glBlendFunc(GL_SRC_ALPHA, GL_ONE);
   		break;
   		default:
   			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
   		break;
   }
   
   glDepthMask(0);

	//Setup Shader
	Shader* m_Shader = GetShader();
	m_Shader->use();
	Camera* m_Camera = GetCamera();
	m_Shader->setCamera(m_Camera);
	// if(m_type == pPoint)
	m_Shader->setVec3("cameraRight", m_Camera->Right);
	m_Shader->setVec3("cameraUp", m_Camera->Up);
	m_Shader->setFloat("Time", glfwGetTime());
	noiseRGB->Bind(0);
	smokeTexture->Bind(1);


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

	noiseRGB->Unbind(0);
	smokeTexture->Unbind(1);

   glDepthMask(1);   
   glDisable(GL_BLEND); 
}

void ParticleLayer::RenderKeep()
{
	Render();
}

