#ifndef MAIN
#define MAIN 

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "pch.h"

#include "lib/stb_image.h"

#include "config.h"

#include "rendering/shader.h"
#include "rendering/camera.h"
#include "rendering/light.h"
#include "rendering/group.h"

#include "rendering/renderer.h"
#include "rendering/batch/batchrenderer.h"
#include "rendering/batch/batchoutline.h"
#include "rendering/batch/batchparticle.h"
#include "rendering/batch/batchdepthmap.h"
#include "rendering/batch/batchinstanced.h"

#include "rendering/layers/scenelayer.h"
#include "rendering/layers/outlinelayer.h"
#include "rendering/layers/particlelayer.h"
#include "rendering/layers/depthmaplayer.h"
#include "rendering/layers/instancedlayer.h"

#include "material_system/asset.h"

#include "geometry/geometry.h"
#include "geometry/cluster.h"
#include "geometry/model.h"
#include "geometry/weapon.h"
#include "geometry/target.h"
#include "geometry/stairs.h"

#include "scene/scenegraph.h"
#include "scene/room.h"
#include "scene/skybox.h"

#include "player/player.h"

#include "particle_system/particle_system.h"

#include "dynamic_class/dlclass.hpp"
#include "dynamic_class/BaseRoom.hpp"


//Framebuffer and Input 
void framebuffer_size_callback( GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback( GLFWwindow* window, double xpos, double ypos);
//void scroll_callback( GLFWwindow* window, double xoffset, double yoffset);
bool BuildWorld = true;
bool FixCamera = false;
void build_world(std::vector<Room*>& World, Asset* floor, Asset* wall, Asset* door, Asset* beam, Asset* ceiling, Asset* crate);

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

//depthmap first
bool firstPass = true;


#endif