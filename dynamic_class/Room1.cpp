
#include "BaseRoom.hpp"

class Room0 : public BaseRoom {
public:
    Room0() : BaseRoom() 
    {
    	length = 30;
        width = 18;
        height = 8;
		offset = glm::vec3(0.0f, 2.0f, -length -0.05f);

        DoorN = {5,0,8,4, 3,1}; 
        DoorS = {0,0, width, height, -1,0}; 
        DoorE = {3,3,10,3, -1,1,  
                 17,3,10,3, -1,1};
        DoorW = {};

        //LIGHT
        LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
		lightPos = {glm::vec3(width/2.0f, height, length * 1.0f/3.0f),
                    glm::vec3(width/2.0f, height, length * 2.0f/3.0f)};
        lightRadius = {15.0f, 15.0f};
        lightAmbient = glm::vec3(0.7f);
    	lightConstant = 1.0f;
    	lightLinear = 0.045f; //0.14f;//0.045f;
        lightQuadratic = 0.0075f;//0.07f;//0.0075f;

        // CRATES
        vertical = {asset(2,5, glm::vec3(width-2.5f, 0.0f, 3.0f)), 
                    asset(2,4, glm::vec3(width-2.0f, 0.0f, 4.2f)),
                    asset(1,4, glm::vec3(width-1.5f, 0.0f, 6.0f)),
                    asset(2,3, glm::vec3(width-2.2f, 0.0f, 7.5f)),
                    asset(1,2, glm::vec3(width-1.8f, 0.0f, 10.0f)),
                    asset(2,1, glm::vec3(width-3.5f, 0.0f, 11.5f)),
                    asset(2,4, glm::vec3(width-2.3f, 0.0f, 13.0f)),
                    asset(1,3, glm::vec3(width-1.0f, 0.0f, 14.5f)),
                    asset(3,2, glm::vec3(width-3.3f, 0.0f, 17.0f)),
                    asset(1,7, glm::vec3(width-1.0f, 0.0f, 19.0f)),
                    asset(4,2, glm::vec3(width-5.8f, 0.0f, 20.0f)),
                    asset(2,1, glm::vec3(width-2.4f, 0.0f, 22.3f)),
                    asset(1,4, glm::vec3(width-1.5f, 0.0f, 25.2f)),
                    asset(2,6, glm::vec3(width-3.0f, 0.0f, 28.0f)),

                    asset(2,8, glm::vec3(0.5f, 0.0f, 15.0f)), 
                    asset(2,4, glm::vec3(0.0f, 0.0f, 4.2f)),
                    asset(1,4, glm::vec3(1.5f, 0.0f, 26.0f)),
                    asset(3,3, glm::vec3(0.2f, 0.0f, 18.5f)),
                    asset(1,2, glm::vec3(1.8f, 0.0f, 8.0f)),
                    asset(2,1, glm::vec3(1.5f, 0.0f, 11.5f)),

                    asset(5,2, glm::vec3(6.0f, 0.0f, 9.0f))
                    };


        horizontal = {asset(2,2, glm::vec3(3.0f, 0.0f, 22.0f)), 
                      asset(2,1, glm::vec3(3.0f, 1.0f, 23.0f)),
                      asset(1,1, glm::vec3(5.0f, 0.0f, 17.0f)),
                      asset(1,1, glm::vec3(14.0f, 0.0f, 4.0f))
                      };

        // TARGETS
        target = {asset(glm::vec3(3.5f, 2.0f, 22.5f), 'N'),
                  asset(glm::vec3(14.0f, 2.0f, 19.5f), 'N'),
                  asset(glm::vec3(5.0f, 1.0f, 16.5f), 'N'),
                  asset(glm::vec3(7.5f, 2.0f, 8.5f), 'N'),
                  asset(glm::vec3(14.0f, 1.0f, 3.5f), 'N')
                  };

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