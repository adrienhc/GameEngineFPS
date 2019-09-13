#include "renderer.h"

Renderer::Renderer()
{
    //MatrixStack.push(glm::mat4(1.0f));
}

void Renderer::RenderRoom(Room* room, Camera* cam, float ratio)
{
	if (room == NULL || cam == NULL)
		return;

	RenderGraph(room->Root, cam, ratio); //RENDER NON INSTANCED -- TO SET UP LIGHTS, FIRST THING


	instancedShader.setMaterial(room->floor->getMaterial()); 
	room->i_floor->Render();
	instancedShader.setMaterial(room->wall->getMaterial());
	room->i_wall->Render();
	instancedShader.setMaterial(room->door->getMaterial());
	room->i_door->Render();
	instancedShader.setMaterial(room->beam->getMaterial());
	room->i_beam->Render();
	instancedShader.setMaterial(room->ceiling->getMaterial());
	room->i_ceiling->Render();
	instancedShader.setMaterial(room->crate->getMaterial()); //RENDER INSTANCED
	room->i_crate->Render();

}

void Renderer::RenderGraph(nNode* Root, Camera* cam, float ratio)
{
	if (Root == NULL || cam == NULL)
		return;

	myShader.setCamera(cam, ratio);
	lightShader.setCamera(cam, ratio);
	instancedShader.setCamera(cam, ratio);

	Traverse(Root, eRoot); 
}

void Renderer::Traverse(nNode* Root, eType type)
{
	std::list<nNode*> nChildren = Root->GetChildren();

	if(type == eRoot) //ALWAYS GO THERE FIRST
	{
		//std::cout << "ROOT" << std::endl;
		while(!MatrixStack.empty())
			MatrixStack.pop();
		MatrixStack.push(glm::mat4(1.0f));

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}
	}

	else if (type == eTranslate)
	{
		//std::cout << "TR" << std::endl;
		nTranslate* Tr = dynamic_cast<nTranslate*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Tr->Get();
		MatrixStack.push( currMat * prevMat); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eRotate)
	{
		//std::cout << "ROT" << std::endl;
		nRotate* Rot = dynamic_cast<nRotate*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Rot->Get();
		MatrixStack.push( currMat * prevMat); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

		Rot->Update(); // Keep Rotating for next update

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eScale)
	{
		//std::cout << "SC" << std::endl;
		nScale* Sc = dynamic_cast<nScale*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Sc->Get();
		MatrixStack.push( prevMat * currMat ); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eAsset)
	{
		//std::cout << "ASSET" << std::endl;
		
		nAsset* Ast = dynamic_cast<nAsset*> (Root);
		
		if(Ast->RenderStatus())
		{
			//eShader shader_type = Root->GetShader();
			glm::mat4 model_transform = MatrixStack.top();
			myShader.setTransform(model_transform);
			
			Asset* asset = Ast->GetAsset();
			myShader.setMaterial(asset->getMaterial());
			eShape shape = asset->getGeometry();
			
			if(shape == eCube)
			{
				Cube.Render();
			}
			else if(shape == eTriangle)
			{
				Triangle.Render();
			}
			else if(shape == eSquare)
			{
				Square.Render();
			}
		}

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}

	}
	else if (type == ePointLight)
	{
		//std::cout << "LIGHT" << std::endl;
		nPointLight* Lt = dynamic_cast<nPointLight*> (Root);
		PointLight* light = Lt->GetLight();
		glm::mat4 model_transform = MatrixStack.top();
		light->setTransform(model_transform);

		myShader.setPointLight(light, Lt->GetIndex()); //INDEX FROM SCENEGRAPH
		instancedShader.setPointLight(light, Lt->GetIndex()); //INSTANCED SHADER TOO

		//DRAWING LIGHT CUBE
		model_transform = glm::scale(model_transform, glm::vec3(2.0f, 0.2f, 1.0f)); //change light cube dimensions
		lightShader.setTransform(model_transform);
		lightShader.setVec3("lightColor", glm::vec3(1.0f));
		Cube.Render();

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Traverse(*it, (*it)->GetType());
		}
	}
}







void Renderer::Instance(nNode* Root, eType type, Asset* Instanced, std::vector<glm::mat4> &m_transforms, std::vector<glm::mat3> &n_transforms)
{

	std::list<nNode*> nChildren = Root->GetChildren();

	if(type == eRoot) //ALWAYS GO THERE FIRST
	{
		//std::cout << "ROOT" << std::endl;
		while(!MatrixStack.empty())
			MatrixStack.pop();
		MatrixStack.push(glm::mat4(1.0f));

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
		}
	}

	else if (type == eTranslate)
	{
		//std::cout << "TR" << std::endl;
		nTranslate* Tr = dynamic_cast<nTranslate*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Tr->Get();
		MatrixStack.push( currMat * prevMat); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
		}

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eRotate)
	{
		//std::cout << "ROT" << std::endl;
		nRotate* Rot = dynamic_cast<nRotate*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Rot->Get();
		MatrixStack.push( currMat * prevMat); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
		}

		Rot->Update(); // Keep Rotating for next update

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eScale)
	{
		//std::cout << "SC" << std::endl;
		nScale* Sc = dynamic_cast<nScale*> (Root);
		glm::mat4 prevMat = MatrixStack.top();
		glm::mat4 currMat = Sc->Get();
		MatrixStack.push( prevMat * currMat ); //APPLY MATRIX, R TO L MULTIPLICATION

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
		}

		if(!MatrixStack.empty())
		{
			MatrixStack.pop();
		}
	}

	else if (type == eAsset)
	{
		nAsset* Ast = dynamic_cast<nAsset*> (Root);
		
				
		if ( (Ast->GetShader() == eInstanced) && (Ast->GetAsset()->getName() == Instanced->getName()) && (Ast->RenderStatus() == true) )
		{

			glm::mat4 model_transform = MatrixStack.top(); 
			m_transforms.push_back(model_transform);
			glm::mat3 norm_transform = glm::mat3(glm::transpose(glm::inverse(model_transform)));
			n_transforms.push_back(norm_transform);

			Ast->Instanced(); //USEFUL -- TO NOT RE-INSTANCE THE SAME THING TWICE  
		}
		

		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
		}

	}
	else if (type == ePointLight)
	{
		for(std::list<nNode*>::iterator it = nChildren.begin(); it != nChildren.end(); it++)
		{
			Instance(*it, (*it)->GetType(), Instanced, m_transforms, n_transforms);
		}
	}
}