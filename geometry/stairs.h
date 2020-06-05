#ifndef STAIRS_H
#define STAIRS_H

#include "../pch.h"
#include "../scene/scenegraph.h"
#include "../material_system/asset.h"
#include "cluster.h" //for BB class


class Stairs
{
public:
	Stairs(glm::vec3 bottom_left, float width, float length, float height, char orientation); //'N' 'S' 'E' 'W' -- For Bounding Box 
	~Stairs();
	bool SetHeight(glm::vec3 player_position, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb);

	BB stairs_bb;
	std::vector<BB> steps_bb;
	char orientation;
	float length;
	float height;
	Asset* step = NULL;
	nNode* Root = NULL;

private:
	

};

#endif