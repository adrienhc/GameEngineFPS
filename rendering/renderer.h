#ifndef RENDER
#define RENDER

#include <iostream>
#include <stack>
#include <vector>
#include <cmath>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp> //rotate vector to align weapon with camera 

#include "light.h"
#include "camera.h"
#include "shader.h"

#include "../material_system/asset.h"

#include "../geometry/geometry.h"
#include "../geometry/model.h"

#include "../scene/scenegraph.h"
#include "../scene/room.h"


class Room;

class Renderer
{
public:
	Renderer();
	void RenderGraph(nNode* Root, Camera* camera);
	void RenderRoom(Room* room, Camera* cam);
	void RenderModel(Model* model, Camera* cam);
	void Instance(nNode* Root, eType type, Asset* Instanced, std::vector<glm::mat4> &m_transforms, std::vector<glm::mat3> &n_transforms);
	void SetLights(Room* room);

	static bool ads;
	
private:
	void Traverse(nNode* Root, eType type);
	std::stack<glm::mat4> MatrixStack;

	cCube Cube; 
	cTriangle Triangle;
	cSquare Square;
	
	Shader myShader =  Shader("shaders/shader.vs", "shaders/shader.fs"); //PATH FROM MAIN
	Shader instancedShader = Shader("shaders/instancedShader.vs", "shaders/instancedShader.fs");
    Shader lightShader = Shader("shaders/lightShader.vs", "shaders/lightShader.fs");
    Shader modelShader = Shader("shaders/modelShader.vs", "shaders/modelShader.fs");
};

#endif //RENDER