#include "batchparticle.h"

BatchParticle::BatchParticle()
{
	//Setup the dynamic Vertex Buffer 
	m_VertexBuffer = new PARTICLE_VERTEX_TYPE();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, PARTICLE_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(PARTICLE_SHADER_VERTEX_INDEX);
	glEnableVertexAttribArray(PARTICLE_SHADER_NORMAL_INDEX);
	glEnableVertexAttribArray(PARTICLE_SHADER_TEXCOORD_INDEX);
	glEnableVertexAttribArray(PARTICLE_SHADER_PARTICLEDATA_INDEX);
	glEnableVertexAttribArray(PARTICLE_SHADER_COLOR_INDEX);

	glVertexAttribPointer(PARTICLE_SHADER_VERTEX_INDEX, 4, GL_FLOAT, GL_FALSE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::Position));
	glVertexAttribPointer(PARTICLE_SHADER_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::Normal));
	glVertexAttribPointer(PARTICLE_SHADER_TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::TexCoord));
	glVertexAttribPointer(PARTICLE_SHADER_PARTICLEDATA_INDEX, 4, GL_FLOAT, GL_FALSE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::ParticleData));
	glVertexAttribPointer(PARTICLE_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::Color));

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

BatchParticle::~BatchParticle()
{
	//delete [] m_IndexBuffer;
	delete m_VertexBuffer;
	delete m_IndexBuffer;

	glBindVertexArray(m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_VAO);
}

void BatchParticle::Begin()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBuffer = (PARTICLE_VERTEX_TYPE*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);//GL_READ_WRITE);
}

void BatchParticle::End()
{
	//Vertices
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void BatchParticle::Submit(Particle* particle)
{
	// if(m_Keep)
		// return;
	m_VertexBuffer->Position = particle->Position; 
	m_VertexBuffer->Normal = particle->Direction;
	m_VertexBuffer->TexCoord = particle->RandomDirectionOffset;
	m_VertexBuffer->ParticleData = particle->ParticleData;
	m_VertexBuffer->Color = particle->Color;
	m_VertexBuffer++;
	m_Offset++;
}


void BatchParticle::Flush()
{
	glLineWidth(3.0f);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_Offset);
	glBindVertexArray(0);
	m_Offset = 0;
	m_Keep = false;
}

void BatchParticle::FlushKeep() //NOTUSED AT THE MOMENT
{
	glLineWidth(3.0f);
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_Offset);
	glBindVertexArray(0);
	m_Keep = true; //Not used 
}

void BatchParticle::Clear()
{
	m_Offset = 0;
	m_Keep = false;
}


/*
void BatchParticle::Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms) 
{
	if(m_Keep)
		return;

	srand(123); //GIVE EACH ASSET/MODEL/PARTICLE EMITTER A RANDOM SEED AND THEY KEEP IT!!

	int max_vertex;

	float particle_life = 0.0f;
	float particle_speed = 2.0f;
	float particle_size = 0.05f;
	float particle_rotation = 180.0f;
	glm::vec4 particle_data = glm::vec4(particle_life, particle_speed, particle_size, particle_rotation);

	glm::vec3* vPos;
	glm::vec3* vNorm;
	glm::vec2* vTexCoord;
	glm::vec4 col = glm::vec4(0.576f, 0.164f, 0.164f, 1.0f); 
	int r = col.x * 255.0f;
	int g = col.y * 255.0f;
	int b = col.z * 255.0f;
	int a = col.w * 255.0f;
	unsigned int vColor = ( a << 24 | b << 16 | g << 8 | r );
	//vColor = asset->getMaterial()->getColorPacked();

	Asset* asset;

	for(int a = 0; a < assets.size(); a++)
	{
		asset = assets[a];
		if(asset->getGeometry() == eSquare)
		{
			max_vertex = 4;
			vPos = bSquare.Position;
			vNorm = bSquare.Normal;
			vTexCoord = bSquare.TexCoord;
		}
		else if(asset->getGeometry() == eCube) 
		{
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.Normal;
			vTexCoord = bCube.TexCoord;
		}
		else if(asset->getGeometry() == eInvertedCube) //Cube with Flipped Normals
		{
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.InvertedNormal;
			vTexCoord = bCube.TexCoord;
		}

		glm::mat3 normal_transform = glm::mat3(glm::transpose(glm::inverse(model_transforms[a]))); //best place?? 

		//Stream Vertex Data
		for(int i = 0; i < max_vertex; i++)
		{
			float s = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
			m_VertexBuffer->TexCoord = glm::vec2(s, t);

			m_VertexBuffer->Position = model_transforms[a] * glm::vec4(vPos[i], 1.0f); //No need divide by .w 
			m_VertexBuffer->Normal = normal_transform * vNorm[i];
			m_VertexBuffer->ParticleData = particle_data;
			m_VertexBuffer->Color = vColor;
			m_VertexBuffer++;
		}

		m_Offset += max_vertex; //vertices count	
	}
	
}



void BatchParticle::Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms)
{	
	if(m_Keep)
		return;

	srand(123); //GIVE EACH ASSET/MODEL/PARTICLE EMITTER A RANDOM SEED AND THEY KEEP IT!!

	float particle_life = 0.5f;
	float particle_speed = 2.0f;
	float particle_size = 0.05f;
	float particle_rotation = 180.0f;
	glm::vec4 particle_data = glm::vec4(particle_life, particle_speed, particle_size, particle_rotation);

	//DEFAULT COLOR
	//DIFFUSE RED IF NO TEXTURE
	glm::vec4 col = glm::vec4(0.576f, 0.164f, 0.164f, 1.0f); 
	int r = col.x * 255.0f;
	int g = col.y * 255.0f;
	int b = col.z * 255.0f;
	int a = col.w * 255.0f;
	unsigned int vColor = ( a << 24 | b << 16 | g << 8 | r );
	
	Model* model;

	for(int a = 0; a < models.size(); a++)
	{
		model = models[a];
		glm::mat3 normal_transform = glm::mat3(glm::transpose(glm::inverse(model_transforms[a]))); //best place?? 

		//Load Each Mesh in Model
		for(int k = 0; k < model->meshes.size(); k++)
		{
			std::vector<mVertex>& mesh_vertices = model->meshes[k].vertices;
			//Stream Vertex Data
			for(int i = 0; i < mesh_vertices.size(); i++)
			{
				//if(mesh_vertices[i].TexCoords.x < 0.0 || mesh_vertices[i].TexCoords.y < 0.0)
				//{
				float s = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				float t = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
				m_VertexBuffer->TexCoord = glm::vec2(s, t);
				//}
				
				m_VertexBuffer->Position = model_transforms[a] * glm::vec4(mesh_vertices[i].Position, 1.0f); //No need divide by .w
				m_VertexBuffer->Normal = normal_transform * mesh_vertices[i].Normal;
				m_VertexBuffer->ParticleData = particle_data;
				m_VertexBuffer->Color = vColor;
				m_VertexBuffer++;
			}
			m_Offset += mesh_vertices.size(); //vertices count
		}		
	}

}
*/