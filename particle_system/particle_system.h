#ifndef PARTICLE_SYSTEM
#define PARTICLE_SYSTEM

#include "../pch.h"
#include "particle.h"
#include "../rendering/batch/batchparticle.h"
#include "../geometry/geometry.h"

enum pShape {pLine, pPoint, pCube, pNone};

class ParticleSystem
{
	public:
		ParticleSystem();
		~ParticleSystem();
		void Submit(BatchParticle* renderer, pShape type);
		void Add(Particle* base, pShape type, unsigned short count = 1);
		void Add(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, pShape type);
		void Add(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms, pShape type);
		void Update(float delta_time);
		void Clear(pShape type);

	private:
		void Bind(pShape type);
		void UnBind(pShape type);

		std::vector<Particle*>* m_Particles = NULL;
		unsigned short m_Offset = 0;
		pShape m_CurrentlyBound = pNone;
		
		std::vector<Particle*> m_Cubes;
		unsigned short m_OffsetCubes;
		std::vector<Particle*> m_Points;
		unsigned short m_OffsetPoints;
		std::vector<Particle*> m_Lines;
		unsigned short m_OffsetLines;
		

		//Basic Geometry Info -- make the class static?? 
		batchSquare bSquare;
		batchCube bCube;

};

#endif //PARTICLE_SYSTEM