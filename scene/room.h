#ifndef ROOM_H
#define ROOM_H

#include "../pch.h"

#include "../geometry/cluster.h"
#include "../geometry/target.h"
#include "../geometry/stairs.h"
#include "../geometry/weapon.h"

#include "../scene/scenegraph.h"

#include "../material_system/asset.h"

#include "../particle_system/particle_system.h"
#include "../particle_system/particle.h"

#include "../utils/collision.h"

#include "../rendering/light.h"
#include "../rendering/camera.h"
#include "../rendering/renderer.h"
#include "../rendering/group.h"
#include "../rendering/layers/abstractlayer.h"
#include "../rendering/layers/scenelayer.h"
#include "../rendering/layers/instancedlayer.h"

#include "../geometry/bounding_box.h"


class Renderer;

class Room
{

friend class Renderer; // Let Renderer access private data, no accessor needed

public:
	Room(int length, int width, int height, glm::vec3 ofst, std::vector<int> DN,  std::vector<int> DS, std::vector<int> DE, std::vector<int> DW, 
		std::vector<glm::vec3> ptLghtPs, std::vector<float> ptLghtRad, std::vector<asset> vertical, std::vector<asset> horizontal, std::vector<asset> target, std::vector<asset> stairs,
		Asset* floor, Asset* wall, Asset* door, Asset* beam, Asset* ceiling, Asset* crate, PointLight* ptLght);
	~Room();
	void makeRoom();//Renderer renderer);	
	bool collisionChecks(Camera &camera);
	void getLights(Renderer renderer);
	void addLayoutLayer(AbstractLayer* layer);
	void addLayoutLayerCull(AbstractLayer* layer);
	void addCubesLayer(InstancedLayer* instanced_layer);
	void addTilesLayer(InstancedLayer* instanced_layer);
	void addLightsLayer(SceneLayer* layer, bool renderable = false);
	void addLightsLayerCull(SceneLayer* layer, bool renderable = false);
	void addTargetsLayer(AbstractLayer* model_layer, AbstractLayer* outline_layer, AbstractLayer* particle_layer, AbstractLayer* depthmap_layer);
	void addBulletImpact(Camera* camera, SceneLayer* layer, ParticleSystem* particle_system);
	BB getBoundingBox();
	static bool collideOn()
	{
		return !active;
	}

	static void setupCollisions();	
	static float minBulletDist;
	void postCollisions();
	static bool shadowPass;

	

private:
	//ROOM ID
	static int sharedID;
	int ID;

	//SCENE GRAPH
	nNode* Root = NULL;
	nNode* Lights = NULL; //Return Lights in World Space (!= than Room Space)

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
	BB bounding_box;
	float buffer = 0.25f; //collision buffer for point only

	//offset X, offset Y, dim X, dim Y    ,Opening Height (-1 No window), Beam (0/-1 No, 1 Yes) 
	std::vector<int> DoorN;
	std::vector<int> DoorS;
	std::vector<int> DoorE;
	std::vector<int> DoorW;
	int doorSpec = 6;

	//CRATE CLUSTERS -- GEOMETRY IS INSTANCED
	std::vector<asset> vertical;
	std::vector<asset> horizontal;
	std::vector<BB> asset_bb;

	//TARGETS -- NON INSTANCED, Only a few
	std::vector<Target*> targets;

	//STAIRS
	std::vector<Stairs*> stairs;

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
	

	//GEOMETRY GROUP
	Group* g_layout = NULL; //ALL ROOM GEOMETRY

	Group* g_tiles = NULL;
	Group* g_cubes = NULL;
	
	//LIGHT INFO
	std::vector<glm::vec3> pointLightPos;
	std::vector<float> pointLightRad;
	PointLight* pointLight;

	//LIGHT GROUP
	Group* g_light = NULL;

	//PARTICLE IMPACT
	Particle* baseImpactShort;
	Particle* baseImpactLong;

	//BULLET COLLISION MIN DIST
	static float maxBulletDist;
	static float maxRange; //if do not hit any geometry, cutoff value, max range 

	//METHODS
	glm::vec3 adjustAssetPos(glm::vec3 Pos);
	//OLD ROOM TILES, ONE SQUARE PER TILE
	nNode* horizontalPlane(nNode* Root, Asset* asset);
	nNode* verticalPlaneNS(nNode* Root, std::vector<int> Door, bool justBeams);
	nNode* verticalPlaneEW(nNode* Root, std::vector<int> Door, bool justBeams);
	//NEW ROOM TILES, LEAST SQUARE PER WALL
	nNode* horizontalPlaneLowpoly(nNode* Root, Asset* asset);
	//Recursive approach,  Divide and Conquer, fit largest pannel within constraints
	nNode* verticalPlaneNSLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<int> Door); 
	nNode* verticalWallNSLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<glm::vec4> dim);
	nNode* verticalPlaneEWLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<int> Door); 
	nNode* verticalWallEWLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<glm::vec4> dim);
	nNode* verticalDoorNSLowpoly(nNode* Root, std::vector<int> Door);
	nNode* verticalDoorEWLowpoly(nNode* Root, std::vector<int> Door);
	nNode* addBeamNS(nNode* Root, std::vector<int> Door);
	nNode* addBeamEW(nNode* Root, std::vector<int> Door); 

	bool isDoor(int height, int width, std::vector<int> Door);
	bool pointOpeningDoor(int height, int width, std::vector<int> Door, bool construction = false);
	bool boxOpeningDoor(int ref_height, int ref_width, int other_height, int other_width, std::vector<int> Door);
	bool hasBeam(int ibool);

	bool cameraCollide(Camera &camera);
	bool bulletCollide(Camera &camera);

	bool pointCollide(glm::vec3 &playerPos, glm::vec3 min_bb, glm::vec3 max_bb);
	bool boxCollide(glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb, bool ignore_top = false);
	void handlePointWall(glm::vec3 &playerPos, glm::vec3 min_bb, glm::vec3 max_bb);
	void handleBoxWall(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);
	std::string handleBoxObject(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb);

	//MATH UTILS
	//In what order we sort the Doors
	static bool Vec4ComparisonFunction(const glm::vec4 &vecA, const glm::vec4 &vecB)
	{
	 return vecA[0]<vecB[0] //closer x 
	        || vecA[1]>vecB[1] //wider x
	        || vecA[2]<vecB[2] //lower y
	        || vecA[3]>vecB[3]; //higher y
	}

};


#endif //ROOM_H