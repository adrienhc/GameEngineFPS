#ifndef ROOM_H
#define ROOM_H

#include <iostream>
#include <vector>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../geometry/cluster.h"

#include "../scene/scenegraph.h"

#include "../material_system/asset.h"

#include "../utils/collision.h"

#include "../rendering/light.h"
#include "../rendering/camera.h"
#include "../rendering/renderer.h"


class Renderer;

class Room
{

friend class Renderer; // Let Renderer access private data, no accessor needed

public:
	Room(int length, int width, int height, glm::vec3 ofst, std::vector<int> DN,  std::vector<int> DS, std::vector<int> DE, std::vector<int> DW, std::vector<glm::vec3> ptLghtPs, std::vector<asset> vertical, std::vector<asset> horizontal,
		Asset* floor, Asset* wall, Asset* door, Asset* beam, Asset* ceiling, Asset* crate, PointLight* ptLght);
	void makeRoom(Renderer renderer);	
	bool cameraCollide(Camera &camera);


private:
	//SCENE GRAPH
	nNode* Root = NULL;

	//COLLISION ACTIVATION
	static bool active; //only one for all rooms!
	bool self_collision = false; //do a ticket system 

	//DIMENSIONS
	int length;
	int width;
	int height;
	glm::vec3 offset;

	//COLLISION BOX
	glm::vec3 room_min_bb; // bb = bounding box
	glm::vec3 room_max_bb;
	float buffer = 0.25f; //collision buffer for point only

	//offset X, offset Y, dim X, dim Y    ,Opening Height (-1 No window), Beam (0/-1 No, 1 Yes) 
	std::vector<int> DoorN;
	std::vector<int> DoorS;
	std::vector<int> DoorE;
	std::vector<int> DoorW;
	int doorSpec = 6;

	//CRATE CLUSTERS
	std::vector<asset> vertical;
	std::vector<asset> horizontal;
	std::vector<BB> asset_bb;

	//MATERIALS
	Asset* floor; 
	Asset* wall; 
	Asset* door; 
	Asset* beam;
	Asset* ceiling;
	Asset* crate;

	//INSTANTIATED GEOMETRY PLACEHOLDERS
	cSquare* i_floor = NULL;
	cSquare* i_wall = NULL;
	cSquare* i_door = NULL;
	cCube* i_beam = NULL;
	cSquare* i_ceiling = NULL;
	cCube* i_crate = NULL;
	
	//LIGHT INFO
	std::vector<glm::vec3> pointLightPos;
	PointLight* pointLight;

	//METHODS
	glm::vec3 adjustAssetPos(glm::vec3 Pos);
	nNode* horizontalPlane(nNode* Root, Asset* asset);
	nNode* verticalPlaneNS(nNode* Root, std::vector<int> Door);
	nNode* verticalPlaneEW(nNode* Root, std::vector<int> Door);
	bool isDoor(int height, int width, std::vector<int> Door);
	bool pointOpeningDoor(int height, int width, std::vector<int> Door);
	bool boxOpeningDoor(int ref_height, int ref_width, int other_height, int other_width, std::vector<int> Door);
	bool hasBeam(int ibool);
	nNode* addBeamNS(nNode* Root, std::vector<int> Door);
	nNode* addBeamEW(nNode* Root, std::vector<int> Door); 
	bool pointCollide(glm::vec3 &playerPos, glm::vec3 min_bb, glm::vec3 max_bb);
	bool boxCollide(glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);

	void handlePointWall(glm::vec3 &playerPos, glm::vec3 min_bb, glm::vec3 max_bb);
	void handleBoxWall(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);
	std::string handleBoxObject(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);
};


#endif //ROOM_H