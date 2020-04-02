#include "batchoutline.h"

BatchOutline::BatchOutline()
{
	//Setup the dynamic Vertex Buffer 
	m_VertexBuffer = new OUTLINE_VERTEX_TYPE();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, OUTLINE_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(OUTLINE_SHADER_VERTEX_INDEX);
	glEnableVertexAttribArray(OUTLINE_SHADER_COLOR_INDEX);

	glVertexAttribPointer(OUTLINE_SHADER_VERTEX_INDEX, 4, GL_FLOAT, GL_FALSE, OUTLINE_VERTEX_SIZE, (void*) offsetof(OUTLINE_VERTEX_TYPE, OUTLINE_VERTEX_TYPE::Position));
	glVertexAttribPointer(OUTLINE_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, OUTLINE_VERTEX_SIZE, (void*) offsetof(OUTLINE_VERTEX_TYPE, OUTLINE_VERTEX_TYPE::Color));

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, OUTLINE_MAX_INDICES, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

BatchOutline::~BatchOutline()
{
	//delete [] m_IndexBuffer;

    //glDeleteBuffers(1, &m_VBO);
    //glDeleteBuffers(1, &m_IBO);
    //glDeleteVertexArrays(1, &m_VAO);
}

void BatchOutline::Begin()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBuffer = (OUTLINE_VERTEX_TYPE*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);//GL_READ_WRITE);
	//Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_IndexBuffer = (unsigned int*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);//GL_READ_WRITE);
}

void BatchOutline::End()
{
	//Vertices
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Indices
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BatchOutline::Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms) 
{
	if(m_Keep)
		return;

	int max_vertex;
	float outline_size = 0.05f;
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
			m_IndexCount += (unsigned int) (sizeof(bSquare.indices)/sizeof(bSquare.indices[0]));
			max_vertex = 4;
			vPos = bSquare.Position;
			vNorm = bSquare.Normal;
		}
		else if(asset->getGeometry() == eCube) 
		{
			m_IndexCount += (unsigned int) (sizeof(bCube.indices)/sizeof(bCube.indices[0]));
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.Normal;
		}
		else if(asset->getGeometry() == eInvertedCube) //Cube with Flipped Normals
		{
			m_IndexCount += (unsigned int) (sizeof(bCube.indices)/sizeof(bCube.indices[0]));
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.InvertedNormal;
		}

		//Stream Vertex Data
		for(int i = 0; i < max_vertex; i++)
		{
			m_VertexBuffer->Position = model_transforms[a] * glm::vec4(vPos[i] + outline_size * vNorm[i], 1.0f); //No need divide by .w 
			m_VertexBuffer->Color = vColor;
			m_VertexBuffer++;
		}

		//Stream Indices Data 
		unsigned int m_PrevIndexCount = m_IndexCount;
		for(int i = m_PrevIndexCount; i < m_IndexCount; i+=6)
		{
			*m_IndexBuffer = m_Offset;
			m_IndexBuffer++; 
			*m_IndexBuffer = m_Offset + 1;
			m_IndexBuffer++; 
			*m_IndexBuffer = m_Offset + 2;
			m_IndexBuffer++; 
			*m_IndexBuffer = m_Offset;
			m_IndexBuffer++; 
			*m_IndexBuffer = m_Offset + 2;
			m_IndexBuffer++; 
			*m_IndexBuffer = m_Offset + 3;
			m_IndexBuffer++; 

			m_Offset += 4;
		}
	}
	
}



void BatchOutline::Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms)
{	
	if(m_Keep)
		return;

	float outline_size = 0.05f;
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
				m_VertexBuffer->Position = model_transforms[a] * glm::vec4(mesh_vertices[i].Position + outline_size * mesh_vertices[i].Normal, 1.0f); //No need divide by .w
				m_VertexBuffer->Color = vColor;
				m_VertexBuffer++;
			}


			std::vector<unsigned int>& mesh_indices = model->meshes[k].indices;
			//Stream Indices Data 
			unsigned int max_Offset = 0;
			for(unsigned int i = 0; i < mesh_indices.size(); i++)
			{
				unsigned int max_temp = m_Offset + (unsigned int) mesh_indices[i]; //Assign to value since buffer is WONLY
				*m_IndexBuffer = max_temp;
				m_IndexBuffer++;

				if(max_temp >= max_Offset)
					max_Offset = max_temp;
			}

			m_IndexCount += (unsigned int) mesh_indices.size();
			m_Offset = max_Offset+1; //Move to Offset next max unused value 
		}

	}
	
}


void BatchOutline::Flush()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	glDrawElements(GL_TRIANGLES, m_IndexCount, OUTLINE_TYPE_INDICES, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_IndexCount = 0;
	m_Offset = 0;
	m_Keep = false;

}

void BatchOutline::FlushKeep()
{
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	glDrawElements(GL_TRIANGLES, m_IndexCount, OUTLINE_TYPE_INDICES, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	m_Keep = true;
}

void BatchOutline::Clear()
{
	m_IndexCount = 0;
	m_Offset = 0;
	m_Keep = false;
}

void BatchOutline::PrintIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_IndexBuffer = (unsigned int*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
	
	for(int i = 0; i < m_IndexCount; i++)
	{
		std::cout << m_IndexBuffer[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Index Count = " << m_IndexCount << std::endl;

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);	
}