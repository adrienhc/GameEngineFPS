#include "particle.h"

Particle::Particle(glm::vec4 Position, glm::vec3 Direction, float LifeTime, unsigned int Color, float Speed, float Size, float DirRotationMax, bool OneFrame)
{
	float dir = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	this->Position = Position;
	this->Direction = Direction;
	this->RandomDirectionOffset = glm::vec2(dir, angle);
	ParticleData = glm::vec4(0.0f, Speed, Size, DirRotationMax);
	this->Color = Color;

	Active = true;
	this->OneFrame = OneFrame;
	StartTime = glfwGetTime();
	Time = StartTime;
	this->LifeTime = LifeTime;
	
	if(this->OneFrame)
		this->ParticleData.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	
}

Particle::Particle(Particle* other)
{
	float dir = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	this->Position = other->Position;
	this->Direction = other->Direction;
	this->RandomDirectionOffset = glm::vec2(dir, angle); //Resample own values
	this->ParticleData = other->ParticleData;
	this->Color = other->Color;

	this->Active = other->Active;
	this->OneFrame = other->OneFrame;
	this->StartTime = other->StartTime;
	this->Time = other->StartTime;

	float low = 0.5f;
	float high = 1.0f;
	float life = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(high-low)));
	this->LifeTime = other->LifeTime * life;

	if(this->OneFrame)
		this->ParticleData.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
		
}

Particle::Particle(Particle* other, glm::vec4 Position, glm::vec3 Direction)
{
	float dir = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float angle = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);

	this->Position = Position;
	this->Direction = Direction;
	this->RandomDirectionOffset = glm::vec2(dir, angle); //Resample own values
	this->ParticleData = other->ParticleData;
	this->Color = other->Color; 

	this->Active = other->Active;
	this->OneFrame = other->OneFrame;
	this->StartTime = other->StartTime;
	this->Time = other->StartTime;

	float low = 0.5f;
	float high = 1.0f;
	float life = low + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX/(high-low)));
	this->LifeTime = other->LifeTime * life;

	if(this->OneFrame)
		this->ParticleData.x = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	
}

void Particle::Submit(BatchParticle* renderer) 
{
	renderer->Submit(this);
}

void Particle::Update(float delta_time)
{
	if(OneFrame)
	{
		Active = false;
		return;
	}
	
	Time += delta_time;
	ParticleData.x = (Time - StartTime) / LifeTime;

	if(ParticleData.x >= 1.0f)
		Active = false;
}

bool Particle::IsActive()
{
	return Active;
}