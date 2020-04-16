#include "particle_system.h"

ParticleSystem::ParticleSystem()
{
	m_Cubes.reserve(PARTICLE_MAX_VERTICES); //Defined in BatchParticle header
	m_OffsetCubes = 0;
	m_Lines.reserve(PARTICLE_MAX_VERTICES);
	m_OffsetLines = 0;
}

void ParticleSystem::Bind(pShape type)
{
	if(type == m_CurrentlyBound)
		return;

	switch(type)
	{
		case pLine:
			m_Particles = &m_Lines;
			m_Offset = m_OffsetLines;
			m_CurrentlyBound = pLine;
		break;

		case pCube:
			m_Particles = &m_Cubes;
			m_Offset = m_OffsetCubes;
			m_CurrentlyBound = pCube;
		break;

		case pNone:
			m_Particles = NULL;
			m_Offset = 0;
			m_CurrentlyBound = pNone;
		break;
	}


}

void ParticleSystem::UnBind(pShape type)
{
	switch(type)
	{
		case pLine:
			m_Particles = NULL;
			m_OffsetLines = m_Offset;
			m_CurrentlyBound = pNone;
		break;

		case pCube:
			m_Particles = NULL;
			m_OffsetCubes = m_Offset;
			m_CurrentlyBound = pNone;
		break;

		case pNone:
			m_Particles = NULL;
			m_Offset = 0;
			m_CurrentlyBound = pNone;
		break;
	}	
}

void ParticleSystem::Submit(BatchParticle* renderer, pShape type)
{
	Bind(type);

	for(unsigned short i = 0; i < m_Offset; i++)
	{
		if((*m_Particles)[i]->IsActive())
		{
			(*m_Particles)[i]->Submit(renderer);
		}
	}

	UnBind(type);
}

void ParticleSystem::Update(float delta_time)
{
	// std::cout << m_Offset << std::endl;
	Bind(pLine);

	for(unsigned short i = 0; i < m_Offset; i++)
	{
		if((*m_Particles)[i]->IsActive())
		{
			(*m_Particles)[i]->Update(delta_time);
		}
	}

	UnBind(pLine);

	Bind(pCube);
	
	for(unsigned short i = 0; i < m_Offset; i++)
	{
		if((*m_Particles)[i]->IsActive())
		{
			(*m_Particles)[i]->Update(delta_time);
		}
	}
	
	UnBind(pCube);
}

void ParticleSystem::Add(Particle* base, pShape type, unsigned short count)
{
	Bind(type);
	unsigned short offset = 0;

	for(unsigned short i = 0; i < count; )
	{
		if(offset == PARTICLE_MAX_VERTICES) //Cannot add more 
			return; 

		if(offset == m_Offset)
		{
			(*m_Particles).push_back(new Particle(base)); //copy contructor
			offset++;
			m_Offset++;
			i++;
		}
		else if((*m_Particles)[offset]->IsActive())
		{
			offset++;
		}
		else
		{
			delete (*m_Particles)[offset];
			(*m_Particles)[offset] = new Particle(base); //copy constructor
			offset++;
			i++;
		}
	}	
	UnBind(type);
}

void ParticleSystem::Add(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, pShape type)
{

	Bind(type);

	glm::vec4 position = glm::vec4(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	float lifetime = 1.0f;
	float speed = 2.0f;
	float size = 0.05f;
	float rotation = 180.0f;
	//DEFAULT COLOR
	glm::vec4 col = glm::vec4(0.576f, 0.164f, 0.164f, 1.0f); 
	int r = col.x * 255.0f;
	int g = col.y * 255.0f;
	int b = col.z * 255.0f;
	int a = col.w * 255.0f;
	unsigned int color = ( a << 24 | b << 16 | g << 8 | r );	

	Particle* base = new Particle(position, direction, lifetime, color, speed, size, rotation);

	int max_vertex;
	glm::vec3* Position;
	glm::vec3* Normal;
	
	Asset* asset;

	for(int a = 0; a < assets.size(); a++)
	{
		asset = assets[a];
		if(asset->getGeometry() == eSquare)
		{
			max_vertex = 4;
			Position = bSquare.Position;
			Normal = bSquare.Normal;
		}
		else if(asset->getGeometry() == eCube) 
		{
			max_vertex = 24;
			Position = bCube.Position;
			Normal = bCube.Normal;
		}
		else if(asset->getGeometry() == eInvertedCube) //Cube with Flipped Normals
		{
			max_vertex = 24;
			Position = bCube.Position;
			Normal = bCube.InvertedNormal;
		}

		glm::mat3 normal_transform = glm::mat3(glm::transpose(glm::inverse(model_transforms[a]))); //best place?? 

		//Stream Vertex Data
		for(int i = 0; i < max_vertex; i++)
		{
			position = model_transforms[a] * glm::vec4(Position[i], 1.0f); //No need divide by .w
			direction = normal_transform * Normal[i];
			Add(new Particle(base, position, direction), type);
		}	
	}

	UnBind(type);
}

void ParticleSystem::Add(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms, pShape type)
{
	Bind(type);

	glm::vec4 position = glm::vec4(0.0f);
	glm::vec3 direction = glm::vec3(0.0f);
	float lifetime = 0.3f;
	float speed = 2.0f;
	float size = 0.05f;
	float rotation = 180.0f;
	//DEFAULT COLOR
	glm::vec4 col = glm::vec4(0.576f, 0.164f, 0.164f, 1.0f); 
	int r = col.x * 255.0f;
	int g = col.y * 255.0f;
	int b = col.z * 255.0f;
	int a = col.w * 255.0f;
	unsigned int color = ( a << 24 | b << 16 | g << 8 | r );

	Particle* base = new Particle(position, direction, lifetime, color, speed, size, rotation);

	Model* model;

	for(int a = 0; a < models.size(); a++)
	{
		model = models[a];
		glm::mat3 normal_transform = glm::mat3(glm::transpose(glm::inverse(model_transforms[a]))); 

		//Load Each Mesh in Model
		for(int k = 0; k < model->meshes.size(); k++)
		{
			std::vector<mVertex>& mesh_vertices = model->meshes[k].vertices;
			//Stream Vertex Data
			for(int i = 0; i < mesh_vertices.size(); i++)
			{	
				position = model_transforms[a] * glm::vec4(mesh_vertices[i].Position, 1.0f); //No need divide by .w
				direction = normal_transform * mesh_vertices[i].Normal;
				Add(new Particle(base, position, direction), type);

			}
		}		
	}

	UnBind(type);
}

void ParticleSystem::Clear(pShape type)
{
	Bind(type);

	for(unsigned short i = 0; i < m_Offset; i++)
		delete (*m_Particles)[i];

	m_Offset = 0;

	UnBind(type);
}
