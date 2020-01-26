EXEC = main
OBJECTS = main.o
OPTI = -O3

FILES = main.cpp  GLAD/glad.c                                                 
FILES += rendering/shader.cpp rendering/camera.cpp rendering/renderer.cpp rendering/light.cpp
FILES += material_system/asset.cpp material_system/material.cpp material_system/texture.cpp 
FILES += scene/scene.cpp scene/room.cpp scene/skybox.cpp
FILES += geometry/geometry.cpp geometry/cluster.cpp geometry/model.cpp geometry/mesh.cpp geometry/weapon.cpp geometry/target.cpp
FILES += player/player.cpp utils/collision.cpp

# The flags to use for compilation                                    
FLAGS = -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLU -lGL -lglut -lassimp
FLAGS += -g #FOR DEBUG

# The code compiler to use for compilation                            
CC = g++                                                              

# Perform action on all object files (May or may not exist)           
all: $(OBJECTS)                                                       
	$(CC) -o $(EXEC) $(OPTI) $(FILES) $(FLAGS)

clean:
	rm *.o  main