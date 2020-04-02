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
	glEnableVertexAttribArray(PARTICLE_SHADER_COLOR_INDEX);
	glEnableVertexAttribArray(PARTICLE_SHADER_SIZE_INDEX);

	glVertexAttribPointer(PARTICLE_SHADER_VERTEX_INDEX, 4, GL_FLOAT, GL_FALSE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::Position));
	glVertexAttribPointer(PARTICLE_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::Color));
	glVertexAttribPointer(PARTICLE_SHADER_SIZE_INDEX, 1, GL_FLOAT, GL_FALSE, PARTICLE_VERTEX_SIZE, (void*) offsetof(PARTICLE_VERTEX_TYPE, PARTICLE_VERTEX_TYPE::Size));


	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

BatchParticle::~BatchParticle()
{
	//delete [] m_IndexBuffer;

    //glDeleteBuffers(1, &m_VBO);
    //glDeleteBuffers(1, &m_IBO);
    //glDeleteVertexArrays(1, &m_VAO);
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

void BatchParticle::Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms) 
{
	int max_vertex;
	float particle_size = 0.05f;
	glm::vec3 particle_offset = glm::vec3(0.0f);

	glm::vec3* vPos;
	glm::vec3* vNorm;
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
		}
		else if(asset->getGeometry() == eCube) 
		{
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.Normal;
		}
		else if(asset->getGeometry() == eInvertedCube) //Cube with Flipped Normals
		{
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.InvertedNormal;
		}

		//Stream Vertex Data
		for(int i = 0; i < max_vertex; i++)
		{
			// particle_offset = asset->getParticleOffset(vNorm[i]);
			particle_offset = vNorm[i];
			m_VertexBuffer->Position = model_transforms[a] * glm::vec4(vPos[i] + particle_offset, 1.0f); //No need divide by .w 
			m_VertexBuffer->Color = vColor;
			m_VertexBuffer->Size = particle_size;
			m_VertexBuffer++;
		}

		m_Offset += max_vertex; //vertices count	
	}
	
}



void BatchParticle::Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms)
{	
	if(m_Keep)
		return;

	float particle_size = 0.05f;
	glm::vec3 particle_offset = glm::vec3(0.0f);

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
		//Load Each Mesh in Model
		for(int k = 0; k < model->meshes.size(); k++)
		{
			std::vector<mVertex>& mesh_vertices = model->meshes[k].vertices;
			//Stream Vertex Data
			for(int i = 0; i < mesh_vertices.size(); i++)
			{
				// particle_offset = glm::normalize(mesh_vertices[i].Normal);
				particle_offset = mesh_vertices[i].Normal;
				m_VertexBuffer->Position = model_transforms[a] * glm::vec4(mesh_vertices[i].Position + particle_offset, 1.0f); //No need divide by .w
				m_VertexBuffer->Color = vColor;
				m_VertexBuffer->Size = particle_size;
				m_VertexBuffer++;
			}
			m_Offset += mesh_vertices.size(); //vertices count
		}		
	}

}


void BatchParticle::Flush()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_Offset);
	glBindVertexArray(0);
	m_Offset = 0;
	m_Keep = false;
}

void BatchParticle::FlushKeep()
{
	glBindVertexArray(m_VAO);
	glDrawArrays(GL_POINTS, 0, m_Offset);
	glBindVertexArray(0);
	m_Keep = true;
}

void BatchParticle::Clear()
{
	m_Offset = 0;
	m_Keep = false;
}
