#include "particlelayer.h"

ParticleLayer::ParticleLayer(Camera* camera, Shader* shader)
:SceneLayer(new BatchParticle(), camera, shader)
{}

ParticleLayer::~ParticleLayer()
{}

/*
void ParticleLayer::Render()
{
	//SETUP LIGHTS FOR SCENE
	Shader* m_Shader = GetShader();
	m_Shader->use();
	
	//RENDER SCENE
	SceneLayer::Render();
}
*/
