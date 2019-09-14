#ifndef GEOMETRY
#define GEOMETRY

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtx/string_cast.hpp> //debug, visualize matrix

#include <vector>
#include <iostream>

enum eShape {eTriangle, eSquare, eCube}; 

class cTriangle   //REFINE AND MAKE IT WORK
{
	public: 

		cTriangle();
		cTriangle(std::vector<glm::mat4> &model_transforms, std::vector<glm::mat3> &normal_transforms);
		
		void Render();
		void Bind();
		void Draw();
		void unBind();
		
		~cTriangle();

	private:
		unsigned int VAO;
    	unsigned int VBO;
    	unsigned int VBO_model_instancing;
    	unsigned int VBO_normal_instancing;
    	unsigned int EBO;

    	bool instanced = false;
    	int num_instanced;

		float vertices[24] = {
			//coord               //norm       		  //tex
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,
			0.5f, -0.5f, 0.0f,    0.0f, 0.0f, 1.0f,   1.0f, 0.0f,
			0.0f, 0.5f, 0.0f,     0.0f, 0.0f, 1.0f,   0.5f, 1.0f
		};
		
		unsigned int indices[3] = {
			0, 1, 2
		};

};


class cSquare
{	
	public: 

		cSquare();
		cSquare(std::vector<glm::mat4> &model_transforms, std::vector<glm::mat3> &normal_transforms);

		void Render();
		void Bind();
		void Draw();
		void unBind();

		~cSquare();

	private:
		unsigned int VAO;
    	unsigned int VBO;
    	unsigned int VBO_model_instancing;
    	unsigned int VBO_normal_instancing;
    	unsigned int EBO;

    	bool instanced = false;
    	int num_instanced;
		
		float vertices[32] = {
		//coord               //norm       		  //tex
		-0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 0.0f,
	     0.5f, -0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 0.0f,
	     0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  1.0f, 1.0f,
	    -0.5f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f,  0.0f, 1.0f
		};

		unsigned int indices[6] = {
			0, 1, 2, 0, 2, 3
		};
};


class cCube 
{
	public:

		cCube();
		cCube(std::vector<glm::mat4> &model_transforms, std::vector<glm::mat3> &normal_transforms);

		void Render();
		void Bind();
		void Draw();
		void unBind();
		
		~cCube();
		
	private:

		unsigned int VAO;
	    unsigned int VBO;
	    unsigned int VBO_model_instancing;
    	unsigned int VBO_normal_instancing;
	    unsigned int EBO;

	    bool instanced = false;
    	int num_instanced;

		float vertices[288] = {
	    //position           //normals          //texture
	    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 
	     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
	    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

	    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
	     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

	    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
	     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
	     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
	     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
	     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

	    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
	     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
	     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
	    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
	    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

	    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
	     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
	     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
	    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
	    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
	};

	unsigned int indices[36] =
	{
	    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
	    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
	    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
	    30, 31, 32, 33, 34, 35
	    
	};

};

#endif //GEOMETRY