#ifndef BASE_ROOM_H
#define BASE_ROOM_H

#include "../pch.h"
#include "../geometry/cluster.h"

class BaseRoom {
public:
    BaseRoom() {}
    virtual ~BaseRoom() {}

    typedef BaseRoom * create_t();
    typedef void destroy_t(BaseRoom *);

    //DIMENSIONS
    int length;
    int width;
    int height;
    glm::vec3 offset;

    //DOORS
    std::vector<int> DoorN; 
    std::vector<int> DoorS;
    std::vector<int> DoorE;
    std::vector<int> DoorW;

    //LIGHT
    glm::vec3 LightColor;
    std::vector<glm::vec3> lightPos;
    std::vector<float> lightRadius;
    glm::vec3 lightAmbient;
    float lightConstant;
    float lightLinear;
    float lightQuadratic;

    // CRATES
    std::vector<asset> vertical;
    std::vector<asset> horizontal;

    // TARGETS
    std::vector<asset> target;

    // STAIRS
    std::vector<asset> stairs;

};

#endif