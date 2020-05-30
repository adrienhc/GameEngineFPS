#include "batchinstanced.h"

BatchInstanced::BatchInstanced()
{
	//Setup the dynamic Vertex Buffer 
	m_VertexBufferFixed = new INSTANCED_VERTEX_TYPE_FIXED();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, INSTANCED_BUFFER_SIZE_FIXED, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(INSTANCED_SHADER_VERTEX_INDEX);
	glEnableVertexAttribArray(INSTANCED_SHADER_NORMAL_INDEX);
	glEnableVertexAttribArray(INSTANCED_SHADER_TEXCOORD_INDEX);

	glVertexAttribPointer(INSTANCED_SHADER_VERTEX_INDEX, 3, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_FIXED, (void*) offsetof(INSTANCED_VERTEX_TYPE_FIXED, INSTANCED_VERTEX_TYPE_FIXED::Position));
	glVertexAttribPointer(INSTANCED_SHADER_NORMAL_INDEX, 3, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_FIXED, (void*) offsetof(INSTANCED_VERTEX_TYPE_FIXED, INSTANCED_VERTEX_TYPE_FIXED::Normal));	
	glVertexAttribPointer(INSTANCED_SHADER_TEXCOORD_INDEX, 2, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_FIXED, (void*) offsetof(INSTANCED_VERTEX_TYPE_FIXED, INSTANCED_VERTEX_TYPE_FIXED::TexCoord));


	m_VertexBufferVariable = new INSTANCED_VERTEX_TYPE_VARIABLE();

	glGenBuffers(1, &m_VBO_VAR);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_VAR);
	glBufferData(GL_ARRAY_BUFFER, INSTANCED_BUFFER_SIZE_VARIABLE, NULL, GL_DYNAMIC_DRAW);

	glEnableVertexAttribArray(INSTANCED_SHADER_MODTR_INDEX_0);
	glEnableVertexAttribArray(INSTANCED_SHADER_MODTR_INDEX_1);
	glEnableVertexAttribArray(INSTANCED_SHADER_MODTR_INDEX_2);
	glEnableVertexAttribArray(INSTANCED_SHADER_MODTR_INDEX_3);
	glEnableVertexAttribArray(INSTANCED_SHADER_TEXSCALE_INDEX);
	glEnableVertexAttribArray(INSTANCED_SHADER_TEXDIFFID_INDEX);
	glEnableVertexAttribArray(INSTANCED_SHADER_TEXSPECID_INDEX);
	glEnableVertexAttribArray(INSTANCED_SHADER_LIGHTING_INDEX);
	glEnableVertexAttribArray(INSTANCED_SHADER_COLOR_INDEX);

	glVertexAttribPointer(INSTANCED_SHADER_MODTR_INDEX_0, 4, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::ModelTransform));
	glVertexAttribPointer(INSTANCED_SHADER_MODTR_INDEX_1, 4, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) (offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::ModelTransform) + sizeof(glm::vec4)));
	glVertexAttribPointer(INSTANCED_SHADER_MODTR_INDEX_2, 4, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) (offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::ModelTransform) + 2 * sizeof(glm::vec4)));
	glVertexAttribPointer(INSTANCED_SHADER_MODTR_INDEX_3, 4, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) (offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::ModelTransform) + 3 * sizeof(glm::vec4)));
	glVertexAttribPointer(INSTANCED_SHADER_TEXSCALE_INDEX, 2, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::TexScale));
	glVertexAttribPointer(INSTANCED_SHADER_TEXDIFFID_INDEX, 1, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::TexDiffID));
	glVertexAttribPointer(INSTANCED_SHADER_TEXSPECID_INDEX, 1, GL_FLOAT, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::TexSpecID));
	glVertexAttribPointer(INSTANCED_SHADER_LIGHTING_INDEX, 4, GL_UNSIGNED_BYTE, GL_FALSE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::Lighting));
	glVertexAttribPointer(INSTANCED_SHADER_COLOR_INDEX, 4, GL_UNSIGNED_BYTE, GL_TRUE, INSTANCED_VERTEX_SIZE_VARIABLE, (void*) offsetof(INSTANCED_VERTEX_TYPE_VARIABLE, INSTANCED_VERTEX_TYPE_VARIABLE::Color));
	
	glVertexAttribDivisor(INSTANCED_SHADER_MODTR_INDEX_0, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_MODTR_INDEX_1, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_MODTR_INDEX_2, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_MODTR_INDEX_3, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_TEXSCALE_INDEX, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_TEXDIFFID_INDEX, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_TEXSPECID_INDEX, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_LIGHTING_INDEX, 1);
	glVertexAttribDivisor(INSTANCED_SHADER_COLOR_INDEX, 1);
	

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, INSTANCED_BUFFER_SIZE_INDICES, NULL, GL_DYNAMIC_DRAW);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

}

BatchInstanced::~BatchInstanced()
{
	//delete [] m_IndexBuffer;

    delete m_VertexBufferFixed;
    delete m_VertexBufferVariable;
	delete m_IndexBuffer;

	glBindVertexArray(m_VAO);
    glDeleteBuffers(1, &m_VBO);
    glDeleteBuffers(1, &m_IBO);
    glBindVertexArray(0);
    glDeleteVertexArrays(1, &m_VAO);
}

void BatchInstanced::Begin()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	m_VertexBufferFixed = (INSTANCED_VERTEX_TYPE_FIXED*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);//GL_READ_WRITE);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_VAR);
	m_VertexBufferVariable = (INSTANCED_VERTEX_TYPE_VARIABLE*) glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);

	//Indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_IndexBuffer = (INSTANCED_INDICES_TYPE*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);// GL_WRITE_ONLY GL_READ_WRITE);
}

void BatchInstanced::End()
{
	//Vertices
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO_VAR);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//Indices
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void BatchInstanced::Submit(std::vector<Asset*>& assets, std::vector<glm::mat4>& model_transforms, std::vector<glm::vec2>& texture_scalings) 
{
	if(m_Keep)
		return;
	
	Asset* asset;

	for(int a = 0; a < assets.size(); a++)
	{
		asset = assets[a];

		if(First)
		{
			First = false;
			int max_vertex;
			glm::vec3* vPos;
			glm::vec3* vNorm;
			glm::vec2* vTexCoord;
			
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

			//Stream Vertex Fixed Data ON FIRST OCCURENCE
			for(int i = 0; i < max_vertex; i++)
			{
				m_VertexBufferFixed->Position = vPos[i]; //No need divide by .w 
				m_VertexBufferFixed->Normal = vNorm[i];
				m_VertexBufferFixed->TexCoord = vTexCoord[i]; //TO DO: EITHER MOVE TO VARIABLE (PREFERABLE) OR CREATE NEW VERTEX SHADER THAT TAKES IN TEXTURE SCALING (EASIER)
				m_VertexBufferFixed++;
			}

			//Stream Indices Data ON FIRST OCCURENCE
			for(int i = 0; i < m_IndexCount; i+=6)
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

		//Stream Vertex Variable Data ONCE PER ASSET
		m_VertexBufferVariable->ModelTransform = model_transforms[a];
		m_VertexBufferVariable->TexScale = texture_scalings[a];
		m_VertexBufferVariable->TexDiffID = vTexSlot;
		m_VertexBufferVariable->TexSpecID = 0.0f;
		m_VertexBufferVariable->Lighting = vLighting;
		m_VertexBufferVariable->Color = vColor;
		m_VertexBufferVariable++;

		m_InstancedCount++;
	}
	
}



void BatchInstanced::Submit(std::vector<Model*>& models, std::vector<glm::mat4>& model_transforms)
{
	if(m_Keep)
		return;

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
			if(First)
			{
				std::vector<mVertex>& mesh_vertices = model->meshes[k].vertices;
				//Stream Vertex Data
				for(int i = 0; i < mesh_vertices.size(); i++)
				{
					m_VertexBufferFixed->Position = mesh_vertices[i].Position; //No need divide by .w			
					m_VertexBufferFixed->Normal = mesh_vertices[i].Normal;
					m_VertexBufferFixed->TexCoord = mesh_vertices[i].TexCoords;
					m_VertexBufferFixed++;
				}


				std::vector<unsigned int>& mesh_indices = model->meshes[k].indices;
				//Stream Indices Data 
				INSTANCED_INDICES_TYPE max_Offset = 0;
				for(unsigned int i = 0; i < mesh_indices.size(); i++)
				{
					INSTANCED_INDICES_TYPE max_temp = m_Offset + (INSTANCED_INDICES_TYPE) mesh_indices[i]; //Assign to value since buffer is WONLY
					*m_IndexBuffer = max_temp;
					m_IndexBuffer++;

					if(max_temp >= max_Offset)
						max_Offset = max_temp;
				}

				m_IndexCount += (INSTANCED_INDICES_TYPE) mesh_indices.size();
				m_Offset = max_Offset+1; //Move to Offset next max unused value 
			}


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
				if(m_TextureID.size() >= (MAX_TEXTURE_SLOTS - textureCount)) //check if space for all needed
				{
					//Also do when reached Max Buffer size? 
					End();
					Flush();
					Begin();
					//issue here? want m_TextureID size to be 0?? 
				}

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
			

			if(vTexDiffID > 0)
				vTexDiffSlot += 1.0f;
			if(vTexSpecID > 0)
				vTexSpecSlot += 1.0f;


			//Stream Model Transform Data
			m_VertexBufferVariable->ModelTransform = model_transforms[a];
			m_VertexBufferVariable->TexDiffID = vTexDiffSlot;
			m_VertexBufferVariable->TexSpecID = vTexSpecSlot;
			m_VertexBufferVariable->Lighting = vLighting;
			m_VertexBufferVariable->Color = vColor;
			m_VertexBufferVariable++;

			m_InstancedCount++;
		}
	

		if(First)
			First = false;
	}
}


void BatchInstanced::Flush()
{
	for(int i = 0; i < m_TextureID.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureID[i]);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	glDrawElementsInstanced(GL_TRIANGLES, m_IndexCount, INSTANCED_GL_INDICES_TYPE, NULL, m_InstancedCount);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_Keep = false;
	m_IndexCount = 0;
	m_Offset = 0;
	m_InstancedCount = 0;
	First = true;

}

void BatchInstanced::FlushKeep()
{
	for(int i = 0; i < m_TextureID.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TextureID[i]);
	}

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	
	glDrawElementsInstanced(GL_TRIANGLES, m_IndexCount, INSTANCED_GL_INDICES_TYPE, NULL, m_InstancedCount);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	m_Keep = true;
}

void BatchInstanced::Clear()
{
	m_IndexCount = 0;
	m_Offset = 0;
	m_InstancedCount = 0;
	First = true;
}

void BatchInstanced::PrintIndexBuffer()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	m_IndexBuffer = (INSTANCED_INDICES_TYPE*) glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_READ_ONLY);
	
	for(int i = 0; i < m_IndexCount; i++)
	{
		std::cout << m_IndexBuffer[i] << " ";
	}
	std::cout << std::endl;
	std::cout << "Index Count = " << m_IndexCount << std::endl;

	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	
}