#include "main.h"

int main()
{

    //GLFW WINDOW SETUP
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4); //for OpenGL 4.5
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_FALSE);

    if(!CapFPS)
       glfwWindowHint( GLFW_DOUBLEBUFFER, GL_FALSE );

   
    //
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

    glClearColor(0.423f, 0.701f, 0.756f, 1.0f);

    glEnable(GL_MULTISAMPLE); 
    glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
    glEnable(GL_PROGRAM_POINT_SIZE);
    glEnable(GL_LINE_SMOOTH);

    //ASSETS   
    Asset* REF = new Asset(eCube, "ref", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, glm::vec4(0.0f, 1.0f, 0.0f, 0.5f), false, "");
    Asset* crosshair = new Asset(eSquare, "crosshair", glm::vec3(1.0f), glm::vec3(0.0f), glm::vec3(0.0f), 32.0f, glm::vec4(0.0f), true, "./textures/crosshair.png");
    Asset* crate = new Asset(eCube, "crate", glm::vec3(0.4f), glm::vec3(0.8f), glm::vec3(1.0f), 32.0f, glm::vec4(1.0f), true, "./textures/cratebw.jpg");
    Asset* floor = new Asset(eSquare, "floor", glm::vec3(0.5f), glm::vec3(1.0f), glm::vec3(0.5f), 2.0f, glm::vec4(1.0f), true, "./textures/floornoborder.jpg");
    Asset* ceiling = new Asset(eSquare, "ceiling", glm::vec3(1.0f), glm::vec3(1.0f), glm::vec3(1.0f), 32.0f, glm::vec4(1.0f), true, "./textures/ceilingbw.jpg");
    Asset* wall = new Asset(eSquare, "wall", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec4(1.0f), true, "./textures/rivetWallbw.jpg");
    Asset* beam = new Asset(eCube, "concrete", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), false, "");//"./textures/concretebw.jpg");
    Asset* door = new Asset(eSquare, "door", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec4(1.0f), true, "./textures/doorbw.jpg");

    //TARGET
    Target::LoadModel((char*) "3DModels/Target/poligono1.obj");
    Target::LoadSmoothModel((char*) "3DModels/SmoothTarget/poligono1.obj");

    std::vector<Room*> World;

    
    /* //OLD BIG SMG
    struct sWeapon1
    {
        char* path = (char*) "3DModels/SMG_Upload/SMG.dae";
        glm::vec3 hip_offset = glm::vec3(0.3f, 0.3f, 0.2f); //FRONT, RIGHT, DOWN
        glm::vec3 ads_offset = glm::vec3(0.1f, -0.002f, 0.135f);
        float scale_factor = 0.1f;
        float zoom_min = 75.0f;
        float zoom_max = 35.0f;
    } smg;*/

    struct sWeapon1
    {
        char* path = (char*) "3DModels/SMG_Upload/SMG.dae";
        glm::vec3 hip_offset = glm::vec3(0.03f, 0.03f, 0.02f); //FRONT, RIGHT, DOWN
        glm::vec3 ads_offset = glm::vec3(0.02f, -0.0002f, 0.0135f);
        float scale_factor = 0.01f;
        float zoom_min = 75.0f;
        float zoom_max = 35.0f;
    } smg;
    
    struct sWeapon2
    {
        //char* path = (char*) "3DModels/L96 Sniper Rifle/L96.obj";
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

    //WEAPONS
    Weapon SMG(smg.path, smg.hip_offset, smg.ads_offset, smg.scale_factor, smg.zoom_min, smg.zoom_max);
    // Weapon ARG(airgun.path, airgun.hip_offset, airgun.ads_offset, airgun.scale_factor, airgun.zoom_min, airgun.zoom_max);

    // CAMERA
    camera = Camera(glm::vec3(5.0f, 5.0f, 3.0f));
    lastX = WINDOW_WIDTH / 2.0f;
    lastY = WINDOW_HEIGHT / 2.0f;
    float ratio = (float) WINDOW_WIDTH / (float) WINDOW_HEIGHT;
    camera.SetView(0.01f, 50.0f, ratio);

    Player Player(&camera, &SMG);

    //LAYERS
    Shader batchShader = Shader("shaders/batch/batchShader.vs", "shaders/batch/batchShader.fs"); 
    SceneLayer weapon = SceneLayer(&camera, &batchShader);
    SceneLayer targets = SceneLayer(&camera, &batchShader);
    SceneLayer scene = SceneLayer(&camera, &batchShader);
    //InstancedLayer instanced = InstancedLayer(&camera, &batchShader);

    Shader outlineShader = Shader("shaders/batch/outlineShader.vs", "shaders/batch/outlineShader.fs"); 
    OutlineLayer outline = OutlineLayer(&camera, &outlineShader);

    ParticleSystem ParticleSys = ParticleSystem();                                                    //or Point or Cube
    Shader particleCubeShader = Shader("shaders/particles/particleShader.vs", "shaders/particles/particleCubeShader.gs", "shaders/particles/particleShader.fs");
    ParticleLayer particleCube = ParticleLayer(&camera, &particleCubeShader, &ParticleSys, pCube);

    Shader particlePointShader = Shader("shaders/particles/particleShader.vs", "shaders/particles/particlePointShader.gs", "shaders/particles/particleShader.fs");
    ParticleLayer particlePoint = ParticleLayer(&camera, &particlePointShader, &ParticleSys, pPoint);
    
    Shader particleLineShader = Shader("shaders/particles/particleShader.vs", "shaders/particles/particleLineShader.gs", "shaders/particles/particleShader.fs");
    ParticleLayer particleLine = ParticleLayer(&camera, &particleLineShader, &ParticleSys, pLine);

    Shader depthShader = Shader("shaders/batch/depthShader.vs", "shaders/batch/depthShader.gs", "shaders/batch/depthShader.fs");
    DepthmapLayer depthmap = DepthmapLayer(&camera, &depthShader); 

    //TARGET BOUNDING BOXES
    /*
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
    */

    //RNG SEED
    srand (static_cast <unsigned> (glfwGetTime()));
    //Fragment Shader supports 32 textures!
    float deltaTimeAcc = 0.0f;
    // float shadowCooldown = 0.0f;
    int Frames;

    //RENDER LOOP
    bool printNext = false;


    while(!glfwWindowShouldClose(window))
    {

        //TIMING
        float current_frame = glfwGetTime();
        deltaTime = current_frame - lastFrame;
        if(DisplayFPS)
        {
            deltaTimeAcc += deltaTime;
            Frames++;
            if(deltaTimeAcc > 1.0f)
            {
                std::cout << Frames << " fps " << deltaTime << std::endl;
                Frames = 0;
                deltaTimeAcc = 0.0f;
            }
        }
        lastFrame = current_frame;

        if(printNext)
        {
            std::cout << "Next dt " << deltaTime << std::endl;
            printNext = false;
        }

        //WORLD BUILDING
        if(BuildWorld == true)
        {
            build_world(World, floor, wall, door, beam, ceiling, crate);

            //DEPTHMAP, TARGETS AND OUTLINE
            glStencilMask(0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            depthmap.Clear();
            for(int i = 0; i < World.size(); i++)
            {
                World[i]->addLightsLayer(&depthmap);
                World[i]->addLayoutLayer(&depthmap);
                World[i]->addTargetsLayer(NULL, NULL, NULL, &depthmap);
            }
            depthmap.Render(); //Render DepthMap of scene from each of light's layers POV

        }

        //INPUT PROCESSING
        process_input(window);

        //GAME LOGIC
        Room::setupCollisions();  
        for(int i = 0; i < World.size(); i++)
        {
            World[i]->collisionChecks(camera);
            World[i]->postCollisions();
        }

        //TIME UPDATE
        ParticleSys.Update(deltaTime);
        Player.Update(deltaTime);

        //PARTICLE SYSTEM
        if(World.size() > 0)
            World[0]->addBulletImpact(&camera, &scene, &ParticleSys);
        Player.addParticle(&ParticleSys);
        
        //RENDERING
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

        if(polygon)
            glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);


        //DEPTHMAP LAYER
        if(Room::shadowPass)
        {
            std::cout << "Update " << deltaTime << std::endl;
            printNext = true;
            //DEPTHMAP, TARGETS AND OUTLINE
            glStencilMask(0xFF);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);

            depthmap.Clear();
            for(int i = 0; i < World.size(); i++)
            {
                World[i]->addLightsLayer(&depthmap);
                World[i]->addLayoutLayer(&depthmap);
                World[i]->addTargetsLayer(NULL, NULL, NULL, &depthmap);
            }
            depthmap.Render(); //Render DepthMap of scene from each of light's layers POV

        }
        
        //PLAYER LAYER
        //Do Stencil Shennenigans so that Gun Not Overwritten
        
        //glStencilMask(0xFF);
        //glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        //glStencilFunc(GL_GEQUAL, 2, 0xFF);
        weapon.Clear();
        for(int i = 0; i < World.size(); i++)
        {
            World[i]->addLightsLayer(&weapon);
        }
        //Player.addLayer(&weapon); //NO MUZZLE FLASH
        //glEnable(GL_CULL_FACE); 
        //glCullFace(GL_BACK);
        //weapon.Render();
        //glDisable(GL_CULL_FACE); 
        

        //OUTLINED OBJECTS LAYER 
        //Target //if shared outline ok
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_GEQUAL, 1, 0XFF);
        targets.Clear();
        outline.Clear();
        for(int i = 0; i < World.size(); i++)
        {
            World[i]->addLightsLayer(&targets);
            World[i]->addTargetsLayer(&targets, &outline, NULL, NULL);
        }
        targets.Render();
        //Outline //if shared outline ok 
        glStencilFunc(GL_GREATER, 1, 0xFF);
        outline.Render();

        //ROOM LAYER
        glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        glStencilFunc(GL_GREATER, 2, 0XFF); //From here on, do not overwrite Gun
        //STATIC GEOMETRY
        //SCENE
        scene.Clear();
        Player.addLayer(&scene); //WORKS MODEL THEN ASSETS
        for(int i = 0; i < World.size(); i++)
        {
            World[i]->addLightsLayer(&scene, true);
            World[i]->addLayoutLayer(&scene);   
        }
        //Player.addLayer(&scene); //NOT WORK ASSET THEN MODELS
        scene.Render();

        //PARTICLES LAYER
        particleCube.ClearLight();
        for(int i = 0; i < World.size(); i++)
        {
            World[i]->addLightsLayer(&particleCube);
            World[i]->addTargetsLayer(NULL, NULL, &particleCube, NULL);
        }      
        
        //SKYBOX LAYER
        renderer.RenderSkybox(&skybox, &camera);
        
        //PARTICLES RENDER
        particleCube.Render();
        particlePoint.Render();
        particleLine.Render();

        //TRANSPARENCY ITEMS 
        Player.addLayer(&weapon, true); //ADD MUZZLE FLASH
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        weapon.Render();
        glDisable(GL_BLEND);

        if(firstPass)
            firstPass = false;

        //Swap Buffers, Poll IO events
        if(CapFPS)
            glfwSwapBuffers(window);
        else
            glFlush();
            
        glfwPollEvents();
    }

    delete REF;
    delete crosshair;
    delete floor;
    delete wall;
    delete door;
    delete ceiling;
    delete crate;
    delete beam;

    for(int i = 0; i < World.size(); i++)
    {
        delete World[i];
    }
    //terminate GLFW Window
    //glfwTerminate();

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

    if(!FixCamera)
    {
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
    
        
    

    if(glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS)
        polygon = !polygon; 

    /*if(glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS && blend < 1.0f)
        blend += 0.05f;
    if(glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS && blend > 0.0f)
        blend -= 0.05f;*/

    

    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        BuildWorld = true;
        FixCamera = false;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
        sleep(0.5f);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        FixCamera = true;
        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
        sleep(0.3f);
    }



}

void mouse_callback( GLFWwindow* window, double xpos, double ypos)
{
    if(!FixCamera)
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
    
}

//void scroll_callback( GLFWwindow* window, double xoffset, double yoffset)
//{
    //camera.ProcessMouseScroll(yoffset);
//}

void build_world(std::vector<Room*>& World, Asset* floor, Asset* wall, Asset* door, Asset* beam, Asset* ceiling, Asset* crate)
{
    for(int i = 0; i < World.size(); i++)
    {
        delete World[i];
    }
    World.clear();
    

    for(int i = 0; i < NumRooms; i++)
    {
        auto dlRoom = new DLClass<BaseRoom>("./dynamic_class/libRoom"+std::to_string(i)+".so");
        std::shared_ptr<BaseRoom> room = dlRoom->make_obj();
        delete dlRoom;

        if(!room) 
        {
            std::cerr << "Failed to instantiate room - "+std::to_string(i)+"." << std::endl;
            // return 1;
        }
        else
        {
            std::cerr << "Success to instantiate room - "+std::to_string(i)+"." << std::endl;   
        }

        PointLight* pointLight = new PointLight(room->lightPos.size(), room->lightAmbient,  room->LightColor, room->LightColor, room->lightConstant, room->lightLinear, room->lightQuadratic);
        
        Room* current_room = new Room(room->length, room->width, room->height, room->offset, room->DoorN, room->DoorS, room->DoorE, room->DoorW, 
            room->lightPos, room->lightRadius, room->vertical, room->horizontal, room->target, room->stairs,
            floor, wall, door, beam, ceiling, crate, pointLight);
        
        current_room->makeRoom();

        World.push_back(current_room);
        BuildWorld = false;
    }
}