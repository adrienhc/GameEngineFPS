#ifndef RENDER
#define RENDER

#include <iostream>
#include "geometry.h"
#include "asset.h"
#include "light.h"
#include "scenegraph.h"
#include "camera.h"
#include "shader.h"
#include "room.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <stack>
#include <vector>

class Room;

class Renderer
{
public:
	Renderer();
	void RenderGraph(nNode* Root, Camera* camera, float ratio);
	void RenderRoom(Room* room, Camera* cam, float ratio);
	void Instance(nNode* Root, eType type, Asset* Instanced, std::vector<glm::mat4> &m_transforms, std::vector<glm::mat3> &n_transforms);
	
private:
	void Traverse(nNode* Root, eType type);
	std::stack<glm::mat4> MatrixStack;

	cCube Cube; 
	cTriangle Triangle;
	cSquare Square;
	
	Shader myShader =  Shader("./shaders/shader.vs", "./shaders/shader.fs");
	Shader instancedShader = Shader("./shaders/instancedShader.vs", "./shaders/instancedShader.fs");
    Shader lightShader = Shader("./shaders/lightShader.vs", "./shaders/lightShader.fs");
};

#endif //RENDER