#ifndef TEXTURE
#define TEXTURE

#include <string>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>


class Texture
{
public:
		Texture(std::string path);
		void Free();
		void Bind();
		void unBind();
private:
		unsigned char *data = NULL;
		int width, height, nrChannels;
		unsigned int texture;

};


#endif //TEXTURE