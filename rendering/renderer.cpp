#include "renderer.h"

Renderer::Renderer()
{
    //MatrixStack.push(glm::mat4(1.0f));
}

void Renderer::RenderRoom(Room* room, Camera* cam)
{
	if (room == NULL || cam == NULL)
		return;

	RenderGraph(room->Lights, cam); //TO SET UP LIGHTS, FIRST THING
	//RenderGraph(room->Root, cam); //For Targets if in Graph
	
	for(int i = 0; i < room->targets.size(); i++) //New Method Render Targets directly =) 
	{
		Traverse(room->targets[i]->GetNodeModel(), eModel);
	}

	instancedShader.setMaterial(room->crate->getMaterial()); 
	room->i_crate->Render();
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
}

void Renderer::SetCamera(Camera* cam)
{
	myShader.setCamera(cam);
	lightShader.setCamera(cam);
	instancedShader.setCamera(cam);
	modelShader.setCamera(cam);
	outlineShader.setCamera(cam);
}

void Renderer::SetLights(Room* room)
{
	Traverse(room->Lights, eRoot); //"this" pointer from room, do not check for NULL as will say it is 
}

void Renderer::RenderWeapon(Weapon* weapon, Camera* cam) //NEED Weapon Specific actions to position it correctly 
{

	glm::vec4 weapon_offsets = weapon->GetOffset(); 
	cam->Zoom = weapon_offsets.w;
	modelShader.setCamera(cam); //zoom camera when ADS

	glm::mat4 modeltr = glm::mat4(1.0f);
	modeltr = glm::translate(modeltr, cam->Position);
	
	float front_offset = weapon_offsets.x; 
	float right_offset = weapon_offsets.y;
	float down_offset = weapon_offsets.z;

	glm::vec3 offset_weapon = cam->Front * front_offset + cam->Right * right_offset + cam->Up * -down_offset; 
	modeltr = glm::translate(modeltr, offset_weapon);

	glm::vec3 center_weapon = cam->Position + offset_weapon;
	float point_offset = 20.0f; //how far ahead weapon points
	glm::vec3 point_to = cam->Position + cam->Front * point_offset;
	glm::vec3 weapon_axis = glm::normalize(point_to - center_weapon); 

	modeltr = glm::scale(modeltr, weapon->scaling);

	//ALIGN FRONT - POINT TO CAM FRONT DIR 
	glm::vec3 front_loaded = glm::vec3(0.0f, 0.0f, 1.0f); //Dir Vector of loaded weapom -- same for all 
	float horiz_angle = atan2(weapon_axis.z, weapon_axis.x) - atan2(front_loaded.z, front_loaded.x); //Horiz angle between loaded front and where weapon should be 
	if(horiz_angle < 0.0f)																		     //Note special arctan2 to get [0,2pi] angle
		horiz_angle += 2.0f * 3.1415f; 
	modeltr = glm::rotate(modeltr, -horiz_angle, cam->WorldUp); //Align weapon horizontally 
	
	//ALIGN SIDE - POINT TO CAM FRONT HEIGHT
	//glm::vec3 right_weapon = glm::normalize(glm::cross( weapon_axis, cam->WorldUp)); //right of weapon	
	glm::vec3 front_flat = glm::normalize(glm::vec3(cam->Front.x, 0.0f, cam->Front.z)); //horiz plane point to
	glm::vec3 rot_axis = glm::normalize(glm::cross(front_flat, cam->WorldUp));
	rot_axis = glm::rotate(rot_axis, horiz_angle, cam->WorldUp); //Align vertical rotation axis to new reference
	float vert_angle = acos(glm::dot(front_flat, cam->Front)); //angle in [-pi/2, pi/2]
	if(cam->Front.y < 0.0f) //SMG  
		vert_angle *= -1.0f;
	modeltr = glm::rotate(modeltr, vert_angle, rot_axis);//glm::vec3(right_weapon.x, 0.0f, right_weapon.z)); //Align weapon vertically 

	modelShader.setTransform(modeltr);

	glClear(GL_DEPTH_BUFFER_BIT); //To Avoid Weapon Clipping into Objects

	glEnable(GL_CULL_FACE); 
	glCullFace(GL_BACK);

	weapon->GetModel()->Draw(modelShader);

	glDisable(GL_CULL_FACE); 

}



void Renderer::RenderGraph(nNode* Root, Camera* cam)
{
	if (Root == NULL || cam == NULL)
		return;

	SetCamera(cam);
	Traverse(Root, eRoot); 
}

void Renderer::Traverse(nNode* Root, eType type)
{
	std::list<nNode*> nChildren = Root->GetChildren();

	if(type == eRoot) //ALWAYS GO THERE FIRST
	{
		//std::cout << "ROOT" << std::endl;
		//std::cout << "HAS " << nChildren.size() << " KIDS" << std::endl;
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

	else if (type == eModel)
	{		
		//std::cout << "MODEL" << std::endl;

		nModel* Mdl = dynamic_cast<nModel*> (Root);
		
		if(Mdl->RenderStatus()) //ALSO LEAVE OPTION TO RENDER WITH GEOMETRY SHADER WHEN EXPLODE
		{
			glm::mat4 model_transform;

			if(Mdl->HasTransform()) //IF RENDER MODEL ON ITS OWN -- sometimes want to render without being part of a tree
			{
				model_transform = Mdl->GetTransform();
			}
			else
				 model_transform = MatrixStack.top();

			if(Mdl->HasOutline()) //IF MODEL HAS AN OUTLINE
			{
    			glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
				glStencilFunc(GL_ALWAYS, 1, 0xFF);
				glStencilMask(0xFF);
				modelShader.setTransform(model_transform);
				Mdl->GetModel()->Draw(modelShader);

				glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
				glStencilMask(0x00);
				outlineShader.setTransform(model_transform);
				Mdl->GetSmoothModel()->Draw(outlineShader);

				glStencilMask(0xFF);
				glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
				glClear(GL_STENCIL_BUFFER_BIT);
			
			}
			else
			{
				modelShader.setTransform(model_transform);
				Mdl->GetModel()->Draw(modelShader);
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
		modelShader.setPointLight(light, Lt->GetIndex()); //MODEL SHADER

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

	else
		std::cout << " -- UNKNOWN TYPE -- " << std::endl;
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