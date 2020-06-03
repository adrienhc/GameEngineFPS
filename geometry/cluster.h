#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../scene/scenegraph.h"
#include "../material_system/asset.h"

#include "bounding_box.h"

#include <glm/gtx/rotate_vector.hpp> //rotate vector to place BB correctly, For BB 

struct asset
{
    asset(int _x, int _y, glm::vec3 position)
    {
        x = _x;
        y = _y;
        pos = position;
    }

    asset(glm::vec3 position, char _orientation)
    {
        pos = position;
        orientation = _orientation;
    }

    asset(int _x, int _y, glm::vec3 position, char _orientation)
    {
        x = _x;
        y = _y;
        pos = position;
        orientation = _orientation;   
    }

    
    int x, y;
    glm::vec3 pos;
    char orientation;

};

class Cluster
{
public:
	static BB Vertical(nNode* Root, int width, int height, glm::vec3 position, float y_rotation, Asset* asset);
	static BB Horizontal(nNode* Root, int width, int length, glm::vec3 position, float y_rotation, Asset* asset);
    static BB Array();
};


#endif //CLUSTER_H