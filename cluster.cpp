#include "cluster.h"
#include <glm/gtx/rotate_vector.hpp>

BB Cluster::Vertical(nNode* Root, int width, int height, glm::vec3 position, float y_rotation, Asset* asset ) //RETURN BOUNDING BOX
{
	//ADD GEOMETRY TO TREE
	nNode* Ref = Root;
	
	Ref = Ref->AddChildrenRecursive(new nRotate(y_rotation, glm::vec3(0.0f, 1.0f, 0.0f), true)); //ASSUME ALREADY AT ORIGIN
	Ref = Ref->AddChildrenRecursive(new nTranslate(position));
	
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Ref = Ref->AddChildrenRecursive(new nAsset(asset, eInstanced));

            if(j < width-1)
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::rotate(glm::vec3(1.0f, 0.0f, 0.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f))));
        }
        
        Ref = Ref->AddChildrenRecursive(new nTranslate (glm::rotate(glm::vec3(-1.0f * (width - 1), 1.0f, 0.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f))));
    }

    //COMPUTE AND RETURN BOUNDING BOX 
	BB BoundBox;

	glm::vec3 min_offset = glm::rotate(glm::vec3(-0.5f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 max_offset = glm::rotate(glm::vec3(width, height, 1.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 min_cluster = glm::vec3(position.x + min_offset.x, position.y + min_offset.y, position.z + min_offset.z);
	glm::vec3 max_cluster = glm::vec3(min_cluster.x + max_offset.x, min_cluster.y + max_offset.y, min_cluster.z + max_offset.z);

    BoundBox.min = min_cluster; //MIN CORNER
    BoundBox.max = max_cluster;

    return BoundBox;
}


BB Cluster::Horizontal(nNode* Root, int width, int length, glm::vec3 position, float y_rotation, Asset* asset ) //RETURN BOUNDING BOX
{
	//ADD GEOMETRY TO TREE
	nNode* Ref = Root;
	
	Ref = Ref->AddChildrenRecursive(new nRotate(y_rotation, glm::vec3(0.0f, 1.0f, 0.0f), true)); //ASSUME ALREADY AT ORIGIN
	Ref = Ref->AddChildrenRecursive(new nTranslate(position));

    for(int i = 0; i < length; i++)
    {
        for(int j = 0; j < width; j++)
        {
            Ref = Ref->AddChildrenRecursive(new nAsset(asset, eInstanced));

            if(j < width-1)
                Ref = Ref->AddChildrenRecursive(new nTranslate(glm::rotate(glm::vec3(1.0f, 0.0f, 0.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f))));
        }
        
        Ref = Ref->AddChildrenRecursive(new nTranslate (glm::rotate(glm::vec3(-1.0f * (width - 1), 0.0f, 1.0f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f))));
    }
	
	//COMPUTE AND RETURN BOUNDING BOX 
	BB BoundBox;

	glm::vec3 min_offset = glm::rotate(glm::vec3(-0.5f), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));
	glm::vec3 max_offset = glm::rotate(glm::vec3(width, 1.0f, length), y_rotation, glm::vec3(0.0f, 1.0f, 0.0f));

	glm::vec3 min_cluster = glm::vec3(position.x + min_offset.x, position.y + min_offset.y, position.z + min_offset.z);
	glm::vec3 max_cluster = glm::vec3(min_cluster.x + max_offset.x, min_cluster.y + max_offset.y, min_cluster.z + max_offset.z);

    BoundBox.min = min_cluster; //MIN CORNER
    BoundBox.max = max_cluster;

    return BoundBox;
}




BB Cluster::Array()
{}