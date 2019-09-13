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

#include "stb_image.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "shader.h"
#include "camera.h"
#include "geometry.h"
#include "asset.h"
#include "light.h"
#include "renderer.h"
#include "scenegraph.h"
#include "room.h"
#include "cluster.h"
//#include "scene.h"


//Window
const unsigned int WINDOW_WIDTH = 800; //1920;//800;
const unsigned int WINDOW_HEIGHT = 600; //1080;//600;

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