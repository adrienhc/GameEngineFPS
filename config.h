#ifndef CONFIG_H
#define CONFIG_H

#define TEXTURE_SLOTS 32
#define LIGHT_TEXTURE_OFFSET 20
#define MAX_RENDERER_TEXTURE_SLOTS 20 
#define SHADER_MAX_LIGHTS 5
#define SHADER_MAX_BULLET_HOLES 5

const unsigned int WINDOW_WIDTH = 800; 
const unsigned int WINDOW_HEIGHT = 600;
const bool CapFPS = false;
const bool DisplayFPS = true;
const bool Gravity = true;
const int NumRooms = 3;

#endif