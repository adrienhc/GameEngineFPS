#ifndef RENDER
#define RENDER

#include <iostream>
#include <stack>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "light.h"
#include "camera.h"
#include "shader.h"

#include "../material_system/asset.h"

#include "../geometry/geometry.h"

#include "../scene/scenegraph.h"
#include "../scene/room.h"


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
	
	Shader myShader =  Shader("shaders/shader.vs", "shaders/shader.fs"); //PATH FROM MAIN
	Shader instancedShader = Shader("shaders/instancedShader.vs", "shaders/instancedShader.fs");
    Shader lightShader = Shader("shaders/lightShader.vs", "shaders/lightShader.fs");
};

#endif //RENDER