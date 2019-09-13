#include "main.h"

int main()
{

    std::cout << "RUN MAIN RUN" << std::endl;
    //GLFW WINDOW SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //for OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!CapFPS)
       glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

    //GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "FPS Game", NULL, NULL);
                                                                                        //glfwGetPrimaryMonitor()   --for fullscreen app automatically

    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "FPS Game", glfwGetPrimaryMonitor(), NULL);

    if (window == NULL)
    {
        std::cout << "Failed to create the GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    //if(noCapFPS)
    //    glfwSwapInterval( 0 );

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //setup GLAD, gives pointers to OS specific OpenGL function pointers
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.423f, 0.701f, 0.756f, 1.0f);

    glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);
                                       //ambient        diffuse      //specular  //cst //lin //quad
    PointLight pointLight = PointLight(glm::vec3(0.6f),  LightColor, LightColor, 1.0f, 0.09f, 0.032f);

                                        //ambient,  diffuse,  specular,  shininess,  color
    Asset crate = Asset(eCube, "crate", glm::vec3(0.4f), glm::vec3(0.8f), glm::vec3(1.0f), 32.0f, glm::vec3(0.0f), true, "./textures/cratebw.jpg");
    Asset floor = Asset(eSquare, "floor", glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(0.5f), 2.0f, glm::vec3(0.0f), true, "./textures/floornoborder.jpg");
    Asset REF = Asset(eCube, "ref", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, glm::vec3(1.0f, 0.0f, 0.0f), false, "");
    Asset ceiling = Asset(eSquare, "ceiling", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, glm::vec3(0.0f), true, "./textures/ceilingbw.jpg");
    Asset wall = Asset(eSquare, "wall", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec3(0.0f), true, "./textures/rivetWallbw.jpg");
    Asset beam = Asset(eCube, "concrete", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec3(0.8f, 0.8f, 0.8f), false, "./textures/concretebw.jpg");
    Asset door = Asset(eSquare, "door", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec3(0.0f), true, "./textures/doorbw.jpg");

    struct Lobby  //offset X, offset Y, dim X, dim Y    ,Opening Height (-1 No window), Beam (0/-1 No, 1 Yes) 
    {
        int length = 20;
        int width = 20;
        int height = 7;

        glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

        std::vector<int> DoorN = {2,0,10,4, -1,1}; 
        std::vector<int> DoorS = {2,0,6,4, 3,1,  
                                 12,0,6,4, 3,1};
        std::vector<int> DoorE = {1,1,1,1, -1,1,  
                                  5,0,3,1, -1,1};
        std::vector<int> DoorW = {1,1,1,1, 1,1,  
                                  2,2,2,2, 1,1,  
                                  5,0,3,1, 1,1};

        std::vector<glm::vec3> lightPos = {glm::vec3(2.0f/10.0f * width, height, length/2.0f),
                                           glm::vec3(5.0f/10.0f * width, height, length/2.0f),
                                           glm::vec3(8.0f/10.0f * width, height, length/2.0f)};

        std::vector<asset> vertical = {asset(1,1, glm::vec3(13.0f, 0.0f, 5.0f)), 
                                       asset(1,2, glm::vec3(13.0f, 0.0f, 6.0f)), 
                                       asset(1,3, glm::vec3(13.0f, 0.0f, 7.0f)),
                                       asset(1,1, glm::vec3(7.0f, 0.0f, 13.0f))};

        std::vector<asset> horizontal = {asset(2,2, glm::vec3(5.0f, 0.0f, 7.0f)), 
                                         asset(2,1, glm::vec3(5.0f, 1.0f, 7.0f))};


    } lobby; 

    struct Lobby2  //offset X, offset Y, dim X, dim Y    ,Opening Height (-1 No window), Beam (0/-1 No, 1 Yes) 
    {
        int length = 20;
        int width = 20;
        int height = 7;

        glm::vec3 offset = glm::vec3(0.0f, 0.0f, -length -0.05f);

        std::vector<int> DoorN = {2,0,6,4, 3,1,  
                                 12,0,6,4, 3,1};
        std::vector<int> DoorS = {2,0,10,4, -1,1};                          
        std::vector<int> DoorE = {1,1,1,1, -1,1,  
                                  5,0,3,1, -1,1};
        std::vector<int> DoorW = {1,1,1,1, 1,1,  
                                  2,2,2,2, 1,1,  
                                  5,0,3,1, 1,1};

        std::vector<glm::vec3> lightPos = {glm::vec3(2.0f/10.0f * width, height, length/2.0f),
                                           glm::vec3(5.0f/10.0f * width, height, length/2.0f),
                                           glm::vec3(8.0f/10.0f * width, height, length/2.0f)};

        std::vector<asset> vertical = {asset(1,1, glm::vec3(10.0f, 0.0f, 11.0f)), 
                                       asset(1,2, glm::vec3(10.0f, 0.0f, 12.0f)), 
                                       asset(1,3, glm::vec3(10.0f, 0.0f, 13.0f)),
                                       asset(1,1, glm::vec3(7.0f, 0.0f, 13.0f))};

        std::vector<asset> horizontal = {asset(2,2, glm::vec3(7.0f, 0.0f, 7.0f)), 
                                         asset(2,1, glm::vec3(7.0f, 1.0f, 7.0f))};

    } lobby2; 
   
    //RENDERER
    Renderer renderer = Renderer();

    //ROOMS
    Room Lobby = Room(lobby.length, lobby.width, lobby.height, lobby.offset, lobby.DoorN, lobby.DoorS, lobby.DoorE, lobby.DoorW, lobby.lightPos, lobby.vertical, lobby.horizontal,
        &floor, &wall, &door, &beam, &ceiling, &crate, &pointLight);
    Lobby.makeRoom(renderer);

    Room Lobby2 = Room(lobby2.length, lobby2.width, lobby2.height, lobby2.offset, lobby2.DoorN, lobby2.DoorS, lobby2.DoorE, lobby2.DoorW, lobby2.lightPos, lobby2.vertical, lobby2.horizontal,
        &floor, &wall, &door, &beam, &ceiling, &crate, &pointLight);
    Lobby2.makeRoom(renderer);

    //SCENE
    nNode* Root = new nNode(); 
    Root->AddChildren(new nAsset(&REF, eObject));

    // CAMERA
    camera = Camera(glm::vec3(5.0f, 5.0f, 5.0f));
    lastX = WINDOW_WIDTH / 2.0f;
    lastY = WINDOW_HEIGHT / 2.0f;
    float ratio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;


    //RENDER LOOP
    while(!glfwWindowShouldClose(window))
    {

        //timing
        float current_frame = glfwGetTime();
        deltaTime = current_frame - lastFrame;
        if(DisplayFPS)
            std::cout << "F.P.S = " << 1.0f/deltaTime << std::endl;
        lastFrame = current_frame;
    
        //Input Processing
        process_input(window);

        if(polygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

        //Render
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        Lobby.cameraCollide(camera);
        Lobby2.cameraCollide(camera);

        renderer.RenderGraph(Root, &camera, ratio);
        renderer.RenderRoom(&Lobby, &camera, ratio);
        renderer.RenderRoom(&Lobby2, &camera, ratio);

        //Swap Buffers, Poll IO events
        if(CapFPS)
            glfwSwapBuffers(window);
        else
            glFlush();
            
        glfwPollEvents();
    }
    
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
    if(glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true); 
        


    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        polygon = !polygon; 

    /*if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && blend < 1.0f)
        blend += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && blend > 0.0f)
        blend -= 0.05f;*/
    if (Gravity)
        camera.Gravity(deltaTime);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(FORWARD, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(BACKWARDS, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(LEFT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(RIGHT, deltaTime);
    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        camera.ProcessKeyboard(UP, deltaTime);
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
