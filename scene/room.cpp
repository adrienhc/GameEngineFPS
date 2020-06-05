#include "room.h"

bool Room::active = true;
bool Room::shadowPass = true;
int Room::sharedID = 0;
float Room::minBulletDist = 1000.0f;
float Room::maxBulletDist = 1000.0f;
float Room::maxRange = 100.0f;

Room::Room(int lgth, int wdth, int hght, glm::vec3 ofst,  std::vector<int> DN,  std::vector<int> DS, std::vector<int> DE, std::vector<int> DW, 
    std::vector<glm::vec3> ptLghtPs, std::vector<float> ptLghtRad, std::vector<asset> vrtcl, std::vector<asset> hrztl, std::vector<asset> trgt, std::vector<asset> strs,
    Asset* flr, Asset* wll, Asset* dr, Asset* bm, Asset* ceil, Asset* crte, PointLight* ptLght)
{
    //ID -- Take current and increment for next rooms
    ID = sharedID;
    sharedID++;

    //Collision Ticket available
    active = true;

    //DIMENSIONS
	length = lgth;
	width = wdth;
	height = hght;
    offset = ofst;

    //ROOM BOUNDING BOX
    room_min_bb = glm::vec3(offset.x - 0.5f, offset.y, offset.z - 0.5f );
    room_max_bb = glm::vec3(room_min_bb.x + width, room_min_bb.y + height, room_min_bb.z + length);
    bounding_box.min = room_min_bb;
    bounding_box.max = room_max_bb;

    //ROOM DOORS
	DoorN = DN;
	DoorS = DS;
	DoorE = DE;
	DoorW = DW;

    //LIGHTNING
    pointLight = ptLght;
    //ROOM LIGHT RAD
    pointLightRad = ptLghtRad;
    //ROOM LIGHTS POS
    pointLightPos = ptLghtPs;
    

    //CRATE CLUSTERS
    vertical = vrtcl;
    horizontal = hrztl;

    //TEXTURES
	floor = flr; 
	wall = wll; 
	door = dr; 
	beam = bm;
	ceiling = ceil;
    crate = crte;

    //OBJECT OUTLINE
    glm::vec4 outline_color = glm::vec4(0.576f, 0.164f, 0.164f, 1.0f);
    float outline_size = 0.05f;

    //TARGETS
    for(int i = 0; i < trgt.size(); i++)
        targets.push_back(new Target(adjustAssetPos(trgt[i].pos), trgt[i].orientation, outline_color, outline_size));


    //STAIRS
    for(int i = 0; i < strs.size(); i++)
        stairs.push_back(new Stairs(adjustAssetPos(strs[i].pos + glm::vec3(-0.5f, -0.5f, -0.5f)), strs[i].x, strs[i].y, strs[i].y, strs[i].orientation));
    
    //PARTICLE
    glm::vec4 col = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f); 
    int r = col.x * 255.0f;
    int g = col.y * 255.0f;
    int b = col.z * 255.0f;
    int a = col.w * 255.0f;
    unsigned int color = ( a << 24 | b << 16 | g << 8 | r );
    baseImpactShort = new Particle(glm::vec4(0.0f), glm::vec3(0.0f), 0.2f, color, 0.2f, 0.03f, 180.0f);

    r = col.x * 255.0f;
    g = col.y * 255.0f;
    b = col.z * 255.0f;
    a = col.w * 255.0f;
    color = ( a << 24 | b << 16 | g << 8 | r );
    baseImpactLong = new Particle(glm::vec4(0.0f), glm::vec3(0.0f), 0.3f, color, 0.7f, 0.03f, 180.0f);    


}

Room::~Room()
{
    sharedID = 0;

    delete Root;
    delete Lights;
    delete pointLight;
    delete baseImpactShort;
    delete baseImpactLong;

    for(int i = 0; i < targets.size(); i++)
        delete targets[i];
    for(int i = 0; i < stairs.size(); i++)
        delete stairs[i];

    delete g_light;
    delete g_layout;
    delete g_tiles;
    delete g_cubes;
}

glm::vec3 Room::adjustAssetPos(glm::vec3 Pos)
{
    return glm::vec3(offset.x + Pos.x, offset.y + 0.5f + Pos.y, offset.z + Pos.z);
} 

BB Room::getBoundingBox()
{
    return bounding_box;
}

void Room::makeRoom() //MAKES SCENE GRAPH, AND INSTANTIATES ROOM'S GEOMETRY
{
    std::vector<nNode*> Roots;

    Root = new nNode();
    Lights = new nNode();

    std::vector<nNode*> Tiles;
    std::vector<nNode*> Cubes;

    //LIGHTS
    nNode* Ref = Root;
    nNode* RefBeam = NULL;
    nNode* RefLights = Lights;

    for(int i = 0; i < pointLightPos.size(); i++)
    {
        pointLight->setRadius(pointLightRad[i], i);
        Ref = Root->AddChildrenRecursive(new nTranslate(glm::vec3(offset.x - 0.5f + pointLightPos[i].x, offset.y - 0.1f + pointLightPos[i].y, offset.z - 0.5f + pointLightPos[i].z)));
        RefLights = Lights->AddChildrenRecursive(new nTranslate(glm::vec3(offset.x - 0.5f + pointLightPos[i].x, offset.y - 0.1f + pointLightPos[i].y, offset.z - 0.5f + pointLightPos[i].z)));
        Ref->AddChildren(new nPointLight(pointLight, i));
        RefLights->AddChildren(new nPointLight(pointLight, i));
    }

    g_light = new Group(Lights);

    //FLOOR AND CEILING
    nNode* Instanciated = new nNode();
    Ref = Instanciated->AddChildrenRecursive(new nRotate(glm::radians(270.0f), glm::vec3(1.0f, 0.0f, 0.0f), true)); //FLOOR
    Ref = horizontalPlaneLowpoly(Ref, floor);
    //Ref = horizontalPlane(Ref, floor);
    Ref = Instanciated;
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(1.0f, 0.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, (float) height, 0.0f))); //CEILING
    Ref = horizontalPlaneLowpoly(Ref, ceiling);
    //Ref = horizontalPlane(Ref, ceiling);

    Roots.push_back(Instanciated);
    Tiles.push_back(Instanciated);

    //SEPARATE WALLS AND BEAMS
    Instanciated = new nNode();
    Ref = Instanciated->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.5f, -0.5f))); //SOUTH
    Ref = verticalPlaneNSLowpoly(Ref, 0.0f, (float)width, 0.0f, (float)height, DoorS);
    //Ref = verticalPlaneNS(Ref, DoorS, false);
    nNode* InstanciatedBeam = new nNode();
    RefBeam = InstanciatedBeam->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.5f, -0.5f))); //SOUTH
    RefBeam = verticalPlaneNS(RefBeam, DoorS, true);

    Ref = Instanciated;
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.5f, -0.5f))); 
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, (float) length))); //NORTH
    Ref = verticalPlaneNSLowpoly(Ref, 0.0f, (float)width, 0.0f, (float)height, DoorN);
    //Ref = verticalPlaneNS(Ref, DoorN, false);
    RefBeam = InstanciatedBeam;
    RefBeam = RefBeam->AddChildrenRecursive(new nRotate(glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    RefBeam = RefBeam->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.5f, -0.5f))); 
    RefBeam = RefBeam->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, (float) length))); //NORTH
    RefBeam = verticalPlaneNS(RefBeam, DoorN, true);
    

    Ref = Instanciated->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(-0.5f, 0.5f, 0.0f))); //EAST
    Ref = verticalPlaneEWLowpoly(Ref, 0.0f, (float)length, 0.0f, (float)height, DoorE);
    //Ref = verticalPlaneEW(Ref, DoorE, false);
    RefBeam = InstanciatedBeam->AddChildrenRecursive(new nRotate(glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    RefBeam = RefBeam->AddChildrenRecursive(new nTranslate(glm::vec3(-0.5f, 0.5f, 0.0f))); //EAST
    RefBeam = verticalPlaneEW(RefBeam, DoorE, true);

    Ref = Instanciated;
    Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(-0.5f, 0.5f, 0.0f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3((float) width, 0.0f, 0.0f))); //WEST
    Ref = verticalPlaneEWLowpoly(Ref, 0.0f, (float)length, 0.0f, (float)height, DoorW);
    //Ref = verticalPlaneEW(Ref, DoorW, false);
    RefBeam = InstanciatedBeam;
    RefBeam = RefBeam->AddChildrenRecursive(new nRotate(glm::radians(270.0f), glm::vec3(0.0f, 1.0f, 0.0f), true));
    RefBeam = RefBeam->AddChildrenRecursive(new nTranslate(glm::vec3(-0.5f, 0.5f, 0.0f)));
    RefBeam = RefBeam->AddChildrenRecursive(new nTranslate(glm::vec3((float) width, 0.0f, 0.0f))); //WEST
    RefBeam = verticalPlaneEW(RefBeam, DoorW, true);

    Roots.push_back(Instanciated);
    Roots.push_back(InstanciatedBeam);
    Tiles.push_back(Instanciated);
    Cubes.push_back(InstanciatedBeam);
    
    //CRATES
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

    Roots.push_back(Instanciated);
    Cubes.push_back(Instanciated);

    //Add Stairs to Groups
    for(int i = 0; i < stairs.size(); i++)
    {
        Roots.push_back(stairs[i]->Root);
        Cubes.push_back(stairs[i]->Root);
    }

    g_layout = new Group(Roots);
    g_tiles = new Group(Tiles);
    g_cubes = new Group(Cubes);
}




//LEAST AMOUNT OF RECTANGLES
nNode* Room::horizontalPlaneLowpoly(nNode* Root, Asset* asset) //floor or ceiling
{
    nNode* Ref = Root;
    Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(width, length, 1.0f))); 
    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(width/2.0f - 0.5f, 0.0f, length/2.0f - 0.5f)));
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
    Ref = Ref->AddChildrenRecursive(new nAsset(asset, glm::vec2(width, length), eInstanced)); //before eObject        

    return Root;
}

//ADD DOORS AS ONE BLOCK 
nNode* Room::verticalDoorNSLowpoly(nNode* Root, std::vector<int> Door)
{
    for(int i = 0; i < Door.size(); i+=doorSpec)
    {
        if(Door[i+4] == -1) //Specified no Door
            continue;

        float width = Door[i+2];
        float height = Door[i+3] - Door[i+4];

        nNode* Ref = Root;
        Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(width, height, 1.0f))); 
        Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(width/2.0f + Door[i+0] - 0.5f, height/2.0f + Door[i+1] + Door[i+4] - 0.5f, 0.0f)));
        Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
        Ref = Ref->AddChildrenRecursive(new nAsset(door, glm::vec2(width, height), eInstanced)); //before eObject
    }        
}

nNode* Room::verticalDoorEWLowpoly(nNode* Root, std::vector<int> Door)
{
    for(int i = 0; i < Door.size(); i+=doorSpec)
    {
        if(Door[i+4] == -1) //Specified no Door
            continue;

        float width = Door[i+2];
        float height = Door[i+3] - Door[i+4];

        nNode* Ref = Root;
        Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(width, height, 1.0f))); 
        Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, height/2.0f + Door[i+1] + Door[i+4] - 0.5f, width/2.0f + Door[i+0] - 0.5f)));
        Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
        Ref = Ref->AddChildrenRecursive(new nAsset(door, glm::vec2(width, height), eInstanced)); //before eObject
    }        
}


nNode* Room::verticalPlaneNSLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<int> Door)
{
    std::vector<glm::vec4> dim;
    for(int i = 0; i < Door.size(); i+=doorSpec)
    {
        dim.push_back(glm::vec4(Door[i+0], Door[i+0] + Door[i+2], Door[i+1], Door[i+1] + Door[i+3]));
    }
    std::sort(dim.begin(), dim.end(), Vec4ComparisonFunction);

    Root = verticalWallNSLowpoly(Root, min_w, max_w, min_h, max_h, dim); 
    Root = verticalDoorNSLowpoly(Root, Door);

    return Root;

}

nNode* Room::verticalWallNSLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<glm::vec4> dim)
{
    
    if(min_w == max_w || min_h == max_h)
        return Root;

    //Check if door Within bounds
    bool foundDoor = false;
    for(int i = 0; i < dim.size(); i++)
    {
        //Find if door intersects
        if( Collision::boxCollide(glm::vec2(dim[i].x, dim[i].z), glm::vec2(dim[i].y, dim[i].w), glm::vec2(min_w, min_h), glm::vec2(max_w, max_h)))
        {
        
            foundDoor = true;
            //Divide W -- No Door -- Include all Height
            Root = verticalWallNSLowpoly(Root, min_w, dim[i].x, min_h, max_h, dim);
            Root = verticalWallNSLowpoly(Root, dim[i].y, max_w, min_h, max_h, dim);
            //Divide H -- Do not include Width
            Root = verticalWallNSLowpoly(Root, dim[i].x, dim[i].y, min_h, dim[i].z, dim);
            Root = verticalWallNSLowpoly(Root, dim[i].x, dim[i].y, dim[i].w, max_h, dim);
            break;
        }
    }

    if(!foundDoor)
    {
        float width = max_w - min_w;
        float height = max_h - min_h;
        nNode*Ref = Root;
        Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(width, height, 1.0f)));
        Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(min_w + width/2.0f - 0.5f, min_h + height/2.0f - 0.5f, 0.0f)));
        Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
        Ref = Ref->AddChildrenRecursive(new nAsset(wall, glm::vec2(width, height), eInstanced));
    }

    return Root;
}


nNode* Room::verticalPlaneEWLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<int> Door)
{
    std::vector<glm::vec4> dim;
    for(int i = 0; i < Door.size(); i+=doorSpec)
    {
        dim.push_back(glm::vec4(Door[i+0], Door[i+0] + Door[i+2], Door[i+1], Door[i+1] + Door[i+3]));
    }
    std::sort(dim.begin(), dim.end(), Vec4ComparisonFunction);

    Root = verticalWallEWLowpoly(Root, min_w, max_w, min_h, max_h, dim); 
    Root = verticalDoorEWLowpoly(Root, Door);

    return Root;

}

nNode* Room::verticalWallEWLowpoly(nNode* Root, float min_w, float max_w, float min_h, float max_h, std::vector<glm::vec4> dim)
{
    
    if(min_w == max_w || min_h == max_h)
        return Root;

    //Check if door Within bounds
    bool foundDoor = false;
    for(int i = 0; i < dim.size(); i++)
    {
        //Find if door intersects
        if( Collision::boxCollide(glm::vec2(dim[i].x, dim[i].z), glm::vec2(dim[i].y, dim[i].w), glm::vec2(min_w, min_h), glm::vec2(max_w, max_h)))
        {

            foundDoor = true;
            //Divide W -- No Door -- Include all Height
            Root = verticalWallEWLowpoly(Root, min_w, dim[i].x, min_h, max_h, dim);
            Root = verticalWallEWLowpoly(Root, dim[i].y, max_w, min_h, max_h, dim);
            //Divide H -- Do not include Width
            Root = verticalWallEWLowpoly(Root, dim[i].x, dim[i].y, min_h, dim[i].z, dim);
            Root = verticalWallEWLowpoly(Root, dim[i].x, dim[i].y, dim[i].w, max_h, dim);
            break;
        }
    }

    if(!foundDoor)
    {
        float width = max_w - min_w;
        float height = max_h - min_h;
        nNode*Ref = Root;
        Ref = Ref->AddChildrenRecursive(new nScale(glm::vec3(width, height, 1.0f)));
        Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, min_h + height/2.0f - 0.5f, min_w + width/2.0f - 0.5f)));
        Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
        Ref = Ref->AddChildrenRecursive(new nAsset(wall, glm::vec2(width, height), eInstanced));
    }

    return Root;
}

//ONE SQUARE PER TILE
nNode* Room::horizontalPlane(nNode* Root, Asset* asset) //floor or ceiling
{
    nNode* Ref = Root;

    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));

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

nNode* Room::verticalPlaneNS(nNode* Root, std::vector<int> Door, bool justBeams)
{
    nNode* Ref = Root;
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));

    if(!justBeams)
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < width; j++)
            {
                if( !isDoor(i, j, Door) )
                    Ref = Ref->AddChildrenRecursive(new nAsset(wall, eInstanced));
                else
                {
                    if( !pointOpeningDoor(i, j, Door, true) )                      
                        Ref = Ref->AddChildrenRecursive(new nAsset(door, eInstanced));    
                }

                if(j < width-1)
                    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(1.0f, 0.0f, 0.0f)));
            }
            
            Ref = Ref->AddChildrenRecursive(new nTranslate (glm::vec3(-1.0f * (width - 1), 1.0f, 0.0f)));
        }
    }
    else
    {
        for(int i = 0; i < Door.size(); i+=doorSpec)
        {
            if(hasBeam(Door[i+5]))
            {
               std::vector<int> mDoor(Door.begin()+i, Door.begin()+i+4);
               Root = addBeamNS(Ref, mDoor);
            }
        }
    }
   
    return Root;
}

nNode* Room::verticalPlaneEW(nNode* Root, std::vector<int> Door, bool justBeams)
{
    nNode* Ref = Root;
    Ref = Ref->AddChildrenRecursive(new nTranslate(offset));

    if(!justBeams)
    {
        for(int i = 0; i < height; i++)
        {
            for(int j = 0; j < length; j++)
            {
                if( !isDoor(i, j, Door) )
                    Ref = Ref->AddChildrenRecursive(new nAsset(wall, eInstanced));
                else
                {
                    if( !pointOpeningDoor(i, j, Door, true) )                      
                        Ref = Ref->AddChildrenRecursive(new nAsset(door, eInstanced));
                    
                }
                if(j < length-1)
                    Ref = Ref->AddChildrenRecursive(new nTranslate(glm::vec3(0.0f, 0.0f, 1.0f)));
            }
            
            Ref = Ref->AddChildrenRecursive(new nTranslate (glm::vec3(0.0f, 1.0f, -1.0f * (length - 1))));
        }    
    }
    else
    {
        for(int i = 0; i < Door.size(); i+=doorSpec)
        {
            if(hasBeam(Door[i+5]))
            {
               std::vector<int> mDoor(Door.begin()+i, Door.begin()+i+4);
               Root = addBeamEW(Ref, mDoor);
            }
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

bool Room::pointOpeningDoor(int height, int width, std::vector<int> Door, bool contruction) //RETURN HEIGHT DOOR
{
    float offset = 0.0f;
    if(!contruction)
        offset = room_min_bb.y;
    
    for(int i = 0; i < Door.size(); i+=doorSpec)   
        if( (offset + Door[i+1] <= height) && (height < offset + Door[i+1] + Door[i+3]) && ((Door[i+0] <= width) && (width < Door[i+0] + Door[i+2])) )
        {
            if(Door[i+4] == -1) //-1 to specify No Door
                return true;
            else
                return  height < (offset + Door[i+1] + Door[i+4]);   //current sampled height < y offset + opening height
        }

    return false;
}


bool Room::boxOpeningDoor(int ref_height, int ref_width, int other_height, int other_width, std::vector<int> Door) //ONLY LET THROUGH IF SAME DOOR
{
    for(int i = 0; i < Door.size(); i+=doorSpec)
    {   
        if( (room_min_bb.y + Door[i+1] <= ref_height) && (ref_height < room_min_bb.y + Door[i+1] + Door[i+3]) && ((Door[i+0] <= ref_width) && (ref_width < Door[i+0] + Door[i+2])) &&         // MIN AND
            (room_min_bb.y + Door[i+1] <= other_height) && (other_height < room_min_bb.y + Door[i+1] + Door[i+3]) && ((Door[i+0] <= other_width) && (other_width < Door[i+0] + Door[i+2])) )  // MAX
        {
            if(Door[i+4] == -1) //-1 to specify No Door
            {
                return true;
            }
            else
            {
                return  (ref_height < (room_min_bb.y + Door[i+1] + Door[i+4])) && (other_height < (room_min_bb.y + Door[i+1] + Door[i+4]));   //current sampled height < y offset + opening height
            }
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



bool Room::boxCollide(glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb, bool ignore_top)
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

            if(!ignore_top)
            {
                update = handleBoxObject("on top", player_min_bb, player_max_bb, min_bb, max_bb);
                if (update == "max")
                {
                    //std::cout << "ont top" << std::endl;
                    player_max_bb = glm::vec3(player_min_bb.x + dist_bb.x, player_min_bb.y + dist_bb.y, player_min_bb.z + dist_bb.z);
                    return true;
                }
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
        if (player_min_bb.y < min_bb.y) //FLOOR
            player_min_bb.y = min_bb.y;

        if ( player_min_bb.x < min_bb.x ) //EAST
        {
            // std::cout << "MIN HIT EAST" << std::endl;
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
            {
                player_min_bb.z = min_bb.z;
            }

        }
        /*
        if ( max_bb.z < player_min_bb.z) //NORTH
        {
            std::cout << "MIN HIT NORTH" << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.x + 0.5f - offset.x, player_max_bb.y, player_max_bb.x + 0.5f - offset.x, DoorN) )
                player_min_bb.z = max_bb.z;
        }
        */
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

        if ( max_bb.y < player_max_bb.y ) //CEILING
        {
            player_max_bb.y = max_bb.y;
        }

        if ( max_bb.x < player_max_bb.x ) //WEST
        {
            // std::cout << "MAX HIT WEST" << std::endl;
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
            //std::cout << "MAX HIT NORTH" << glm::to_string(player_max_bb) << std::endl;
            if ( !boxOpeningDoor(player_min_bb.y, player_min_bb.x + 0.5f - offset.x, player_max_bb.y, player_max_bb.x + 0.5f - offset.x, DoorN) )
            {
                player_max_bb.z = max_bb.z;
            }

        }
    }
}

std::string Room::handleBoxObject(std::string update, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb, glm::vec3 min_bb, glm::vec3 max_bb)
{

    if ( update == "on top" )
        if( max_bb.y - 0.45f < player_min_bb.y && player_min_bb.y < max_bb.y )
        {   
            player_min_bb.y = max_bb.y;
            return "max";  
        }
        else
            return "None";

    if (update == "below")
        if( min_bb.y < player_max_bb.y && player_max_bb.y < min_bb.y + 0.45f )
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

bool Room::collisionChecks(Camera &camera)
{
    //std::cout << "T S = " << targets.size() << std::endl;
    bool camCheck = cameraCollide(camera);
    bool bulletCheck = false;
    if(Weapon::newBullet)
        bulletCheck = bulletCollide(camera);
    return bulletCheck;
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

            for(int i = 0; i < targets.size(); i++) //CHECK TARGETS
            {
                if(targets[i]->IsShot()) //if target already shot, ignore collision box
                    continue;

                if(boxCollide(cam_min_bb, cam_max_bb, targets[i]->body_low_bb.min, targets[i]->body_low_bb.max)) //Low Body
                {
                    ret = true;
                    camera.PosFromBB(cam_min_bb, cam_max_bb);
                    cam_min_bb = camera.GetMinBB();
                    cam_max_bb = camera.GetMaxBB();   
                }
                
                if(boxCollide(cam_min_bb, cam_max_bb, targets[i]->body_high_bb.min, targets[i]->body_high_bb.max)) //High Body
                {
                    ret = true;
                    camera.PosFromBB(cam_min_bb, cam_max_bb);
                    cam_min_bb = camera.GetMinBB();
                    cam_max_bb = camera.GetMaxBB();   
                    
                }

                if(boxCollide(cam_min_bb, cam_max_bb, targets[i]->head_bb.min, targets[i]->head_bb.max)) //Head 
                {
                    ret = true;
                    camera.PosFromBB(cam_min_bb, cam_max_bb);
                    cam_min_bb = camera.GetMinBB();
                    cam_max_bb = camera.GetMaxBB();   
                }
            }

            glm::vec3 prev_cam_min_bb = cam_min_bb;
            glm::vec3 prev_cam_max_bb = cam_max_bb;
            if ( boxCollide(cam_min_bb, cam_max_bb, room_min_bb, room_max_bb) ) //CHECK FLOOR -- ONLY UPDATE Y
            {
                ret = true;
                cam_min_bb.x = prev_cam_min_bb.x;
                cam_min_bb.z = prev_cam_min_bb.z;

                cam_max_bb.x = prev_cam_max_bb.x;
                cam_max_bb.z = prev_cam_max_bb.z;

                camera.PosFromBB(cam_min_bb, cam_max_bb);
                cam_min_bb = camera.GetMinBB();
                cam_max_bb = camera.GetMaxBB();
            }


            //CHECK STAIRS AS A WHOLE
            for(int i = 0; i < stairs.size(); i++)
            {
                
                if( Collision::boxCollide(cam_min_bb, cam_max_bb, stairs[i]->stairs_bb.min, stairs[i]->stairs_bb.max) ) //CHECK STAIRS
                {
                    ret = true;
                    bool height_set = stairs[i]->SetHeight(camera.Position, cam_min_bb, cam_max_bb);

                    if(!height_set)
                    {
                        boxCollide(cam_min_bb, cam_max_bb, stairs[i]->stairs_bb.min, stairs[i]->stairs_bb.max, true);
                    }

                    camera.PosFromBB(cam_min_bb, cam_max_bb);
                    cam_min_bb = camera.GetMinBB();
                    cam_max_bb = camera.GetMaxBB();

                }
            }

            for(int i = 0; i < asset_bb.size(); i++)
                if( boxCollide(cam_min_bb, cam_max_bb, asset_bb[i].min, asset_bb[i].max) ) //CHECK BOXES
                {
                    ret = true;
                    camera.PosFromBB(cam_min_bb, cam_max_bb);
                    cam_min_bb = camera.GetMinBB();
                    cam_max_bb = camera.GetMaxBB();
                }

            if ( boxCollide(cam_min_bb, cam_max_bb, room_min_bb, room_max_bb) ) //CHECK WALLS
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


bool Room::bulletCollide(Camera &camera) 
{
    bool hit = false;
    float placeholder;
    float targDist = 1000.0f; //always assume target way too far away
    float boxDist = 1000.0f;
    float wallDist = 1000.0f;
    float stairsDist = 1000.0f;

    //reset tempDist within targets 
    for(int i = 0; i < targets.size(); i++)
    {
        targets[i]->ResetDist(maxBulletDist);
        targets[i]->SetHitRay(false);
    }

    //Intersect targets first
    if(Weapon::fire)
    {
        for(int i = 0; i < targets.size(); i++)
        {
            if(targets[i]->IsShot()) //if target already shot, ignore collision box
                continue;

            if(Collision::rayBoxCollide(camera.Position, camera.Front, targets[i]->body_low_bb.min, targets[i]->body_low_bb.max, true, placeholder)) //Low Body
            {
                targDist = placeholder < maxRange ? placeholder : 1000.0f; 
                targets[i]->ResetDist(targDist);
                targets[i]->SetHitRay(true);
            }
            
            else if(Collision::rayBoxCollide(camera.Position, camera.Front, targets[i]->body_high_bb.min, targets[i]->body_high_bb.max, true, placeholder)) //High Body
            {
                 targDist = placeholder < maxRange ? placeholder : 1000.0f; 
                 targets[i]->ResetDist(targDist);
                 targets[i]->SetHitRay(true); 
            }

            else if(Collision::rayBoxCollide(camera.Position, camera.Front, targets[i]->head_bb.min, targets[i]->head_bb.max, true, placeholder)) //Head 
            {
                targDist = placeholder < maxRange ? placeholder : 1000.0f; 
                targets[i]->ResetDist(targDist);
                targets[i]->SetHitRay(true);
            }

            if(0.0 <= targDist)
                minBulletDist = targDist < minBulletDist ? targDist : minBulletDist;
        }
    }

    if(Weapon::fire)
    {
        for(int i = 0; i < asset_bb.size(); i++)
        {
            if(Collision::rayBoxCollide(camera.Position, camera.Front, asset_bb[i].min, asset_bb[i].max, true, placeholder)) //Boxes
            {
                if(placeholder >= 0 &&  placeholder < boxDist)
                    boxDist = placeholder;
            }

            minBulletDist = boxDist < minBulletDist ? boxDist : minBulletDist;
        }
    }

    if(Weapon::fire)
    {
        float dummy_placeholder;
        for(int i = 0; i < stairs.size(); i++)
        {
            if(Collision::rayBoxCollide(camera.Position, camera.Front, stairs[i]->stairs_bb.min, stairs[i]->stairs_bb.max, true, dummy_placeholder)) //Stairs Box
            {
                for(int k = 0; k < stairs[i]->steps_bb.size(); k++)
                {
                    if(Collision::rayBoxCollide(camera.Position, camera.Front, stairs[i]->steps_bb[k].min, stairs[i]->steps_bb[k].max, true, placeholder)) //Steps Box
                    {
                        if(placeholder >= 0 &&  placeholder < stairsDist)
                        stairsDist = placeholder;
                    }
                }
            }

            minBulletDist = stairsDist < minBulletDist ? stairsDist : minBulletDist;
        }
    }

    //if firing and hit target, find if wall in front of it
    if(Weapon::fire)
    {
       
       if(Collision::rayBoxCollide(camera.Position, camera.Front, room_min_bb, room_max_bb, false, placeholder))
       {
            bool wallHit = false;
            
            if(0 <= placeholder)
                wallDist = placeholder;

            if(wallDist < minBulletDist) //targDist)
            {
                glm::vec3 intersection_point = camera.Position + wallDist * camera.Front;

                if ( intersection_point.x == room_min_bb.x ) //EAST
                {
                    if ( !pointOpeningDoor(intersection_point.y, intersection_point.z + 0.5f - offset.z, DoorE) )
                        wallHit = true;
                }

                else if ( room_max_bb.x == intersection_point.x ) //WEST
                {
                    if ( !pointOpeningDoor(intersection_point.y, intersection_point.z + 0.5f - offset.z, DoorW) )
                        wallHit = true;
                }

                else if ( intersection_point.z == room_min_bb.z ) //SOUTH
                {
                    if ( !pointOpeningDoor(intersection_point.y, intersection_point.x + 0.5f - offset.x, DoorS) )
                        wallHit = true;
                }

                else if ( room_max_bb.z == intersection_point.z ) //NORTH
                {
                    if ( !pointOpeningDoor(intersection_point.y, intersection_point.x + 0.5f - offset.x, DoorN) )
                        wallHit = true;
                }

                else if (intersection_point.y == room_min_bb.y) //FLOOR
                {
                    wallHit = true;
                }

                else if (intersection_point.y == room_max_bb.y) //CEILING
                {
                    wallHit = true;
                }

                // std::cout << ID << " " << glm::to_string(intersection_point) << " " << wallHit << std::endl;

                if(wallHit) //if wall in front of ray 
                {
                    minBulletDist = wallDist < minBulletDist ? wallDist : minBulletDist;
                }
            }
       }
    }
    return hit;
}

void Room::setupCollisions()
{
    minBulletDist = 1000.0f;
    shadowPass = false;
}

void Room::postCollisions()
{
    //CHECK IF TARGET HAS BEEN SHOT SINCE LONGER THAN ITS LIFESPAN
    for(int i = 0; i < targets.size(); i++)
    {
        //if one target is shot, will be true, need to update shadow map for as long as shot and before deleted 
        //if(Weapon::fire)
         //   std::cout << minBulletDist << std::endl;

        if(targets[i]->IsShot())  //No need keep Target for whole explosion time, just submit to particle system later and delete
            targets.erase(targets.begin()+i);

        if(Weapon::newBullet)
        {   //if tagrget shot that had not been shot before, shadow pass 
            if(!targets[i]->IsShot() && targets[i]->TestDist(minBulletDist) && targets[i]->HitRay())
            {
                targets[i]->Shot();
                shadowPass = true;
            }
        }

    }
}


void Room::getLights(Renderer renderer)
{
    if(self_collision) //If I am the room that collides with player, player is inside room
        renderer.GetLights(this);
}


void Room::addLayoutLayer(AbstractLayer* layer)
{
    layer->Add(g_layout);
}

void Room::addLayoutLayerCull(AbstractLayer* layer)
{
        //Submit layout under strict conditions -- fustrum culling   
    if(layer->IsCulled(bounding_box))
    {
        return;
    }
    layer->Add(g_layout);
}

void Room::addCubesLayer(InstancedLayer* instanced_layer)
{
    instanced_layer->Add(g_cubes);
}

void Room::addTilesLayer(InstancedLayer* instanced_layer)
{
    instanced_layer->Add(g_tiles);
}

void Room::addLightsLayer(SceneLayer* scene, bool renderable)
{
    scene->AddLight(g_light, renderable);
}

void Room::addLightsLayerCull(SceneLayer* scene, bool renderable)
{
    //Always submit light, or under lax conditions -- room and distance from player?
    if(scene->IsCulled(bounding_box))
    {
        return;
    }

    scene->AddLight(g_light, renderable);
}

void Room::addTargetsLayer(AbstractLayer* model_layer, AbstractLayer* outline_layer, AbstractLayer* particle_layer, AbstractLayer* depthmap_layer)
{
    nNode* TargetsDepthmap = new nNode();
    nNode* TargetsParticle = new nNode();
    nNode* TargetsOutline = new nNode();
    nNode* TargetsRender = new nNode();

    bool Shot = false;

    for(int i = 0; i < targets.size(); i++)
    {
        nModel* targetModel = targets[i]->GetNodeModel(); 
        //Explode Group
        if(targets[i]->IsShot()) 
        {
            if(particle_layer) //&& targetModel->GetShader() == eExplode)
            {
                Shot = true;
                TargetsParticle->AddChildren(targetModel);
            }
        }        
        else
        {
            //Outline Group
            if(outline_layer && targetModel->HasOutline())
            {
                targetModel->EnableOutline();
                TargetsOutline->AddChildren(targetModel);
            }

            //Regular Model
            if(model_layer)
                TargetsRender->AddChildren(targetModel);
            if(depthmap_layer)
                TargetsDepthmap->AddChildren(targetModel);
        }
    }

    if(particle_layer && Shot)
    {
        Group* g_TargetsParticle = new Group(TargetsParticle);
        particle_layer->Add(g_TargetsParticle);
    }

    //PUSH OUTLINES FIRST!!! -- disables outline for NodeModel when pushed
    if(outline_layer) //if add outline, add model at the same time too?
    {
        Group* g_TargetsOutline = new Group(TargetsOutline);
        outline_layer->Add(g_TargetsOutline);
    }

    //PUSH MODELS LAST
    if(model_layer)
    {
        Group* g_TargetsRender = new Group(TargetsRender);
        model_layer->Add(g_TargetsRender);
    }

    if(depthmap_layer)
    {
        Group* g_TargetsDepthmap = new Group(TargetsDepthmap);
        depthmap_layer->Add(g_TargetsDepthmap);
    }
}

void Room::addBulletImpact(Camera* camera, SceneLayer* layer, ParticleSystem* particle_system)
{
    if(minBulletDist <= maxRange)
    {
        glm::vec3 bulletImpact = camera->Position + glm::normalize(camera->Front) * minBulletDist;

        layer->AddBulletHole(bulletImpact);
        
        Particle* base = new Particle(baseImpactShort, glm::vec4(bulletImpact, 1.0f), -1.0f * camera->Front);
        particle_system->Add(base, pLine, 10);

        base = new Particle(baseImpactLong, glm::vec4(bulletImpact, 1.0f), -1.0f * camera->Front);
        particle_system->Add(base, pLine, 10);

    }
}