#include "room.h"

bool Room::active = true;

Room::Room(int lgth, int wdth, int hght, glm::vec3 ofst,  std::vector<int> DN,  std::vector<int> DS, std::vector<int> DE, std::vector<int> DW, std::vector<glm::vec3> ptLghtPs, std::vector<asset> vrtcl, std::vector<asset> hrztl,
    Asset* flr, Asset* wll, Asset* dr, Asset* bm, Asset* ceil, Asset* crte, PointLight* ptLght)
{
	length = lgth;
	width = wdth;
	height = hght;
    offset = ofst;

    room_min_bb = glm::vec3(offset.x - 0.5f, offset.y, offset.z - 0.5f );
    room_max_bb = glm::vec3(room_min_bb.x + width, room_min_bb.y + height, room_min_bb.z + length);

	DoorN = DN;
	DoorS = DS;
	DoorE = DE;
	DoorW = DW;

    pointLightPos = ptLghtPs;

    vertical = vrtcl;
    horizontal = hrztl;

	floor = flr; 
	wall = wll; 
	door = dr; 
	beam = bm;
	ceiling = ceil;
    crate = crte;
    pointLight = ptLght;
}

Room::~Room()
{
    delete Root;
    delete Lights;
}

glm::vec3 Room::adjustAssetPos(glm::vec3 Pos)
{
    return glm::vec3(offset.x + Pos.x, offset.y + 0.5f + Pos.y, offset.z + Pos.z);
} 

void Room::makeRoom(Renderer renderer) //MAKES SCENE GRAPH, AND INSTANTIATES ROOM'S GEOMETRY
{
    //NON INSTANTIATED
    Root = new nNode();
    Lights = new nNode();

    //LIGHTS
    nNode* Ref = Root;
    nNode* RefLights = Lights;

    for(int i = 0; i < pointLightPos.size(); i++)
    {
        Ref = Root->AddChildrenRecursive(new nTranslate(glm::vec3(offset.x - 0.5f + pointLightPos[i].x, offset.y - 0.1f + pointLightPos[i].y, offset.z - 0.5f + pointLightPos[i].z)));
        RefLights = Lights->AddChildrenRecursive(new nTranslate(glm::vec3(offset.x - 0.5f + pointLightPos[i].x, offset.y - 0.1f + pointLightPos[i].y, offset.z - 0.5f + pointLightPos[i].z)));
        Ref->AddChildren(new nPointLight(pointLight, i));
        RefLights->AddChildren(new nPointLight(pointLight, i));
    }

    //INSTANTIATE
    std::vector<glm::mat4> inst_tr;
    std::vector<glm::mat3> inst_norm;

    //INSTANTIATED 
    //WALLS + DOOR + BEAM + CRATES
    nNode* Instanciated = new nNode();
    Ref = Instanciated->AddChildrenRecursive(new nRotate(glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f), true)); //FLOOR
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = horizontalPlane(Ref, floor);
    Ref = Instanciated;
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, (float) height, 0.0f))); //CEILING
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = horizontalPlane(Ref, ceiling);

    renderer.Instance(Instanciated, eRoot, floor, inst_tr, inst_norm); //FLOOR
    i_floor = new cSquare(inst_tr, inst_norm);
    inst_tr.clear();
    inst_norm.clear();

    renderer.Instance(Instanciated, eRoot, ceiling, inst_tr, inst_norm); //CEILING
    i_ceiling = new cSquare(inst_tr, inst_norm);
    inst_tr.clear();
    inst_norm.clear();

    delete Instanciated;

    Instanciated = new nNode();
    Ref = Instanciated->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.5f, -0.5f))); //SOUTH
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = verticalPlaneNS(Ref, DoorS);
    Ref = Instanciated;
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.5f, -0.5f))); 
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, (float) length))); //NORTH
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = verticalPlaneNS(Ref, DoorN);
    
    Ref = Instanciated->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(-0.5f, 0.5f, 0.0f))); //EAST
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = verticalPlaneEW(Ref, DoorE);
    Ref = Instanciated;
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(-0.5f, 0.5f, 0.0f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3((float) width, 0.0f, 0.0f))); //WEST
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = verticalPlaneEW(Ref, DoorW);

    renderer.Instance(Instanciated, eRoot, wall, inst_tr, inst_norm); //WALL
    i_wall = new cSquare(inst_tr, inst_norm);
    inst_tr.clear();
    inst_norm.clear();

    renderer.Instance(Instanciated, eRoot, door, inst_tr, inst_norm); //DOOR
    i_door = new cSquare(inst_tr, inst_norm);
    inst_tr.clear();
    inst_norm.clear();    

    renderer.Instance(Instanciated, eRoot, beam, inst_tr, inst_norm); //BEAM
    i_beam = new cCube(inst_tr, inst_norm);
    inst_tr.clear();
    inst_norm.clear();

    delete Instanciated;
    
    Instanciated = new nNode();
    //VERTICAL CRATES
    for(int i = 0; i < vertical.size(); i++)
    {
       asset_bb.push_back(Cluster::Vertical(Instanciated, vertical[i].x, vertical[i].y, adjustAssetPos(vertical[i].pos), glm::radians(0.0f), crate));
    }
    //HORIZONTAL CRATES
    for(int i = 0; i < horizontal.size(); i++)
    {
       asset_bb.push_back(Cluster::Horizontal(Instanciated, horizontal[i].x, horizontal[i].y, adjustAssetPos(horizontal[i].pos), glm::radians(0.0f), crate));
    }

    renderer.Instance(Instanciated, eRoot, crate, inst_tr, inst_norm); //CRATES
    i_crate = new cCube(inst_tr, inst_norm);
    inst_tr.clear();
    inst_norm.clear(); 

    delete Instanciated;
}


nNode* Room::horizontalPlane(nNode* Root, Asset* asset) //floor or ceiling
{
    nNode* Ref = Root;
    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Ref = Ref->AddChildrenRecursive(new nAsset(asset, eInstanced)); //before eObject
            if(j < width-1)
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(1.0f, 0.0f, 0.0f)));
        }
        
        Ref = Ref->AddChildrenRecursive(new nTranslate (glm::vec3(-1.0f * (width - 1), 0.0f, 1.0f)));
    }

    return Root;
}


nNode* Room::verticalPlaneNS(nNode* Root, std::vector<int> Door)
{
    nNode* Ref = Root;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            if( !isDoor(i, j, Door) )
                Ref = Ref->AddChildrenRecursive(new nAsset(wall, eInstanced));
            else
            {
                if( !pointOpeningDoor(i, j, Door) )                      
                    Ref = Ref->AddChildrenRecursive(new nAsset(door, eInstanced));    
            }

            if(j < width-1)
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(1.0f, 0.0f, 0.0f)));
        }
        
        Ref = Ref->AddChildrenRecursive(new nTranslate (glm::vec3(-1.0f * (width - 1), 1.0f, 0.0f)));
    }

    for(int i = 0; i < Door.size(); i+=doorSpec)
    {
        if(hasBeam(Door[i+5]))
        {
    	   std::vector<int> mDoor(Door.begin()+i, Door.begin()+i+4);
    	   Root = addBeamNS(Root, mDoor);
        }
    }

    return Root;
}


nNode* Room::verticalPlaneEW(nNode* Root, std::vector<int> Door)
{
    nNode* Ref = Root;
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < length; j++)
        {
            if( !isDoor(i, j, Door) )
                Ref = Ref->AddChildrenRecursive(new nAsset(wall, eInstanced));
            else
            {
                if( !pointOpeningDoor(i, j, Door) )                      
                    Ref = Ref->AddChildrenRecursive(new nAsset(door, eInstanced));
                
            }
            if(j < length-1)
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, 1.0f)));
        }
        
        Ref = Ref->AddChildrenRecursive(new nTranslate (glm::vec3(0.0f, 1.0f, -1.0f * (length - 1))));
    }

    for(int i = 0; i < Door.size(); i+=doorSpec)
    {
        if(hasBeam(Door[i+5]))
        {
    	   std::vector<int> mDoor(Door.begin()+i, Door.begin()+i+4);
    	   Root = addBeamEW(Root, mDoor);
        }
    }

    return Root;
}


bool Room::isDoor(int height, int width, std::vector<int> Door)
{	
	bool ret = false;
	for(int i = 0; i < Door.size(); i+=doorSpec)
		ret |= (Door[i+1] <= height) && (height < Door[i+1] + Door[i+3]) && ((Door[i+0] <= width) && (width < Door[i+0] + Door[i+2]));
    return ret;

}

bool Room::pointOpeningDoor(int height, int width, std::vector<int> Door) //RETURN HEIGHT DOOR
{
    
    for(int i = 0; i < Door.size(); i+=doorSpec)   
        if( (Door[i+1] <= height) && (height < Door[i+1] + Door[i+3]) && ((Door[i+0] <= width) && (width < Door[i+0] + Door[i+2])) )
        {
            if(Door[i+4] == -1) //-1 to specify No Door
                return true;
            else
                return  height < (Door[i+1] + Door[i+4]);   //current sampled height < y offset + opening height
        }

    return false;
}


bool Room::boxOpeningDoor(int ref_height, int ref_width, int other_height, int other_width, std::vector<int> Door) //ONLY LET THROUGH IF SAME DOOR
{
    for(int i = 0; i < Door.size(); i+=doorSpec)
    {   
        if( (Door[i+1] <= ref_height) && (ref_height < Door[i+1] + Door[i+3]) && ((Door[i+0] <= ref_width) && (ref_width < Door[i+0] + Door[i+2])) &&         // MIN AND
            (Door[i+1] <= other_height) && (other_height < Door[i+1] + Door[i+3]) && ((Door[i+0] <= other_width) && (other_width < Door[i+0] + Door[i+2])) )  // MAX
        {
            if(Door[i+4] == -1) //-1 to specify No Door
                return true;
            else
                return  (ref_height < (Door[i+1] + Door[i+4])) && (other_height < (Door[i+1] + Door[i+4]));   //current sampled height < y offset + opening height
        }
    }

    return false;
}


bool Room::hasBeam(int ibool) //True is Door has a beam
{
    return ibool == 1;    
}

nNode* Room::addBeamNS(nNode* Root, std::vector<int> Door)
{
    
    nNode* Ref = Root->AddChildrenRecursive(new nScale(glm::vec3(0.2f, 1.0f, 0.05f))); //DIMENSIONS BEAM
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3( -0.5f, 0.0f, 0.0f))); //ALIGN X ROOM
    nNode* Reset = Ref;

    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3((float) Door[0], Door[1], 0.0f)));
    for(int i = 0; i < Door[3]; i++)
    {
        Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
        if(i < Door[3])
           Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 1.0f, 0.0f)));        
    }

    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3((float) (Door[0] + Door[2]), Door[1], 0.0f)));
    for(int i = 0; i < Door[3]; i++)
    {
        Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
        if(i < Door[3])
           Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 1.0f, 0.0f)));        
    }

    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(-offset.x, -offset.y, -offset.z)));
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f), true)); //ALIGN Y ROOM
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.5f, 0.5f, 0.0f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Reset = Ref; //Avoids redoing rotation and alignment

    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3((float) Door[0], Door[1], 0.0f))); 
    
    //if(Door[1] != 0)    //IF DOOR DOESNT TOUCH FLOOR
        for(int i = 0; i < Door[2]; i++)
        {
            Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
            if(i < Door[2])
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(1.0f, 0.0f, 0.0f)));        
        }


    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3((float) Door[0], (float) (Door[1] + Door[3]), 0.0f))); 
    
    for(int i = 0; i < Door[2]; i++)
    {
        Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
        if(i < Door[2])
            Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(1.0f, 0.0f, 0.0f)));        
    }


    return Root;
}


nNode* Room::addBeamEW(nNode* Root, std::vector<int> Door)
{
    
    nNode* Ref = Root->AddChildrenRecursive(new nScale(glm::vec3(0.2f, 1.0f, 0.05f))); //DIMENSIONS BEAM
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3( 0.0f, 0.0f, -0.5f))); //ALIGN X ROOM
    nNode* Reset = Ref;

    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, Door[1], (float) Door[0])));
    for(int i = 0; i < Door[3]; i++)
    {
        Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
        if(i < Door[3])
           Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 1.0f, 0.0f)));        
    }

    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, Door[1], (float) (Door[0] + Door[2]))));
    for(int i = 0; i < Door[3]; i++)
    {
        Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
        if(i < Door[3])
           Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 1.0f, 0.0f)));        
    }

    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nTranslate((glm::vec3(-offset.x, -offset.y, -offset.z))));
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f), true)); //ALIGN Y ROOM
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, -0.5f, -0.5f)));
    Reset = Ref; //Avoids redoing rotation and alignment

    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3( 0.0f, (float) Door[1], (float) Door[0] ))); 
    
    //if(Door[1] != 0)    //IF DOOR DOESNT TOUCH FLOOR
        for(int i = 0; i < Door[2]; i++)
        {
            Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
            if(i < Door[2])
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, 1.0f)));        
        }


    Ref = Reset;
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, (float) (Door[1] + Door[3]), (float) Door[0]))); 
    
    for(int i = 0; i < Door[2]; i++)
    {
        Ref = Ref->AddChildrenRecursive(new nAsset(beam, eInstanced));
        if(i < Door[2])
            Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, 1.0f)));        
    }


    return Root;
}



bool Room::pointCollide(glm::vec3 &playerPos, glm::vec3 min_bb, glm::vec3 max_bb) 
{
    bool isWall = (min_bb == room_min_bb) && (max_bb == room_max_bb); //TEST IS BB IS WALL TO ONLY CHECK FOR DOORS IN THAT CASE

    if(Collision::pointCollide(playerPos, min_bb, max_bb)) //AS  OF NOW JUST DO ROOM
    {
        if (isWall)
            handlePointWall(playerPos, min_bb, max_bb);
    }
}



bool Room::boxCollide(glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb)
{
    bool collision = Collision::boxCollide(player_min_bb, player_max_bb, min_bb, max_bb);

    if( collision )
    {
    
        glm::vec3 dist_bb = glm::vec3(player_max_bb.x - player_min_bb.x, player_max_bb.y - player_min_bb.y, player_max_bb.z - player_min_bb.z);
        bool isWall = (min_bb == room_min_bb) && (max_bb == room_max_bb); //TEST IS BB IS WALL TO ONLY CHECK FOR DOORS IN THAT CASE

        if(isWall)
        {
            handleBoxWall("min", player_min_bb, player_max_bb, min_bb, max_bb);
            player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
            handleBoxWall("max", player_min_bb, player_max_bb, min_bb, max_bb);
            player_min_bb = glm::vec3(player_max_bb.x - dist_bb.x, player_max_bb.y - dist_bb.y, player_max_bb.z - dist_bb.z);
        }
        else 
        {
            std::string update;

            update = handleBoxObject("on top", player_min_bb, player_max_bb, min_bb, max_bb);
            if (update == "max")
            {
                //std::cout << "ont top" << std::endl;
                player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
                return true;
            }

            update = handleBoxObject("below", player_min_bb, player_max_bb, min_bb, max_bb);
            if (update == "min")
            {
                //std::cout << "below" << std::endl;
                player_min_bb = glm::vec3(player_max_bb.x - dist_bb.x, player_max_bb.y - dist_bb.y, player_max_bb.z - dist_bb.z);
                return true;
            }

            update = handleBoxObject("min min", player_min_bb, player_max_bb, min_bb, max_bb);
            if(update == "max")
            {
                player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
                return true;
            }
            
            update = handleBoxObject("max max", player_min_bb, player_max_bb, min_bb, max_bb);
            if(update == "min")
            {
                player_min_bb = glm::vec3(player_max_bb.x - dist_bb.x, player_max_bb.y - dist_bb.y, player_max_bb.z - dist_bb.z);
                return true;
            }

            
            update = handleBoxObject("min max", player_min_bb, player_max_bb, min_bb, max_bb);
            if(update == "min")
            {
                player_min_bb = glm::vec3(player_max_bb.x - dist_bb.x, player_max_bb.y - dist_bb.y, player_max_bb.z - dist_bb.z);
                return true;
            }
            else if(update == "max")
            {
                player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
                return true;
            }
            else if(update == "both")
            {
               player_min_bb = glm::vec3(player_min_bb.x, player_max_bb.y - dist_bb.y, player_max_bb.z - dist_bb.z);
               player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_max_bb.z);
                return true;
            }

            
            update = handleBoxObject("max min", player_min_bb, player_max_bb, min_bb, max_bb);
            if(update == "min")
            {
                player_min_bb = glm::vec3(player_max_bb.x - dist_bb.x, player_max_bb.y - dist_bb.y, player_max_bb.z - dist_bb.z);
                return true;
            }
            else if(update == "max")
            {
                player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
                return true;
            }
            else if(update == "both")
            {
                player_min_bb = glm::vec3(player_max_bb.x - dist_bb.x, player_max_bb.y - dist_bb.y, player_min_bb.z);
                player_max_bb = glm::vec3(player_max_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
                return true;
            }
        }

           
    }
    
    return collision;
}


void Room::handlePointWall(glm::vec3 &playerPos, glm::vec3 min_bb, glm::vec3 max_bb)
{
    if ( playerPos.x < min_bb.x + buffer ) //EAST
    {
        if ( !pointOpeningDoor(playerPos.y, playerPos.z + 0.5f - offset.z, DoorE) )
            playerPos.x = min_bb.x + buffer;
    }

    if ( max_bb.x - buffer < playerPos.x ) //WEST
    {
        if ( !pointOpeningDoor(playerPos.y, playerPos.z + 0.5f - offset.z, DoorW) )
            playerPos.x = max_bb.x - buffer;
    }

    if ( playerPos.z < min_bb.z + buffer ) //SOUTH
    {
        if ( !pointOpeningDoor(playerPos.y, playerPos.x + 0.5f - offset.x, DoorS) )
            playerPos.z = min_bb.z + buffer;
    }

    if ( max_bb.z - buffer < playerPos.z ) //NORTH
    {
        if ( !pointOpeningDoor(playerPos.y, playerPos.x + 0.5f - offset.x, DoorN) )
            playerPos.z = max_bb.z - buffer;
    }

    if ( playerPos.y < min_bb.y + buffer ) //FLOOR
    {
        playerPos.y = min_bb.y + buffer;
    }       

    if ( max_bb.y - buffer < playerPos.y ) //CEILING
    {
        playerPos.y = max_bb.y - buffer;
    }
}

void Room::handleBoxWall(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb) //HANDLE WITH SETTING DIR VEL TO 0 
{                                                                                                                                    //If collide cam bottom bb, set vel y to 0, ow do not touch it
                                                                                                                              //Can only jump if vel y is 0

    if (update == "min")
    {

        if ( player_min_bb.x < min_bb.x ) //EAST
        {
            //std::cout << "MIN HIT EAST" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.z + 0.5f - offset.z, player_max_bb.y, player_max_bb.z + 0.5f - offset.z, DoorE) )
                player_min_bb.x = min_bb.x;   
        }
        /*
        if ( max_bb.x < player_min_bb.x) //WEST
        {
            std::cout << "MIN HIT WEST" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.z + 0.5f - offset.z, player_max_bb.y, player_max_bb.z + 0.5f - offset.z, DoorW) )
                player_min_bb.x = max_bb.x;
        }
       */
        if ( player_min_bb.z < min_bb.z) //SOUTH
        {
            //std::cout << "MIN HIT SOUTH" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.x + 0.5f - offset.x, player_max_bb.y, player_max_bb.x + 0.5f - offset.x, DoorS) )
                player_min_bb.z = min_bb.z;
        }
        /*
        if ( max_bb.z < player_min_bb.z) //NORTH
        {
            std::cout << "MIN HIT NORTH" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.x + 0.5f - offset.x, player_max_bb.y, player_max_bb.x + 0.5f - offset.x, DoorN) )
                player_min_bb.z = max_bb.z;
        }
        */
        if (player_min_bb.y < min_bb.y) //FLOOR
            player_min_bb.y = min_bb.y;
    }
    else if (update == "max")
    {
        /*
        if ( player_max_bb.x < min_bb.x ) //EAST
        {
            std::cout << "MAX HIT EAST" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.z + 0.5f - offset.z, player_max_bb.y, player_max_bb.z + 0.5f - offset.z, DoorE) )
                player_max_bb.x = min_bb.x;
        }
        */
        if ( max_bb.x < player_max_bb.x ) //WEST
        {
            //std::cout << "MAX HIT WEST" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.z + 0.5f - offset.z, player_max_bb.y, player_max_bb.z + 0.5f - offset.z, DoorW) )
                player_max_bb.x = max_bb.x;
        }
        /*
        if ( player_max_bb.z < min_bb.z ) //SOUTH
        {
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.x + 0.5f - offset.x, player_max_bb.y, player_max_bb.x + 0.5f - offset.x, DoorS) )
                player_max_bb.z = min_bb.z;
        }
        */
        if ( max_bb.z < player_max_bb.z ) //NORTH
        {
            //std::cout << "MAX HIT NORTH" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.x + 0.5f - offset.x, player_max_bb.y, player_max_bb.x + 0.5f - offset.x, DoorN) )
                player_max_bb.z = max_bb.z;
        }

        if ( max_bb.y < player_max_bb.y ) //CEILING
        {
            player_max_bb.y = max_bb.y;
        }
    }
}

std::string Room::handleBoxObject(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb)
{

    if ( update == "on top" )
        if( max_bb.y - 0.2f < player_min_bb.y && player_min_bb.y < max_bb.y )
        {   
            player_min_bb.y = max_bb.y;
            return "max";  
        }
        else
            return "None";

    if (update == "below")
        if( min_bb.y < player_max_bb.y && player_max_bb.y < min_bb.y + 0.2f )
        {
            player_max_bb.y = min_bb.y; 
            return "min"; 
        }
        else
            return "None";

    if ( update == "min min" )
    {
        float distx = -1.0f;
        float distz = -1.0f;
        
        if( min_bb.x < player_min_bb.x && player_min_bb.x < max_bb.x )
            distx = max_bb.x - player_min_bb.x;

        if( min_bb.z < player_min_bb.z && player_min_bb.z < max_bb.z )
            distz = max_bb.z - player_min_bb.z;

        if( distx != -1.0f && distz != -1.0f )
        {
            if( distz < distx)
                player_min_bb.z = max_bb.z;
            else if( distx < distz )
                player_min_bb.x = max_bb.x;
            else// if ( distx != -1.0f && distz != -1.0f )
            {
                player_min_bb.x = max_bb.x;
                player_min_bb.z = max_bb.z;
            }
            return "max";
        }       
        return "None";
    }

    else if ( update == "max max" )
    {
        float distx = -1.0f;
        float distz = -1.0f;

        if( min_bb.x < player_max_bb.x && player_max_bb.x < max_bb.x)
            distx = player_max_bb.x - min_bb.x;

        if( min_bb.z < player_max_bb.z && player_max_bb.z < max_bb.z)
            distz = player_max_bb.z - min_bb.z;

        if( distx != -1.0f && distz != -1.0f )
        {
            if( distz < distx)
                player_max_bb.z = min_bb.z;
            else if( distx < distz )
                player_max_bb.x = min_bb.x;
            else //if ( distx != -1.0f && distz != -1.0f )
            {
                player_max_bb.x = min_bb.x;
                player_max_bb.z = min_bb.z;
            }
            return "min";
        }
        return "None";
    }


    else if ( update == "min max" )
    {
        float distx = -1.0f;
        float distz = -1.0f;

        if( min_bb.x < player_min_bb.x && player_min_bb.x < max_bb.x )
            distx = max_bb.x - player_min_bb.x;

        if( min_bb.z < player_max_bb.z && player_max_bb.z < max_bb.z)
            distz = player_max_bb.z - min_bb.z;

        if ( distx != -1.0f && distz != 1.0f )
        {
            if (distx < distz)
            {
                player_min_bb.x = max_bb.x;
                return "max";
            }
            else if (distz < distx)
            {
                player_max_bb.z = min_bb.z;
                return "min";
            }
            else
            {
                player_min_bb.x = max_bb.x;
                player_max_bb.z = min_bb.z;
                return "both";
            }
        }
        return "None";
    }

    else if ( update == "max min" )
    {
        float distx = -1.0f;
        float distz = -1.0f;

        if( min_bb.x < player_max_bb.x && player_max_bb.x < max_bb.x )
            distx = min_bb.x - player_max_bb.x;

        if( min_bb.z < player_min_bb.z && player_min_bb.z < max_bb.z)
            distz = player_min_bb.z - max_bb.z;

        if ( distx != -1.0f && distz != 1.0f )
        {
            if (distz < distx)
            {
                player_max_bb.x = min_bb.x;
                return "min";
            }
            else if (distx < distz)
            {
                player_min_bb.z = max_bb.z;
                return "max";
            }
            else
            {
                player_max_bb.x = min_bb.x;
                player_min_bb.z = max_bb.z;
                return "both";
            }

        }

        return "None";
    }

    else
        return "None";
}

bool Room::cameraCollide(Camera &camera)
{
    if (!active && !self_collision) //quick check to return earliest if no colllision allowed
        return false;

    glm::vec3 cam_min_bb = camera.GetMinBB();
    glm::vec3 cam_max_bb = camera.GetMaxBB();

    bool ret = false;

    if( Collision::boxCollide(cam_min_bb, cam_max_bb, room_min_bb, room_max_bb) ) //if player is partially in room and room active
    {
        if (self_collision) //If only one that can collide
        { 

            if ( boxCollide(cam_min_bb, cam_max_bb, room_min_bb, room_max_bb) ) //CHECK WALLS
            {
                ret = true;
                camera.PosFromBB(cam_min_bb, cam_max_bb);
                cam_min_bb = camera.GetMinBB();
                cam_max_bb = camera.GetMaxBB();
            }

            for(int i = 0; i < asset_bb.size(); i++)
                if( boxCollide(cam_min_bb, cam_max_bb, asset_bb[i].min, asset_bb[i].max) ) //CHECK BOXES
                {
                    ret = true;
                    camera.PosFromBB(cam_min_bb, cam_max_bb);
                    cam_min_bb = camera.GetMinBB();
                    cam_max_bb = camera.GetMaxBB();
                }

        }
        else if (active) //take ticket if no one does
        {
            self_collision = true;
            active = false;
            return cameraCollide(camera); //call recursiely
        }
    }
    else //if no collision
    {
        if(self_collision) //yield right to collide
        {
            self_collision = false;
            active = true;
        }
    }

    return ret; 
    
} 


void Room::getLights(Renderer renderer)
{
    if(self_collision) //If I am the room that collides with player, player is inside room
        renderer.SetLights(this);
}