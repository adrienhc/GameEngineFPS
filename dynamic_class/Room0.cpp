
#include "BaseRoom.hpp"

class Room0 : public BaseRoom {
public:
    Room0() : BaseRoom() 
    {
    	length = 16;
        width = 30;
        height = 12;
		offset = glm::vec3(0.0f, 0.0f, 0.0f);

        DoorN = {8,0,14,7, 0,1}; 
        DoorS = {5,2,8,4, 3,1,  
                18,2,6,4, 3,1};
        DoorE = {2,5,12,6, -1,1};
        DoorW = {3,0,4,4, 1,1,
        		 9,0,4,4, 1,1,
        		 3,7,10,2, 2,1};

        //LIGHT
        LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		lightPos = {glm::vec3(width * 1.0f/3.0f, height, length/2.0f),
					glm::vec3(width * 2.0f/3.0f, height, length/2.0f)};
		lightRadius = {20.0f, 20.0f};
        lightAmbient = glm::vec3(0.7f);
    	lightConstant = 1.0f;
    	lightLinear = 0.045f;//0.14f;//0.045f;
    	lightQuadratic = 0.0075f;//0.07f;//0.0075f;

        // CRATES
        vertical = {};

        horizontal = {};

        // TARGETS
        target = {};

        // STAIRS
        stairs = {asset(8, 2, glm::vec3(5.0f, 0.0f, 2.0f), 'S'),
                  asset(6, 2, glm::vec3(18.0f, 0.0f, 2.0f), 'S')};
    }
    
};


// the class factories

extern "C" BaseRoom* create() {
    return new Room0();
}

extern "C" void destroy(BaseRoom* p) {
    delete p;
}