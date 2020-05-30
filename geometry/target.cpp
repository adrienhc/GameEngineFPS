#include "target.h"

bool Target::loadedModel = false;
bool Target::loadedSmoothModel = false;
int Target::count = 0;
Model* Target::model = NULL;
Model* Target::smoothModel = NULL;

Target::Target(glm::vec3 position, char orientation) //'N' 'S' 'E' 'W'
{
	//Target Model Node
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	
	switch (orientation) // LOADED ALIGNED WITH N
	{
		case 'E':
			transform = glm::rotate(transform, glm::radians(-90.0f), Up);
		break;
		case 'W':
			transform = glm::rotate(transform, glm::radians(90.0f), Up);
		break;
		case 'S':
			transform = glm::rotate(transform, glm::radians(180.0f), Up);
		break;
		default:
		break;
	}

	count++;

	nodeModel = new nModel(model, eModelshdr);
	nodeModel->SetTransform(transform);

	//Target Bounding Boxes
	body_low_bb.InitFromCube(glm::vec3(-0.05f, -0.75f, 0.0f), glm::vec3(1.17f, 0.85f, 0.1f), orientation);
	body_high_bb.InitFromCube(glm::vec3(0.1f, 0.1f, 0.0f), glm::vec3(1.5f, 0.95f, 0.1f), orientation);
	head_bb.InitFromCube(glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.85f, 0.8f, 0.1f), orientation);

	body_low_bb.Translate(position);
	body_high_bb.Translate(position);
	head_bb.Translate(position);
}

Target::Target(glm::vec3 position, char orientation, glm::vec4 outline_color, float outline_size) //'N' 'S' 'E' 'W'
{
	//Target Mdel Node
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	
	switch (orientation) // LOADED ALIGNED WITH N
	{
		case 'E':
			transform = glm::rotate(transform, glm::radians(-90.0f), Up);
		break;
		case 'W':
			transform = glm::rotate(transform, glm::radians(90.0f), Up);
		break;
		case 'S':
			transform = glm::rotate(transform, glm::radians(180.0f), Up);
		default:
		break;
	}
	
	
	count++;

	outline = new Outline(outline_color, outline_size);
	nodeModel = new nModel(model, smoothModel, outline, eModelshdr);
	nodeModel->SetTransform(transform);

	//Target Bounding Boxes
	body_low_bb.InitFromCube(glm::vec3(-0.05f, -0.75f, 0.0f), glm::vec3(1.17f, 0.85f, 0.1f), orientation);
	body_high_bb.InitFromCube(glm::vec3(0.1f, 0.1f, 0.0f), glm::vec3(1.5f, 0.95f, 0.1f), orientation);
	head_bb.InitFromCube(glm::vec3(0.0f, 0.8f, 0.0f), glm::vec3(0.85f, 0.8f, 0.1f), orientation);

	body_low_bb.Translate(position);
	body_high_bb.Translate(position);
	head_bb.Translate(position);

}

Target::~Target()
{
	//count--;
	//if ( count <= 0)
		//delete model;
	delete outline;
	delete nodeModel;
}

void Target::LoadModel(char* path)
{
	if(!loadedModel)
	{
		model = new Model(path);
		loadedModel = true;
	}
}

void Target::LoadSmoothModel(char* path)
{
	if(!loadedSmoothModel)
	{
		smoothModel = new Model(path);
		loadedSmoothModel = true;
	}
}

nModel* Target::GetNodeModel()
{
	return nodeModel;
}

Model* Target::GetModel()
{
	return model;
}

Model* Target::GetSmoothModel()
{
	return smoothModel;
}

Outline* Target::GetOutline()
{
	return outline;
}

void Target::ResetDist(float max_dist)
{
	collisionDist = max_dist;
}

bool Target::TestDist(float dist)
{
	return collisionDist <= dist;
}

void Target::SetHitRay(bool val)
{
	hitRay = val;
}

bool Target::HitRay()
{
	return hitRay;
}

void Target::Shot()
{
	if(!shot)
	{
		shot = true;
		shotTime = glfwGetTime();
		delete outline;
		outline = NULL;
		nodeModel->Explode();	
	}
}

bool Target::IsShot()
{
	return shot;
}

float Target::TimeSinceShot()
{
	if(!shot)
		return 0.0f;
	else
	{
		return (glfwGetTime() - shotTime);
	}
}

bool Target::Erase()
{
	return (TimeSinceShot() >= lifeSpan);
}