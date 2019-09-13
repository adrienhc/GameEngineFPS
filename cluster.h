#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "scenegraph.h"
#include "asset.h"

struct BB
{
	glm::vec3 min;
	glm::vec3 max;
};

struct asset
{
    asset(int _x, int _y, glm::vec3 position)
    {
        x = _x;
        y = _y;
        pos = position;
    }

    int x, y;
    glm::vec3 pos;
};

class Cluster
{
public:
	static BB Vertical(nNode* Root, int width, int height, glm::vec3 position, float y_rotation, Asset* asset);
	static BB Horizontal(nNode* Root, int width, int length, glm::vec3 position, float y_rotation, Asset* asset);

	static BB Array();
};


#endif //CLUSTER_H