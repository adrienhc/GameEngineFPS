#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../lib/stb_image.h"

Texture::Texture(std::string path)
{
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    //set teture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //S coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //T coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Zoom Out, using Mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Zoom In, DO NOT USE MIPMAP!

	const char* c_path = path.c_str();
    data = stbi_load(c_path, &width, &height, &nrChannels, 0);

    if (data)
    {
        GLenum format;
        if (nrChannels == 1)
            format = GL_RED;
        else if (nrChannels == 3)
            format = GL_RGB;
        else if (nrChannels == 4)
            format = GL_RGBA;

        //specify texture and image data and load data into the texture buffer 
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //generate mipmaps fot the currently bound 2D texture buffer
    }
    else
    {
        std::cout << "Material_System: Failed to load texture" << std::endl;
    }
    
    stbi_image_free(data);
}

Texture::~Texture()
{
    glBindTexture(GL_TEXTURE_2D, 0);
    glDeleteTextures(1, &texture);
}

void Texture::Bind(int offset)
{
	glActiveTexture(GL_TEXTURE0 + offset);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::Unbind(int offset)
{
    glActiveTexture(GL_TEXTURE0 + offset);
	glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetID()
{
    return texture;
}