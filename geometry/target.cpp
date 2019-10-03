#include "target.h"

bool Target::loadedModel = false;
bool Target::loadedSmoothModel = false;
int Target::count = 0;
Model* Target::model = NULL;
Model* Target::smoothModel = NULL;

Target::Target(glm::vec3 position, char orientation) //'N' 'S' 'E' 'W'
{
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	
	switch (orientation) // LOADED ALIGNED WITH W
	{
		case 'N':
			transform = glm::rotate(transform, glm::radians(-90.0f), Up);
		break;
		case 'S':
			transform = glm::rotate(transform, glm::radians(90.0f), Up);
		break;
		case 'E':
			transform = glm::rotate(transform, glm::radians(180.0f), Up);
		break;
		default:
		break;
	}

	count++;

	nodeModel = new nModel(model, eModelshdr);
	nodeModel->SetTransform(transform);
}

Target::Target(glm::vec3 position, char orientation, glm::vec3 outline_color, float outline_size) //'N' 'S' 'E' 'W'
{
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	transform = glm::mat4(1.0f);
	transform = glm::translate(transform, position);
	
	switch (orientation) // LOADED ALIGNED WITH W
	{
		case 'N':
			transform = glm::rotate(transform, glm::radians(-90.0f), Up);
		break;
		case 'S':
			transform = glm::rotate(transform, glm::radians(90.0f), Up);
		break;
		case 'E':
			transform = glm::rotate(transform, glm::radians(180.0f), Up);
		default:
		break;
	}
	
	
	count++;

	outline = new Outline(outline_color, outline_size);
	nodeModel = new nModel(model, smoothModel, outline, eModelshdr);
	nodeModel->SetTransform(transform);
}

Target::~Target()
{
	if ( count <= 0)
		delete model;
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


bool Target::Collide()
{}