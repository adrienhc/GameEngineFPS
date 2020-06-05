#ifndef BOUNDING_BOX_H
#define BOUNDING_BOX_H

#include "../pch.h"

struct BB
{
    void InitFromCube(glm::vec3 offset, glm::vec3 scaling, char orientation)
    {
        glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 temp_min;
        glm::vec3 temp_max;

        //select correct cube corner so that rotated ends up min/max also scale
        switch(orientation) //Facing N by default
        {
            case 'E':
                temp_min = glm::vec3(-0.5f, -0.5f, 0.5f);
                temp_min *= scaling;
                temp_min = glm::rotate(temp_min, glm::radians(-90.0f), Up);

                temp_max = glm::vec3(0.5f, 0.5f, -0.5f);
                temp_max *= scaling;
                temp_max = glm::rotate(temp_max, glm::radians(-90.0f), Up);                
            break;
            case 'W':
                temp_min = glm::vec3(0.5f, -0.5f, -0.5f);
                temp_min *= scaling;
                temp_min = glm::rotate(temp_min, glm::radians(90.0f), Up);

                temp_max = glm::vec3(-0.5f, 0.5f, 0.5f);
                temp_max *= scaling;
                temp_max = glm::rotate(temp_max, glm::radians(90.0f), Up);
            break;
            case 'S':
                temp_min = glm::vec3(0.5f, -0.5f, 0.5f);
                temp_min *= scaling;
                temp_min = glm::rotate(temp_min, glm::radians(180.0f), Up);

                temp_max = glm::vec3(-0.5f, 0.5f, -0.5f);
                temp_max *= scaling;
                temp_max = glm::rotate(temp_max, glm::radians(180.0f), Up);

            break;
            case 'N':
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

    void InitFromMinMax(glm::vec3 tr_min, glm::vec3 tr_max)
    {

        if(tr_min.x < tr_max.x)
        {
            min.x = tr_min.x;
            max.x = tr_max.x;
        }
        else
        {
            max.x = tr_min.x;
            min.x = tr_max.x;
        }

        if(tr_min.y < tr_max.y)
        {
            min.y = tr_min.y;
            max.y = tr_max.y;
        }
        else
        {
            max.y = tr_min.y;
            min.y = tr_max.y;
        }

        if(tr_min.z < tr_max.z)
        {
            min.z = tr_min.z;
            max.z = tr_max.z;
        }
        else
        {
            max.z = tr_min.z;
            min.z = tr_max.z;
        }
    }

	glm::vec3 min;
	glm::vec3 max;
};

#endif