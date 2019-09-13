#include "texture.h"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

Texture::Texture(std::string path)
{
	glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    //set teture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //S coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //T coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Zoom Out, using Mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Zoom In, DO NOT USE MIPMAP!

	stbi_set_flip_vertically_on_load(true); //set true if image loads upside down
	const char* c_path = path.c_str();
    data = stbi_load(c_path, &width, &height, &nrChannels, 0);

    if (data)
    {
        //specify texture and image data and load data into the texture buffer 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D); //generate mipmaps fot the currently bound 2D texture buffer
    }
    else
    {
        std::cout << "Failed to load box texture" << std::endl;
    }

    stbi_image_free(data);
}

void Texture::Bind()
{
	glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture);
}

void Texture::unBind()
{
	 glBindTexture(GL_TEXTURE_2D, 0);
}