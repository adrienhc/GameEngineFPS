#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "shader.h"
#include "camera.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

//Window
const unsigned int WINDOW_WIDTH = 800;
const unsigned int WINDOW_HEIGHT = 600;

//Framebuffer and Input 
void framebuffer_size_callback( GLFWwindow* window, int width, int height);
void process_input(GLFWwindow* window);
void mouse_callback( GLFWwindow* window, double xpos, double ypos);
void scroll_callback( GLFWwindow* window, double xoffset, double yoffset);

// timing
float deltaTime = 0.0f; // time between current frame and last frame
float lastFrame = 0.0f;

//CAMERA -- FPS
Camera camera;
float lastX;
float lastY;
bool firstMouse = true;

//LIGHT
glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
glm::vec3 LightPos = glm::vec3(2.0f, 2.0f, 0.0f);


//Static Vertices and Indices
/*float vertices[] =
{   
    //position           //color                  //texture
    -0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f, 1.0f,  0.0f, 0.0f, //bot left   0 0
    0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f, 1.0f,  1.0f, 0.0f, //bot right  1 0 
    0.5f, 0.5f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  1.0f, 1.0f, //top right   1 1 
    -0.5f, 0.5f, 0.0f,  1.0f, 1.0f, 0.0f, 1.0f,  0.0f, 1.0f  //top left    0 1

};*/

float vertices[] = {
    //position           //normals          //texture
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, 
     0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,
     0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
     0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
     0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
     0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f
};

unsigned int indices[] =
{
    0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 
    10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
    20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
    30, 31, 32, 33, 34, 35
    
};


glm::vec3 cubePositions[] = {
  glm::vec3( 0.0f,  0.0f,  0.0f), 
  glm::vec3( 2.0f,  5.0f, -15.0f), 
  glm::vec3(-1.5f, -2.2f, -2.5f),  
  glm::vec3(-3.8f, -2.0f, -12.3f),  
  glm::vec3( 2.4f, -0.4f, -3.5f),  
  glm::vec3(-1.7f,  3.0f, -7.5f),  
  glm::vec3( 1.3f, -2.0f, -2.5f),  
  glm::vec3( 1.5f,  2.0f, -2.5f), 
  glm::vec3( 1.5f,  0.2f, -1.5f), 
  glm::vec3(-1.3f,  1.0f, -1.5f)  
};


//polygon mode
bool polygon = false;

//texture blending
float blend = 0.6f;

int main()
{
    //GLFW WINDOW SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //for OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "FPS Game", NULL, NULL);
                                                                                        //glfwGetPrimaryMonitor()   --for fullscreen app automatically

    if (window == NULL)
    {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //setup GLAD, gives pointers to OS specific OpenGL function pointers
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.423f, 0.701f, 0.756f, 1.0f);

    //SHADER SETUP
    Shader myShader("./shaders/shader.vs", "./shaders/shader.fs");
    Shader lightShader("./shaders/lightShader.vs", "./shaders/lightShader.fs");


    //Buffers

    //IMPORTANT

        /*VAO HOLDS VBO AND EBO*/

        /*Usually when you have multiple objects you want to draw, 
        you first generate/configure all the VAOs (and thus the required VBO and attribute pointers) and store those for later use. 
        The moment we want to draw one of our objects, we take the corresponding VAO, bind it, then draw the object and unbind the VAO again.*/

        /*The element buffer object currently bound while a VAO is bound, is stored as the VAO's element buffer object. Binding to a VAO thus also automatically binds its EBO. */

        /*don't unbind the element array buffer before unbinding your VAO, otherwise it doesn't have an EBO configured. */

    //Vertex Array Object
    unsigned int VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);  //BIND VAO FIRST BEFORE VBO

    //Vertex Buffer Object
    unsigned int VBO;
    glGenBuffers(1, &VBO); //create VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO); //Bind it
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW); //send data to GPU

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) 0); //setup Vertex Attributes 0 - POS
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (3 * sizeof(float))); //setup Vertex Attributes 1 - COLOR
    glEnableVertexAttribArray(1);

    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*) (6 * sizeof(float))); //setup Vertex Attributes 2 - TEXTURE
    glEnableVertexAttribArray(2);

    //Elements Buffer Object (Indices)
    unsigned int EBO;
    glGenBuffers(1, &EBO); //create EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO); //Bind it
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); //Sent Data to GPU

    //unbind VBO and VAO (don't do unless absolutely necessary here just as an example)
    glBindVertexArray(0); //UNBIND VAO FIRST
    glBindBuffer(GL_ARRAY_BUFFER, 0); //VBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0); //EBO


    //Textures

    //box
    unsigned int box_texture;  //create texture buffer and bind it to a 2D texture object
    glGenTextures(1, &box_texture);
    glBindTexture(GL_TEXTURE_2D, box_texture);

    //set teture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); //S coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT); //T coord
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR); //Zoom Out, using Mipmap
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //Zoom In, DO NOT USE MIPMAP!

    //Load image and set dimension parameters
    stbi_set_flip_vertically_on_load(true); //set true if image loads upside down
    int box_width, box_height, box_nrChannels;

    unsigned char *box_data = stbi_load("./textures/cratebw.jpg", &box_width, &box_height, &box_nrChannels, 0);

    if (box_data)
    {
        //specify texture and image data and load data into the texture buffer 
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, box_width, box_height, 0, GL_RGB, GL_UNSIGNED_BYTE, box_data);
        glGenerateMipmap(GL_TEXTURE_2D); //generate mipmaps fot the currently bound 2D texture buffer
    }
    else
    {
        std::cout << "Failed to load box texture" << std::endl;
    }

    //free image data
    stbi_image_free(box_data);

    //MAP SAMPLERS TO TEXTURE UNIT
    myShader.use(); //use correct shader beforehand
    myShader.setInt("imgTexture", 0); //here value is the texture index
    myShader.setVec3("lightPos", LightPos);
    myShader.setVec3("lightColor", LightColor);
    
    // COORD SPACES 

    // -- VIEWING
    
    camera = Camera(glm::vec3(0.0f, 0.0f, 0.0f));
    lastX = WINDOW_WIDTH / 2.0f;
    lastY = WINDOW_HEIGHT / 2.0f;

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));
    myShader.setMat4("viewing", view);

    // -- PROJECTION
    float ratio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
    //VIEW AND CAMERA
    glm::mat4 proj = glm::perspective(glm::radians(70.0f), ratio, 0.1f, 100.0f);
    myShader.setMat4("projection", proj);



    lightShader.use();
    lightShader.setMat4("model_transform", glm::scale(glm::translate(glm::mat4(1.0f), LightPos), glm::vec3(0.2f, 0.2f, 0.2f)));
    lightShader.setMat4("viewing", view);
    lightShader.setMat4("projection", proj);
    lightShader.setVec3("lightColor", LightColor);


    //RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {

        //timing
        float current_frame = glfwGetTime();
        deltaTime = current_frame - lastFrame;
        lastFrame = current_frame;
        //Input Processing
        process_input(window);

        if(polygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //Time
        float Time = glfwGetTime(); 

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //Take care of texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, box_texture); //automatically assigns texture to fs' sampler :) IF ONLY ONE TEXTURE (same as calling glActiveTexture(GL_TEXTURE0); )  
        
        //set blending coefficient
        myShader.use();
        myShader.setInt("blend", blend);
        view = camera.GetViewMatrix();  
        myShader.setMat4("viewing", view);
        proj = glm::perspective(glm::radians(camera.Zoom), ratio, 0.1f, 100.0f);
        myShader.setMat4("projection", proj);
        myShader.setVec3("cameraPos", camera.Position);
        glBindVertexArray(VAO); //Bind appropriate VAO

         //TRANSFORMS  //update transform
        unsigned int size = sizeof(cubePositions)/sizeof(cubePositions[0]); 
        for (unsigned int i = 0; i < size; i++ )
        {
            glm::mat4 transform = glm::mat4(1.0f);
            transform = glm::translate(transform, cubePositions[i]);
            if (i % 3 != 0)
            {
                transform = glm::rotate(transform, Time, glm::vec3(1.0f, (float) i, (float) i * 0.2f));
                transform = glm::rotate(transform, (float) Time, glm::vec3((float) i, 0.2f, 0.0f));
            }
            myShader.setMat4("model_transform", transform);

            glm::mat3 norm_transform = glm::mat3(glm::transpose(glm::inverse(transform)));
            myShader.setMat3("normal_transform", norm_transform);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        }

        lightShader.use();
        lightShader.setMat4("viewing", view);
        lightShader.setMat4("projection", proj);
        glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);

        //Swap Buffers, Poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    //Deallocate buffers allocated
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    //terminate GLFW Window
    glfwTerminate();
    return 0;
}

void framebuffer_size_callback( GLFWwindow* window, int width, int height)
{
   // glfwSetWindowSizeCallback(window, &width, &height);
   glViewport(0, 0, width, height); 
}

void process_input(GLFWwindow* window)
{
    if(glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 

    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        polygon = !polygon; 

   // if(glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS)
   //     fps_cam = !fps_cam; 

    if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && blend < 1.0f)
        blend += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && blend > 0.0f)
        blend -= 0.05f;

    //if(glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
    //    cam_XZ += camera_Speed;

    //if(glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
    //    cam_XZ -= camera_Speed;

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARDS, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
}

void mouse_callback( GLFWwindow* window, double xpos, double ypos)
{
    if(firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos;
    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovements(xoffset, yoffset);
}

void scroll_callback( GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}