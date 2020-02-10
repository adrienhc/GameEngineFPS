#include "main.h"

int main()
{

    //GLFW WINDOW SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //for OpenGL 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    if(!CapFPS)
       glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

   
    GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "FPS Game", NULL, NULL);
                                                                                    //glfwGetPrimaryMonitor()   --for fullscreen app automatically

    //GLFWwindow* window = glfwCreateWindow( WINDOW_WIDTH, WINDOW_HEIGHT, "FPS Game", glfwGetPrimaryMonitor(), NULL);

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
    //glfwSetScrollCallback(window, scroll_callback);

    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) //setup GLAD, gives pointers to OS specific OpenGL function pointers
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }    

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_STENCIL_TEST);
    glStencilMask(0x00);  

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glClearColor(0.423f, 0.701f, 0.756f, 1.0f);

    //ASSETS
                                        //ambient,  diffuse,  specular,  shininess,  color
    Asset crate = Asset(eCube, "crate", glm::vec3(0.4f), glm::vec3(0.8f), glm::vec3(1.0f), 32.0f, glm::vec3(0.0f), true, "./textures/cratebw.jpg");
    Asset floor = Asset(eSquare, "floor", glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(0.5f), 2.0f, glm::vec3(0.0f), true, "./textures/floornoborder.jpg");
    Asset REF = Asset(eCube, "ref", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, glm::vec3(1.0f, 1.0f, 0.0f), false, "");
    Asset ceiling = Asset(eSquare, "ceiling", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, glm::vec3(0.0f), true, "./textures/ceilingbw.jpg");
    Asset wall = Asset(eSquare, "wall", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec3(0.0f), true, "./textures/rivetWallbw.jpg");
    Asset beam = Asset(eCube, "concrete", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec3(0.8f, 0.8f, 0.8f), false, "./textures/concretebw.jpg");
    Asset door = Asset(eSquare, "door", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec3(0.0f), true, "./textures/doorbw.jpg");


    struct sRoom1  //offset X, offset Y, dim X, dim Y    ,Opening Height (-1 No window), Beam (0/-1 No, 1 Yes) 
    {
        int length = 20;
        int width = 20;
        int height = 10;

        glm::vec3 offset = glm::vec3(0.0f, 0.0f, 0.0f);

        std::vector<int> DoorN = {2,0,10,4, -1,1}; 
        std::vector<int> DoorS = {2,0,6,4, 3,1,  
                                 12,0,6,4, 3,1};
        std::vector<int> DoorE = {1,1,1,1, -1,1,  
                                  5,0,3,1, -1,1};
        std::vector<int> DoorW = {1,1,1,1, 1,1,  
                                  2,2,2,2, 1,1,  
                                  5,0,3,1, 1,1};

            //LIGHT
        glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

        std::vector<glm::vec3> lightPos = {glm::vec3(width/2.0f, height, length/2.0f)};

                                               //ambient        diffuse      //specular  //cst //lin //quad
        PointLight* pointLight = new PointLight(lightPos.size(), glm::vec3(0.6f),  LightColor, LightColor, 1.0f, 0.045f, 0.0075f);

        // CRATES
        std::vector<asset> vertical = {asset(1,1, glm::vec3(13.0f, 0.0f, 5.0f)), 
                                       asset(1,2, glm::vec3(13.0f, 0.0f, 6.0f)), 
                                       asset(1,3, glm::vec3(13.0f, 0.0f, 7.0f)),
                                       asset(1,1, glm::vec3(7.0f, 0.0f, 13.0f))};

        std::vector<asset> horizontal = {asset(2,2, glm::vec3(5.0f, 0.0f, 7.0f)), 
                                         asset(2,1, glm::vec3(5.0f, 1.0f, 7.0f))};

        // TARGETS
        std::vector<asset> target = {asset(glm::vec3(5.5f, 2.0f, 6.5f), 'W'),
                                    asset(glm::vec3(13.5f, 2.0f, 6.0f), 'N')};


    } lobby; 

    struct sRoom2  //offset X, offset Y, dim X, dim Y    ,Opening Height (-1 No window), Beam (0/-1 No, 1 Yes) 
    {
        int length = 20;
        int width = 20;
        int height = 10;

        glm::vec3 offset = glm::vec3(0.0f, 0.0f, -length -0.05f);

        std::vector<int> DoorN = {2,0,6,4, 3,1,  
                                 12,0,6,4, 3,1};
        std::vector<int> DoorS = {2,0,10,4, -1,1};                          
        std::vector<int> DoorE = {1,1,1,1, -1,1,  
                                  5,0,3,1, -1,1};
        std::vector<int> DoorW = {1,1,1,1, 1,1,  
                                  2,2,2,2, 1,1,  
                                  5,0,3,1, 1,1};

        //LIGHT
        glm::vec3 LightColor = glm::vec3(1.0f, 1.0f, 1.0f);

        std::vector<glm::vec3> lightPos = {glm::vec3(width/2.0f, height, length/2.0f)};
                                

                                                                //ambient        diffuse      //specular  //cst //lin = 0.09 //quad = 0.032
        PointLight* pointLight = new PointLight(lightPos.size(), glm::vec3(0.6f),  LightColor, LightColor, 1.0f, 0.045f, 0.0075f);

        // CRATES
        std::vector<asset> vertical = {asset(1,1, glm::vec3(10.0f, 0.0f, 11.0f)), 
                                       asset(1,2, glm::vec3(10.0f, 0.0f, 12.0f)), 
                                       asset(1,3, glm::vec3(10.0f, 0.0f, 13.0f)),
                                       asset(1,1, glm::vec3(7.0f, 0.0f, 13.0f))};

        std::vector<asset> horizontal = {asset(2,2, glm::vec3(7.0f, 0.0f, 7.0f)), 
                                         asset(2,1, glm::vec3(7.0f, 1.0f, 7.0f))};

        // TARGETS POS AND ORIENTATION
        std::vector<asset> target = {asset(glm::vec3(7.5f, 2.0f, 7.5f), 'E'),
                                     asset(glm::vec3(10.0f, 3.0f, 13.5f), 'E')}; 


    } lobby2; 
    
    struct sWeapon1
    {
        char* path = (char*) "3DModels/SMG_Upload/SMG.dae";
        glm::vec3 hip_offset = glm::vec3(0.3f, 0.3f, 0.2f); //FRONT, RIGHT, DOWN
        glm::vec3 ads_offset = glm::vec3(0.1f, -0.002f, 0.135f);
        float scale_factor = 0.1f;
        float zoom_min = 75.0f;
        float zoom_max = 35.0f;
    } smg;
    
    struct sWeapon2
    {
        char* path = (char*) "3DModels/AirGun/AirGun.obj";
        glm::vec3 hip_offset = glm::vec3(0.12f, 0.15f, 0.12f); //FRONT, RIGHT, DOWN
        glm::vec3 ads_offset = glm::vec3(0.15f, 0.002f, 0.1f);
        float scale_factor = 1.2f;
        float zoom_min = 75.0f;
        float zoom_max = 35.0f;
    } airgun;


    //RENDERER
    Renderer renderer = Renderer();

    //SKYBOX
    Skybox skybox("./skybox/PositiveX.png",
                  "./skybox/NegativeX.png",
                  "./skybox/PositiveY.png",
                  "./skybox/NegativeY.png",
                  "./skybox/PositiveZ.png",
                  "./skybox/NegativeZ.png");

    //TARGET
    Target::LoadModel((char*) "3DModels/Target/poligono1.obj");
    Target::LoadSmoothModel((char*) "3DModels/SmoothTarget/poligono1.obj");

    //ROOMS
    Room Lobby = Room(lobby.length, lobby.width, lobby.height, lobby.offset, lobby.DoorN, lobby.DoorS, lobby.DoorE, lobby.DoorW, 
        lobby.lightPos, lobby.vertical, lobby.horizontal, lobby.target,
        &floor, &wall, &door, &beam, &ceiling, &crate, lobby.pointLight);
    Lobby.makeRoom(renderer);

    Room Lobby2 = Room(lobby2.length, lobby2.width, lobby2.height, lobby2.offset, lobby2.DoorN, lobby2.DoorS, lobby2.DoorE, lobby2.DoorW, 
        lobby2.lightPos, lobby2.vertical, lobby2.horizontal, lobby2.target,
        &floor, &wall, &door, &beam, &ceiling, &crate, lobby2.pointLight);
    Lobby2.makeRoom(renderer);

    //WEAPONS
    Weapon SMG(smg.path, smg.hip_offset, smg.ads_offset, smg.scale_factor, smg.zoom_min, smg.zoom_max);
    Weapon ARG(airgun.path, airgun.hip_offset, airgun.ads_offset, airgun.scale_factor, airgun.zoom_min, airgun.zoom_max);

    // CAMERA
    camera = Camera(glm::vec3(5.0f, 5.0f, 3.0f));
    lastX = WINDOW_WIDTH / 2.0f;
    lastY = WINDOW_HEIGHT / 2.0f;
    float ratio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
    camera.SetView(0.01f, 50.0f, ratio);

    nNode* Root = new nNode();
    nNode* Ref = Root->AddChildrenRecursive(new nTranslate(glm::vec3(5.5f, 2.0f + 0.5f, 6.5f)));
    
    nNode* Reset = Ref;
    Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(1.17f, 0.85f, 0.1f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(-0.05f, -0.75f, 0.0f)));
    Ref->AddChildren(new nAsset(&REF, eObject));

    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(1.5f, 0.95f, 0.1f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.1f, 0.1f, 0.0f)));
    Ref->AddChildren(new nAsset(&REF, eObject));

    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(0.85f, 0.8f, 0.1f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.8f, 0.0f)));
    Ref->AddChildren(new nAsset(&REF, eObject));

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
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        Room::setupCollisions();   
        Lobby.collisionChecks(camera);
        Lobby2.collisionChecks(camera);
        Lobby.postCollisions();
        Lobby2.postCollisions();


        //renderer.RenderGraph(Root, &camera);
        
        renderer.RenderRoom(&Lobby, &camera);
        renderer.RenderRoom(&Lobby2, &camera);

        Lobby.getLights(renderer);
        Lobby2.getLights(renderer);


        renderer.RenderSkybox(&skybox, &camera);

        renderer.RenderWeapon(&SMG, &camera, deltaTime);
        //renderer.RenderWeapon(&ARG, &camera);

        
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
        
    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
    {
            Weapon::ads = true;    
    }
    else
        Weapon::ads = false;

    if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
    {
            Weapon::fire = true;    
    }
    else
        Weapon::fire = false;

    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        polygon = !polygon; 

    /*if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && blend < 1.0f)
        blend += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && blend > 0.0f)
        blend -= 0.05f;*/

    if (Room::collideOn() && Gravity)
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
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        if(!Weapon::reload)
            Weapon::reload = true;
    }
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

//void scroll_callback( GLFWwindow* window, double xoffset, double yoffset)
//{
    //camera.ProcessMouseScroll(yoffset);
//}
