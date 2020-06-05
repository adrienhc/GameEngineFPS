#include "stairs.h"

Stairs::Stairs(glm::vec3 bottom_left, float width, float length, float height, char orientation)
{
	this->orientation = orientation;
	this->length = length;
	this->height = height;
	//Target Model Node
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);

	glm::vec3 temp_min;
    glm::vec3 temp_max;
	//glm::mat4 transform = glm::mat4(1.0f);
	//bottom_left = glm::vec3(bottom_left.x - 0.5f, bottom_left.y, bottom_left.z - 0.5f);
	//transform = glm::translate(transform, bottom_left);
	
	glm::vec3 offset = glm::vec3(-width/2.0f, 0.0f, length/2.0f);
	float rotation_dir;
	switch (orientation) // LOADED ALIGNED WITH N
	{
		case 'E':
			rotation_dir = -90.0f;
		break;
		case 'W':
			rotation_dir = 90.0f;
		break;
		case 'S':
			rotation_dir = 180.0f;
		break;
		default: //W
			rotation_dir = 0.0f;
		break;
	}


	step = new Asset(eCube, "step", glm::vec3(0.5f), glm::vec3(0.6f), glm::vec3(0.2f), 32.0f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f), false, "");
	Root = new nNode();
	nNode* Ref;

	float step_height_factor = 0.25f; 
	for(float i = 0.0f; i < height; i+=0.25f)
	{
		glm::vec3 scaling = glm::vec3(width, step_height_factor, length - i);
		glm::vec3 translate = glm::vec3(0.0f, step_height_factor/2.0f + i, i/2.0f);
		glm::vec3 temp_offset;

		//GEOMETRY RENDERED
		Ref = Root;
		Ref = Ref->AddChildrenRecursive(new nScale(scaling));
		Ref = Ref->AddChildrenRecursive(new nTranslate(translate));
		Ref = Ref->AddChildrenRecursive(new nTranslate(offset));
		Ref = Ref->AddChildrenRecursive(new nRotate(glm::radians(rotation_dir), Up, true));
		Ref = Ref->AddChildrenRecursive(new nTranslate(bottom_left));
		Ref->AddChildren(new nAsset(step, eInstanced));

		//STEP BOUNDING BOXES
		switch(orientation) //Facing N by default
	    {
	        case 'E':
	            temp_min = glm::vec3(-0.5f, -0.5f, 0.5f);
	            temp_min *= scaling;
	            temp_min = glm::rotate(temp_min, glm::radians(-90.0f), Up);

	            temp_max = glm::vec3(0.5f, 0.5f, -0.5f);
	            temp_max *= scaling;
	            temp_max = glm::rotate(temp_max, glm::radians(-90.0f), Up);      

	            temp_offset = offset + translate;
	            temp_offset = glm::rotate(temp_offset, glm::radians(-90.0f), Up);          
	        break;
	        case 'W':
	            temp_min = glm::vec3(0.5f, -0.5f, -0.5f);
	            temp_min *= scaling;
	            temp_min = glm::rotate(temp_min, glm::radians(90.0f), Up);

	            temp_max = glm::vec3(-0.5f, 0.5f, 0.5f);
	            temp_max *= scaling;
	            temp_max = glm::rotate(temp_max, glm::radians(90.0f), Up);

	            temp_offset = offset + translate;
	            temp_offset = glm::rotate(temp_offset, glm::radians(90.0f), Up);   
	        break;
	        case 'S':
	            temp_min = glm::vec3(0.5f, -0.5f, 0.5f);
	            temp_min *= scaling;
	            temp_min = glm::rotate(temp_min, glm::radians(180.0f), Up);

	            temp_max = glm::vec3(-0.5f, 0.5f, -0.5f);
	            temp_max *= scaling;
	            temp_max = glm::rotate(temp_max, glm::radians(180.0f), Up);

	            temp_offset = offset + translate;
	            temp_offset = glm::rotate(temp_offset, glm::radians(180.0f), Up);   
	        break;
	        case 'N':
	            temp_min = glm::vec3(-0.5f, -0.5f, -0.5f);
	            temp_min *= scaling;
	            
	            temp_max = glm::vec3(0.5f, 0.5f, 0.5f);
	            temp_max *= scaling;

	            temp_offset = offset + translate;
	        break;
	    }

	    //translate to offset (within object) 
	    BB step_bb;
	    step_bb.min = temp_min + temp_offset + bottom_left;
	    step_bb.max = temp_max + temp_offset + bottom_left;
	    steps_bb.push_back(step_bb);
	}


	//WHOLE STAIRCASE BOUNDING BOX
	offset += glm::vec3(0.0f, height/2.0f, 0.0f);
	glm::vec3 scaling = glm::vec3(width, height, length);

    //select correct cube corner so that rotated ends up min/max also scale
    switch(orientation) //Facing N by default
    {
        case 'E':
            temp_min = glm::vec3(-0.5f, -0.5f, 0.5f);
            temp_min *= scaling;
            temp_min = glm::rotate(temp_min, glm::radians(-90.0f), Up);

            temp_max = glm::vec3(0.5f, 0.5f, -0.5f);
            temp_max *= scaling;
            temp_max = glm::rotate(temp_max, glm::radians(-90.0f), Up);      

            offset = glm::rotate(offset, glm::radians(-90.0f), Up);          
        break;
        case 'W':
            temp_min = glm::vec3(0.5f, -0.5f, -0.5f);
            temp_min *= scaling;
            temp_min = glm::rotate(temp_min, glm::radians(90.0f), Up);

            temp_max = glm::vec3(-0.5f, 0.5f, 0.5f);
            temp_max *= scaling;
            temp_max = glm::rotate(temp_max, glm::radians(90.0f), Up);

            offset = glm::rotate(offset, glm::radians(90.0f), Up);   
        break;
        case 'S':
            temp_min = glm::vec3(0.5f, -0.5f, 0.5f);
            temp_min *= scaling;
            temp_min = glm::rotate(temp_min, glm::radians(180.0f), Up);

            temp_max = glm::vec3(-0.5f, 0.5f, -0.5f);
            temp_max *= scaling;
            temp_max = glm::rotate(temp_max, glm::radians(180.0f), Up);

            offset = glm::rotate(offset, glm::radians(180.0f), Up);   
        break;
        case 'N':
            temp_min = glm::vec3(-0.5f, -0.5f, -0.5f);
            temp_min *= scaling;
            
            temp_max = glm::vec3(0.5f, 0.5f, 0.5f);
            temp_max *= scaling;
        break;
    }

    //translate to offset (within object) 
    stairs_bb.min = temp_min + offset + bottom_left;
    stairs_bb.max = temp_max + offset + bottom_left;
}

Stairs::~Stairs()
{
	delete Root;
	delete step;
}

bool Stairs::SetHeight(glm::vec3 player_position, glm::vec3 &player_min_bb, glm::vec3 &player_max_bb)
{

	//NEED TO PASS IN FLOOR HEIGHT FROM ROOM??? NOT ASSUME 0?

	float player_height = player_max_bb.y - player_min_bb.y;
	float ref_dist = length;
    float player_dist;
    float new_height;

	switch (orientation)
	{
		case 'E':
           player_dist = stairs_bb.max.x - player_min_bb.x; 
        break;
        case 'W':   
           player_dist = player_max_bb.x - stairs_bb.min.x;
        break;
        case 'S':
           player_dist = stairs_bb.max.z - player_min_bb.z;
        break;
        case 'N': //small ratio when coming from bottom of steps
           player_dist = player_max_bb.z - stairs_bb.min.z;
        break;
	}

	float local_height = height + stairs_bb.min.y;
	new_height = stairs_bb.min.y + height * player_dist/length;
	new_height = new_height < local_height ? new_height : local_height;
	
	if(player_dist > 0.2f && new_height - player_min_bb.y > 0.3f)
        return false;

    if(player_min_bb.y < new_height)
    {
    	player_min_bb = glm::vec3(player_min_bb.x, new_height, player_min_bb.z);
    	player_max_bb = glm::vec3(player_min_bb.x, new_height + player_height, player_max_bb.z);
    }

	return true;
}