
#include "BaseRoom.hpp"

class Room0 : public BaseRoom {
public:
    Room0() : BaseRoom() 
    {
    	length = 10;
        width = 22;
        height = 10;
		offset = glm::vec3(-2.0f, 2.0f, -length - 30.0f -0.05f);

        DoorN = {2, 0, width-4, height-2, -1,0}; 
        DoorS = {2,0,2,4, 3,1,
                 5,1,3,2, -1,1}; 
        DoorE = {3,0,4,3, 0,1,
                 2,6,6,2, -1,1};
        DoorW = {};

        //LIGHT
        LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		lightPos = {glm::vec3(width/2.0f, height, length/2.0f)};
        lightRadius = {20.0f};
        lightAmbient = glm::vec3(0.7f);
    	lightConstant = 1.0f;
    	lightLinear = 0.0075f; //0.14f;//0.045f;
        lightQuadratic = 0.0075f;// 0.07f;//0.0075f;

        // CRATES
        vertical = { };


        horizontal = { };

        // TARGETS
        target = { };

        // STAIRS
        stairs = {};
    }
    
};


// the class factories

extern "C" BaseRoom* create() {
    return new Room0();
}

extern "C" void destroy(BaseRoom* p) {
    delete p;
}