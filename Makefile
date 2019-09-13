EXEC = main
OBJECTS = main.o
FILES = main.cpp shader.cpp camera.cpp glad.c asset.cpp material.cpp texture.cpp  geometry.cpp light.cpp scenegraph.cpp renderer.cpp scene.cpp room.cpp player.cpp collision.cpp cluster.cpp                                              
# The flags to use for compilation                                    
FLAGS = -L/usr/local/lib -lglfw3 -lrt -lm -ldl -lXrandr -lXinerama -lXxf86vm -lXext -lXcursor -lXrender -lXfixes -lX11 -lpthread -lxcb -lXau -lXdmcp -lGLU -lGL -lglut     
FLAGS += -g #FOR DEBUG

# The code compiler to use for compilation                            
CC = g++                                                              

# Perform action on all object files (May or may not exist)           
all: $(OBJECTS)                                                       
	$(CC) -o $(EXEC) $(FILES) $(FLAGS)

clean:
	rm *.o  