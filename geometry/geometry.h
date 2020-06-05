#ifndef GEOMETRY
#define GEOMETRY

#include "../pch.h"

enum eShape {eTriangle, eSquare, eCube, eInvertedCube}; 

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



struct batchSquare
{
	glm::vec3 Position[4] = {
	    glm::vec3(-0.5f, -0.5f, 0.0f),  
	    glm::vec3( 0.5f, -0.5f, 0.0f),  
	    glm::vec3( 0.5f,  0.5f, 0.0f),  
	    glm::vec3(-0.5f,  0.5f, 0.0f),  
	};

	glm::vec3 Normal[4] = {
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
	};

	glm::vec2 TexCoord[24] = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),
	};


	unsigned short indices[6] =
	{
	    0, 1, 2, 2, 3, 0 	    
	};

};



struct batchCube
{
	glm::vec3 Position[24] = {
	    glm::vec3(-0.5f, -0.5f, -0.5f),  
	    glm::vec3( 0.5f, -0.5f, -0.5f),  
	    glm::vec3( 0.5f,  0.5f, -0.5f),  
	    glm::vec3(-0.5f,  0.5f, -0.5f),  

	    glm::vec3(-0.5f, -0.5f,  0.5f),  
	    glm::vec3( 0.5f, -0.5f,  0.5f),  
	    glm::vec3( 0.5f,  0.5f,  0.5f),  	     
	    glm::vec3(-0.5f,  0.5f,  0.5f),  

	    glm::vec3(-0.5f,  0.5f,  0.5f),  
	    glm::vec3(-0.5f,  0.5f, -0.5f),  
	    glm::vec3(-0.5f, -0.5f, -0.5f),  
	    glm::vec3(-0.5f, -0.5f,  0.5f),  

	    glm::vec3( 0.5f,  0.5f,  0.5f),  
	    glm::vec3( 0.5f,  0.5f, -0.5f),  
	    glm::vec3( 0.5f, -0.5f, -0.5f),  
	    glm::vec3( 0.5f, -0.5f,  0.5f),  

	    glm::vec3(-0.5f, -0.5f, -0.5f),  
	    glm::vec3( 0.5f, -0.5f, -0.5f),  
	    glm::vec3( 0.5f, -0.5f,  0.5f),  
	    glm::vec3(-0.5f, -0.5f,  0.5f),  

	    glm::vec3(-0.5f,  0.5f, -0.5f),
	    glm::vec3( 0.5f,  0.5f, -0.5f),
	    glm::vec3( 0.5f,  0.5f,  0.5f),
	    glm::vec3(-0.5f,  0.5f,  0.5f)
	};

	glm::vec3 Normal[24] = {
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
	
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),

		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f)
	};

	glm::vec3 InvertedNormal[24] = {
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),
		glm::vec3(0.0f, 0.0f, 1.0f),

		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),
		glm::vec3(0.0f, 0.0f, -1.0f),

		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),
		glm::vec3(1.0f, 0.0f, 0.0f),

		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
		glm::vec3(-1.0f, 0.0f, 0.0f),
	
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),
		glm::vec3(0.0f, 1.0f, 0.0f),

		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f),
		glm::vec3(0.0f, -1.0f, 0.0f)
	};

	glm::vec2 TexCoord[24] = {
		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f),

		glm::vec2(0.0f, 0.0f),
		glm::vec2(1.0f, 0.0f),
		glm::vec2(1.0f, 1.0f),
		glm::vec2(0.0f, 1.0f)
	};

	unsigned short indices[36] =
	{
	    0, 1, 2, 2, 3, 0,
	    4, 5, 6, 6, 7, 4,
	    8, 9, 10, 10, 11, 8,
	    12, 13, 14, 14, 15, 12,
	    16, 17, 18, 18, 19, 16,
	    20, 21, 22, 22, 23, 20
	};
};



#endif //GEOMETRY