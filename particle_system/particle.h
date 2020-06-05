#ifndef PARTICLE_H
#define PARTICLE_H

#include "../pch.h"
#include "../rendering/batch/batchparticle.h"

class BatchParticle;

class Particle
{
	public:
		Particle(glm::vec4 Position, glm::vec3 Direction, float LifeTime, unsigned int Color, float Speed, float Size, float DirRotationMax, bool OneFrame = false);
		Particle(Particle* other);
		Particle(Particle* other, glm::vec4 Position, glm::vec3 Direction);
		void Submit(BatchParticle* renderer);
		void Update(float delta_time);
		bool IsActive();

		glm::vec4 Position;
		glm::vec3 Direction;
		glm::vec2 RandomDirectionOffset; //Cone Circle, Angle
		glm::vec4 ParticleData; //Life, Speed, Size, MAX_Rotation;
		unsigned int Color; 

	private:
		bool Active; 
		bool OneFrame;
		float StartTime;
		float Time;
		float LifeTime;
		
};

#endif //PARTICLE