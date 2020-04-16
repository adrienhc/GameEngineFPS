#include "batchrenderer.h"

BatchRenderer::BatchRenderer()
{
	//Setup the dynamic Vertex Buffer 
	m_VertexBuffer = new RENDERER_VERTEX_TYPE();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, RENDERER_BUFFER_SIZE, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(RENDERER_SHADER_VERTEX_INDEX);
	glEnableVertexAttribArray(RENDERER_SHADER_NORMAL_INDEX);

	glEnableVertexAttribArray(RENDERER_SHADER_MODTR_INDEX_0);
	glEnableVertexAttribArray(RENDERER_SHADER_MODTR_INDEX_1);
	glEnableVertexAttribArray(RENDERER_SHADER_MODTR_INDEX_2);
	glEnableVertexAttribArray(RENDERER_SHADER_MODTR_INDEX_3);

	glEnableVertexAttribArray(RENDERER_SHADER_TEXCOORD_INDEX);
	glEnableVertexAttribArray(RENDERER_SHADER_TEXDIFFID_INDEX);
	glEnableVertexAttribArray(RENDERER_SHADER_TEXSPECID_INDEX);
	glEnableVertexAttribArray(RENDERER_SHADER_LIGHTING_INDEX);
	glEnableVertexAttribArray(RENDERER_SHADER_COLOR_INDEX);

	glVertexAttribPointer(RENDERER_SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::Position));
	glVertexAttribPointer(RENDERER_SHADER_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::Normal));
	
	glVertexAttribPointer(RENDERER_SHADER_MODTR_INDEX_0, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::ModelTransform));
	glVertexAttribPointer(RENDERER_SHADER_MODTR_INDEX_1, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) (offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::ModelTransform) + (1 * sizeof(glm::vec4))));
	glVertexAttribPointer(RENDERER_SHADER_MODTR_INDEX_2, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) (offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::ModelTransform) + (2 * sizeof(glm::vec4))));
	glVertexAttribPointer(RENDERER_SHADER_MODTR_INDEX_3, 4, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) (offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::ModelTransform) + (3 * sizeof(glm::vec4))));

	glVertexAttribPointer(RENDERER_SHADER_TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::TexCoord));
	glVertexAttribPointer(RENDERER_SHADER_TEXDIFFID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::TexDiffID));
	glVertexAttribPointer(RENDERER_SHADER_TEXSPECID_INDEX, 1, GL_FLOAT, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::TexSpecID));
	glVertexAttribPointer(RENDERER_SHADER_LIGHTING_INDEX, 4, GL_UNSIGNED_BYTE, GL_FALSE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::Lighting));
	glVertexAttribPointer(RENDERER_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, RENDERER_VERTEX_SIZE, (void*) offsetof(RENDERER_VERTEX_TYPE, RENDERER_VERTEX_TYPE::Color));
	


	/*unsigned int m_IndexBuffer[RENDERER_MAX_INDICES];

	unsigned int offset = 0;
	
	for(int i = 0; i < RENDERER_MAX_INDICES-6; i+=6)
	{
		m_IndexBuffer[i] = offset; 
		m_IndexBuffer[i+1] = offset + 1;
		m_IndexBuffer[i+2] = offset + 2;

		m_IndexBuffer[i+3] = offset;
		m_IndexBuffer[i+4] = offset + 2;
		m_IndexBuffer[i+5] = offset + 3;

		offset += 4;
	}
	*/
	

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_IndexBuffer), m_IndexBuffer, GL_STATIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, RENDERER_MAX_INDICES, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

BatchRenderer::~BatchRenderer()
{
	//delete [] m_IndexBuffer;

    //glDeleteBuffers(1, &m_VBO);
    //glDeleteBuffers(1, &m_IBO);
    //glDeleteVertexArrays(1, &m_VAO);
}

void BatchRenderer::Begin()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBuffer = (RENDERER_VERTEX_TYPE*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);//GL_READ_WRITE);
	//if(m_VertexBuffer == NULL)
	//	std::cout << "R V NULL" << std::endl;
	//else 
	//	std::cout << "R V NOT NULL" << std::endl;
	
	//Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_IndexBuffer = (RENDERER_INDICES_TYPE*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);// GL_WRITE_ONLY GL_READ_WRITE);
}

void BatchRenderer::End()
{
	//Vertices
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//Indices
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BatchRenderer::Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms) 
{
	if(m_Keep)
		return;

	int max_vertex;
	//glm::mat3 normal_transform = glm::mat3(glm::transpose(glm::inverse(model_transform))); //best place?? 

	glm::vec3* vPos;
	glm::vec3* vNorm;
	glm::vec2* vTexCoord;

	Asset* asset;

	for(int a = 0; a < assets.size(); a++)
	{
		asset = assets[a];

		unsigned int vTexID = asset->getMaterial()->getTextureID(); 
		unsigned int vLighting = asset->getMaterial()->getLightingPacked();
		unsigned int vColor;

		float vTexSlot = 0.0f;
		if(vTexID > 0)
		{
			bool found = false;
			for(int i = 0; i < m_TextureID.size(); i++)
			{
				if(m_TextureID[i] == vTexID)
				{
					vTexSlot = (float) i;
					found = true;
					break;
				}
			}

			if(!found)
			{
				if(m_TextureID.size() >= MAX_TEXTURE_SLOTS)
				{
					//Also do when reached Max Buffer size? 
					End();
					Flush();
					Begin();
					//issue here? want m_TextureID size to be 0?? 
				}

				m_TextureID.push_back(vTexID);
				vTexSlot = (float)(m_TextureID.size() - 1);
			}
			
			vTexSlot += 1.0f;
		}
		else
		{
			vColor = asset->getMaterial()->getColorPacked();
		}

		unsigned int m_PrevIndexCount = m_IndexCount;

		if(asset->getGeometry() == eSquare)
		{
			m_IndexCount += (unsigned int) (sizeof(bSquare.indices)/sizeof(bSquare.indices[0]));
			max_vertex = 4;
			vPos = bSquare.Position;
			vNorm = bSquare.Normal;
			vTexCoord = bSquare.TexCoord;
		}
		else if(asset->getGeometry() == eCube) 
		{
			m_IndexCount += (unsigned int) (sizeof(bCube.indices)/sizeof(bCube.indices[0]));
			//std::cout << "CUBE " << (unsigned int) (sizeof(bCube.indices)/sizeof(bCube.indices[0])) << std::endl; //36
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.Normal;
			vTexCoord = bCube.TexCoord;
		}
		else if(asset->getGeometry() == eInvertedCube) //Cube with Flipped Normals
		{
			m_IndexCount += (unsigned int) (sizeof(bCube.indices)/sizeof(bCube.indices[0]));
			max_vertex = 24;
			vPos = bCube.Position;
			vNorm = bCube.InvertedNormal;
			vTexCoord = bCube.TexCoord;
		}

		//Stream Vertex Data
		for(int i = 0; i < max_vertex; i++)
		{
			m_VertexBuffer->Position = /*model_transform */ vPos[i]; //No need divide by .w 
			m_VertexBuffer->Normal = /*normal_transform */ vNorm[i];
			m_VertexBuffer->ModelTransform = model_transforms[a];
			m_VertexBuffer->TexCoord = vTexCoord[i];
			m_VertexBuffer->TexDiffID = vTexSlot;
			m_VertexBuffer->TexSpecID = 0.0f;
			m_VertexBuffer->Lighting = vLighting;
			m_VertexBuffer->Color = vColor;
			m_VertexBuffer++;
		}

		//Stream Indices Data 
		for(int i = m_PrevIndexCount; i < m_IndexCount; i+=6)
		{
			/*m_IndexBuffer[i] = m_Offset; 
			m_IndexBuffer[i+1] = m_Offset + 1;
			m_IndexBuffer[i+2] = m_Offset + 2;

			m_IndexBuffer[i+3] = m_Offset;
			m_IndexBuffer[i+4] = m_Offset + 2;
			m_IndexBuffer[i+5] = m_Offset + 3;*/

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



void BatchRenderer::Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms)
{
	if(m_Keep)
		return;

	//glm::mat3 normal_transform = glm::mat3(glm::transpose(glm::inverse(model_transform))); //best place?? 

	//DEFAULT COLOR
	//DIFFUSE RED IF NO TEXTURE
	glm::vec4 col = glm::vec4(1.0f, 0.2f, 0.2f, 1.0f); 
	int r = col.x * 255.0f;
	int g = col.y * 255.0f;
	int b = col.z * 255.0f;
	int a = col.w * 255.0f;
	unsigned int vColor = ( a << 24 | b << 16 | g << 8 | r );
	
	//DEFAULT Lighting
	//FULL BLAST IF NOTHING 
	int am = 1.0f * 255.0f; //ow found in Diff Texture
	int di = 1.0f * 255.0f; //ow found in Diff Texture
	int sp = 1.0f * 255.0f; //ow found in Spec Texture
	int sh = 32.0f; //DEFAULT SHININESS FOR MODELS -- NOT IN A TEXTURE -- ALWAYS USED
	unsigned int vLighting = ( sh << 24 | sp << 16 | di << 8 | am );

	Model* model;

	for(int a = 0; a < models.size(); a++)
	{
		model = models[a];

		//Load Each Mesh in Model
		for(int k = 0; k < model->meshes.size(); k++)
		{
			//Extract Texture IDs
			//Limit to One Diffuse, One Specular
			std::vector<mTexture>& mesh_textures = model->meshes[k].textures;
			unsigned int vTexDiffID = 0;
			unsigned int vTexSpecID = 0;
			int textureCount = 0; 
			for(int i = 0; i < mesh_textures.size(); i++)
			{
				//Only take first occuring diffuse and specular maps
				std::string name = mesh_textures[i].type;
				if (vTexDiffID == 0 && name == "texture_diffuse")
				{
					vTexDiffID = mesh_textures[i].id;
					textureCount++;
				}
				else if (vTexSpecID == 0 && name == "texture_specular")
				{
					vTexSpecID = mesh_textures[i].id;
					textureCount++;
				}
			}

			//ALL shall be found at once if they exist! 
			float vTexDiffSlot = 0.0f;
			bool foundDiff = false;
			if(vTexDiffID > 0)
			{
				for(int i = 0; i < m_TextureID.size(); i++)
				{
					if(m_TextureID[i] == vTexDiffID)
					{
						vTexDiffSlot = (float) i;
						foundDiff = true;
						break;
					}
				}
			}

			float vTexSpecSlot = 0.0f;
			bool foundSpec = false;
			if(vTexSpecID > 0)
			{
				for(int i = 0; i < m_TextureID.size(); i++)
				{
					if(m_TextureID[i] == vTexSpecID)
					{
						vTexSpecSlot = (float) i;
						foundSpec = true;
						break;
					}
				}
			}

			if((vTexDiffID > 0 && !foundDiff) || (vTexSpecID > 0 && !foundSpec)) //if missing at least one that was needed
			{
				if(m_TextureID.size() < textureCount) //for empty or very small texture buffer 
				{
					if(vTexDiffID > 0)
					{
						m_TextureID.push_back(vTexDiffID);
						vTexDiffSlot = (float)(m_TextureID.size() - 1);
					}

					if(vTexSpecID > 0)
					{
						m_TextureID.push_back(vTexSpecID);
						vTexSpecSlot = (float)(m_TextureID.size() -1);
					}	
				}
				else if(m_TextureID.size() >= (MAX_TEXTURE_SLOTS - textureCount)) //check if space for all needed
				{
					//Also do when reached Max Buffer size? 
					End();
					Flush();
					Begin();
					//issue here? want m_TextureID size to be 0?? 

					if(vTexDiffID > 0)
					{
						m_TextureID.push_back(vTexDiffID);
						vTexDiffSlot = (float)(m_TextureID.size() - textureCount);
						textureCount--; //so that next check will have a different offset, next slot in array
					}

					if(vTexSpecID > 0)
					{
						m_TextureID.push_back(vTexSpecID);
						vTexSpecSlot = (float)(m_TextureID.size() - textureCount);
					}
				}
			}
			

			if(vTexDiffID > 0)
				vTexDiffSlot += 1.0f;
			if(vTexSpecID > 0)
				vTexSpecSlot += 1.0f;


			std::vector<mVertex>& mesh_vertices = model->meshes[k].vertices;
			//Stream Vertex Data
			for(int i = 0; i < mesh_vertices.size(); i++)
			{
				m_VertexBuffer->Position = /*model_transform */ mesh_vertices[i].Position; //No need divide by .w			
				m_VertexBuffer->Normal = /*normal_transform */ mesh_vertices[i].Normal;
				m_VertexBuffer->ModelTransform = model_transforms[a];
				m_VertexBuffer->TexCoord = mesh_vertices[i].TexCoords;
				m_VertexBuffer->TexDiffID = vTexDiffSlot;
				m_VertexBuffer->TexSpecID = vTexSpecSlot;
				m_VertexBuffer->Lighting = vLighting;
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


void BatchRenderer::Flush()
{
	for(int i = 0; i < m_TextureID.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureID[i]);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	glDrawElements(GL_TRIANGLES, m_IndexCount, RENDERER_GL_INDICES_TYPE, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_IndexCount = 0;
	m_Offset = 0;
	m_Keep = false;
}

void BatchRenderer::FlushKeep()
{
	for(int i = 0; i < m_TextureID.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureID[i]);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	glDrawElements(GL_TRIANGLES, m_IndexCount, RENDERER_GL_INDICES_TYPE, NULL);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_Keep = true;
}

void BatchRenderer::Clear()
{
	m_IndexCount = 0;
	m_Offset = 0;
	m_Keep = false;
}

void BatchRenderer::PrintIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_IndexBuffer = (RENDERER_INDICES_TYPE*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
	
	for(int i = 0; i < m_IndexCount; i++)
	{
		std::cout << m_IndexBuffer[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Index Count = " << m_IndexCount << std::endl;

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}