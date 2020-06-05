#ifndef RENDER
#define RENDER

#include "../pch.h"

#include "light.h"
#include "camera.h"
#include "shader.h"

#include "../material_system/asset.h"

#include "../geometry/geometry.h"
#include "../geometry/model.h"
#include "../geometry/weapon.h"

#include "../scene/scenegraph.h"
#include "../scene/room.h"
#include "../scene/skybox.h"

#include "../config.h"
// extern const unsigned int WINDOW_WIDTH; //defined in main.h 
// extern const unsigned int WINDOW_HEIGHT; //used to reset viewport after shadow pass

class Room;

class Renderer
{
public:
	Renderer();
	void RenderGraph(nNode* Root, Camera* camera); //if not NULL, then build Shadow map 
	void RenderRoom(Room* room, Camera* cam);
	void RenderWeapon(Weapon* weapon, Camera* cam, float deltaTime);
	void RenderOutline(Model* model, Camera* cam); 
	void RenderSkybox(Skybox* skybox, Camera* cam);
	void Instance(nNode* Root, eType type, Asset* Instanced, std::vector<glm::mat4> &m_transforms, std::vector<glm::mat3> &n_transforms); //instances together all instances of Asset in tree 
	void SetCamera(Camera* camera);
	void SetLights(Room* room);
	void GetLights(Room* room);

private:
	void Traverse(nNode* Root, eType type);
	std::stack<glm::mat4> MatrixStack;

	cCube Cube; 
	cTriangle Triangle;
	cSquare Square;
	
	void RenderDepthMapRoom(Room* room, PointLight light, int lightIndex);
	std::vector<PointLight> shadowLights; 
	std::vector<int> shadowLightsIndex;
	bool shadowPass = true;

	int bulletHolesIndex = 0;
	int MAX_BULLET_HOLES = 10;
	std::vector<glm::vec3> bulletHoles;

	//Non Geometry
	Shader myShader =  Shader("shaders/simple/shader.vs", "shaders/simple/shader.fs"); //PATH FROM MAIN
	Shader instancedShader = Shader("shaders/simple/instancedShader.vs", "shaders/simple/instancedShader.fs");
    Shader lightShader = Shader("shaders/simple/lightShader.vs", "shaders/simple/lightShader.fs");
    Shader modelShader = Shader("shaders/simple/modelShader.vs", "shaders/simple/modelShader.fs");
    Shader outlineShader = Shader("shaders/simple/outlineShader.vs", "shaders/simple/outlineShader.fs");
    Shader skyboxShader = Shader("shaders/simple/skyboxShader.vs", "shaders/simple/skyboxShader.fs");

    //With Geometry
    Shader modelExplodeShader = Shader("shaders/simple/explodeShader.vs", "shaders/simple/explodeCubeShader.gs", "shaders/simple/explodeCubeShader.fs");
    Shader depthShader = Shader("shaders/simple/depthShader.vs", "shaders/simple/depthShader.gs", "shaders/simple/depthShader.fs");

};

#endif //RENDER