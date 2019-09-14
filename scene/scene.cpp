/*#include "scene.h"

Scene::Scene()
{
	Root = new nNode();
	nTransform* Origin = new nTransform(glm::vec3( 0.0f,  0.0f,  0.0f));
	nAsset* Crate = new nAsset(&crate, eObject); 
	nLight* Light = new nLight(&light);
	Root->AddChildren(Origin);
	Origin->AddChildren(Light);
	Origin->AddChildren(Crate);
}
*/