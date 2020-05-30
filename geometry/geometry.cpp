#include "geometry.h"


cTriangle::cTriangle()
{
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
	glEnableVertexAttribArray(2);

	//Elements Buffer Object (Indices)

	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO

}

cTriangle::cTriangle(std::vector<glm::mat4> &model_transforms, std::vector<glm::mat3> &normal_transforms)  //MAKE CONTRUCTOR
{

	instanced = true;
	num_instanced = model_transforms.size();


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
	glEnableVertexAttribArray(2);


	//MODEL_TRANSFORM
	glGenBuffers(1, &VBO_model_instancing);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_instancing);
	glBufferData(GL_ARRAY_BUFFER, model_transforms.size() * sizeof(glm::mat4), &model_transforms[0], GL_STATIC_DRAW);

	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	//NORMALS
	glGenBuffers(1, &VBO_normal_instancing);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_instancing);
	glBufferData(GL_ARRAY_BUFFER, normal_transforms.size() * sizeof(glm::mat3), &normal_transforms[0], GL_STATIC_DRAW);

	//glBindVertexArray(VAO);
	
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) 0);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) (sizeof(glm::vec3)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) (2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(9);


	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);



	//Elements Buffer Object (Indices)
	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO
}

void cTriangle::Render()
{
	Bind();
	Draw();
	unBind();
}

void cTriangle::Draw()
{
	if (instanced)
	{
		glDrawElementsInstanced(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0, num_instanced);
	}
	else
		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
}

void cTriangle::Bind()
{
	glBindVertexArray(VAO);
}


void cTriangle::unBind()
{
	glBindVertexArray(0);
}

cTriangle::~cTriangle()
{

	glBindVertexArray(VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	//glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
	
}

//////////////////////////////////////////////////
/////////////////////////////////////////////////
//////////////////////////////////////////////////

cSquare::cSquare()
{
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
	glEnableVertexAttribArray(2);

	//Elements Buffer Object (Indices)

	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO

}

cSquare::cSquare(std::vector<glm::mat4> &model_transforms, std::vector<glm::mat3> &normal_transforms)  //MAKE CONTRUCTOR
{

	instanced = true;
	num_instanced = model_transforms.size();


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
	glEnableVertexAttribArray(2);


	//MODEL_TRANSFORM
	glGenBuffers(1, &VBO_model_instancing);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_instancing);
	glBufferData(GL_ARRAY_BUFFER, model_transforms.size() * sizeof(glm::mat4), &model_transforms[0], GL_STATIC_DRAW);

	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	//NORMALS
	glGenBuffers(1, &VBO_normal_instancing);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_instancing);
	glBufferData(GL_ARRAY_BUFFER, normal_transforms.size() * sizeof(glm::mat3), &normal_transforms[0], GL_STATIC_DRAW);

	//glBindVertexArray(VAO);
	
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) 0);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) (sizeof(glm::vec3)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) (2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(9);


	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);



	//Elements Buffer Object (Indices)
	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO
}

void cSquare::Render()
{
	Bind();
	Draw();
	unBind();
}

void cSquare::Draw()
{
	if (instanced)
	{
		glDrawElementsInstanced(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0, num_instanced);
	}
	else
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

void cSquare::Bind()
{
	glBindVertexArray(VAO);
}


void cSquare::unBind()
{
	glBindVertexArray(0);
}

cSquare::~cSquare()
{
	glBindVertexArray(VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	//glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}








//////////////////////////////////////////////////
/////////////////////////////////////////////////
//////////////////////////////////////////////////


cCube::cCube()
{
	
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
	glEnableVertexAttribArray(2);

	//Elements Buffer Object (Indices)

	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO

}

cCube::cCube(std::vector<glm::mat4> &model_transforms, std::vector<glm::mat3> &normal_transforms)  //MAKE CONTRUCTOR
{

	instanced = true;
	num_instanced = model_transforms.size();


	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

	glGenBuffers(1, &VBO); //create VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
	glEnableVertexAttribArray(1);

	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
	glEnableVertexAttribArray(2);


	//MODEL_TRANSFORM
	glGenBuffers(1, &VBO_model_instancing);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_model_instancing);
	glBufferData(GL_ARRAY_BUFFER, model_transforms.size() * sizeof(glm::mat4), &model_transforms[0], GL_STATIC_DRAW);

	
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (sizeof(glm::vec4)));
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (2 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*) (3 * sizeof(glm::vec4)));
	glEnableVertexAttribArray(6);

	glVertexAttribDivisor(3, 1);
	glVertexAttribDivisor(4, 1);
	glVertexAttribDivisor(5, 1);
	glVertexAttribDivisor(6, 1);

	//glBindVertexArray(0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);


	//NORMALS
	glGenBuffers(1, &VBO_normal_instancing);
	glBindBuffer(GL_ARRAY_BUFFER, VBO_normal_instancing);
	glBufferData(GL_ARRAY_BUFFER, normal_transforms.size() * sizeof(glm::mat3), &normal_transforms[0], GL_STATIC_DRAW);

	//glBindVertexArray(VAO);
	
	glVertexAttribPointer(7, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) 0);
	glEnableVertexAttribArray(7);
	glVertexAttribPointer(8, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) (sizeof(glm::vec3)));
	glEnableVertexAttribArray(8);
	glVertexAttribPointer(9, 3, GL_FLOAT, GL_FALSE, sizeof(glm::mat3), (void*) (2 * sizeof(glm::vec3)));
	glEnableVertexAttribArray(9);


	glVertexAttribDivisor(7, 1);
	glVertexAttribDivisor(8, 1);
	glVertexAttribDivisor(9, 1);



	//Elements Buffer Object (Indices)
	glGenBuffers(1, &EBO); //create EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

	//unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
	glBindVertexArray(0); //UNBIND VAO FIRST
	glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO
}

void cCube::Render()
{
	Bind();
	Draw();
	unBind();
}

void cCube::Draw()
{
	if (instanced)
		glDrawElementsInstanced(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0, num_instanced);
	else
		glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void cCube::Bind()
{
	glBindVertexArray(VAO);
}


void cCube::unBind()
{
	glBindVertexArray(0);
}

cCube::~cCube()
{
	glBindVertexArray(VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
	glBindVertexArray(0);
	glDeleteVertexArrays(1, &VAO);
	//glDeleteVertexArrays(1, &VAO);
    //glDeleteBuffers(1, &VBO);
    //glDeleteBuffers(1, &EBO);
}