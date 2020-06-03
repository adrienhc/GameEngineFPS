EXEC = main
OBJECTS = main.o
OPTI = -O3

FILES = main.cpp  GLAD/glad.c 
# FILES += config.cpp                                            
FILES += rendering/shader.cpp rendering/camera.cpp rendering/frustum.cpp rendering/renderer.cpp rendering/light.cpp rendering/group.cpp
FILES += rendering/batch/batchrenderer.cpp rendering/batch/batchoutline.cpp rendering/batch/batchparticle.cpp rendering/batch/batchdepthmap.cpp rendering/batch/batchinstanced.cpp
FILES += rendering/layers/abstractlayer.cpp rendering/layers/scenelayer.cpp rendering/layers/outlinelayer.cpp rendering/layers/particlelayer.cpp rendering/layers/depthmaplayer.cpp rendering/layers/instancedlayer.cpp
FILES += material_system/asset.cpp material_system/material.cpp material_system/texture.cpp 
FILES += scene/scene.cpp scene/room.cpp scene/skybox.cpp
FILES += geometry/geometry.cpp geometry/cluster.cpp geometry/model.cpp geometry/mesh.cpp geometry/weapon.cpp geometry/target.cpp geometry/stairs.cpp
FILES += player/player.cpp utils/collision.cpp
FILES += particle_system/particle_system.cpp particle_system/particle.cpp
# FILES += dynamic_class/dlclass.cpp

# The flags to use for compilation                                    
FLAGS = -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLU -lGL -lglut -lassimp 
FLAGS += -L/home/adrienhc/Game/dynamic_class/ -lRoom0 -lRoom1 -lRoom2
# FLAGS += -march=broadwell -pipe
#FLAGS += -g #FOR DEBUG 
#FLAGS += -Wall
#FLAGS += -pg #FOR PROFILING

# The code compiler to use for compilation                            
CC = g++                                                              

# Perform action on all object files (May or may not exist)           
all: $(OBJECTS)                                                       
	$(CC) -o $(EXEC) $(OPTI) $(FILES) $(FLAGS)

rooms: room0 room1 room2

room0: 
	g++ -shared -fPIC geometry/cluster.cpp dynamic_class/Room0.cpp -o dynamic_class/libRoom0.so
room1:
	g++ -shared -fPIC geometry/cluster.cpp dynamic_class/Room1.cpp -o dynamic_class/libRoom1.so
room2:
	g++ -shared -fPIC geometry/cluster.cpp dynamic_class/Room2.cpp -o dynamic_class/libRoom2.so

clean:
	rm *.o  main