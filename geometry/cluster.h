#ifndef CLUSTER_H
#define CLUSTER_H

#include <iostream>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "../scene/scenegraph.h"
#include "../material_system/asset.h"

#include <glm/gtx/rotate_vector.hpp> //rotate vector to place BB correctly, For BB 
struct BB
{
    void InitFromCube(glm::vec3 offset, glm::vec3 scaling, char orientation)
    {
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 temp_min;
        glm::vec3 temp_max;

        //select correct cube corner so that rotated ends up min/max also scale
        switch(orientation) //Facing W by default
        {
            case 'N':
                temp_min = glm::vec3(-0.5f, -0.5f, 0.5f);
                temp_min *= scaling;
                temp_min = glm::rotate(temp_min, glm::radians(-90.0f), Up);

                temp_max = glm::vec3(0.5f, 0.5f, -0.5f);
                temp_max *= scaling;
                temp_max = glm::rotate(temp_max, glm::radians(-90.0f), Up);                
            break;
            case 'S':
                temp_min = glm::vec3(0.5f, -0.5f, -0.5f);
                temp_min *= scaling;
                temp_min = glm::rotate(temp_min, glm::radians(90.0f), Up);

                temp_max = glm::vec3(-0.5f, 0.5f, 0.5f);
                temp_max *= scaling;
                temp_max = glm::rotate(temp_max, glm::radians(90.0f), Up);
            break;
            case 'E':
                temp_min = glm::vec3(0.5f, -0.5f, 0.5f);
                temp_min *= scaling;
                temp_min = glm::rotate(temp_min, glm::radians(180.0f), Up);

                temp_max = glm::vec3(-0.5f, 0.5f, -0.5f);
                temp_max *= scaling;
                temp_max = glm::rotate(temp_max, glm::radians(180.0f), Up);

            break;
            case 'W':
                temp_min = glm::vec3(-0.5f, -0.5f, -0.5f);
                temp_min *= scaling;
                
                temp_max = glm::vec3(0.5f, 0.5f, 0.5f);
                temp_max *= scaling;
            break;
        }

        //translate to offset (within object) 
        min = temp_min + offset;
        max = temp_max + offset;
    } 

    void Translate(glm::vec3 position)
    {
        min += position;
        max += position; 
    }

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

    asset(glm::vec3 position, char _orientation)
    {
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