#ifndef MAIN
#define MAIN 

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "lib/stb_image.h"

#include "rendering/shader.h"
#include "rendering/camera.h"
#include "rendering/renderer.h"
#include "rendering/light.h"

#include "material_system/asset.h"

#include "geometry/geometry.h"
#include "geometry/cluster.h"
#include "geometry/model.h"
#include "geometry/weapon.h"

#include "scene/scenegraph.h"
#include "scene/room.h"


//Window
const unsigned int WINDOW_WIDTH = 1920;//800;
const unsigned int WINDOW_HEIGHT = 1080;//600;

//Framebuffer and Input 
void framebuffer_size_callback( GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback( GLFWwindow* window, double xpos, double ypos);
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset);

bool CapFPS = true;
bool DisplayFPS = false;
bool Gravity = true;

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

//CAMERA -- FPS
Camera camera;
float lastX;
float lastY;
bool firstMouse = true;

//polygon mode
bool polygon = false;

//texture blending
float blend = 0.6f;


#endif