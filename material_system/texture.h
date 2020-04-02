#ifndef TEXTURE
#define TEXTURE

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>


class Texture //Load a texture from an image myself!
{
public:
		Texture(std::string path);
		void Free();
		void Bind();
		void unBind();
		unsigned int GetID();
private:
		unsigned char *data = NULL;
		int width, height, nrChannels;
		unsigned int texture;

};


#endif //TEXTURE