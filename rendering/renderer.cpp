#include "renderer.h"

Renderer::Renderer()
{
    //MatrixStack.push(glm::mat4(1.0f));
    for(int i = 0; i < MAX_BULLET_HOLES; i++)
    {
    	bulletHoles.push_back(glm::vec3(1000.0f));
    }
}

void Renderer::RenderRoom(Room* room, Camera* cam)
{

	if (room == NULL || cam == NULL)
		return;

	shadowLights.clear();
	shadowLightsIndex.clear();
	RenderGraph(room->Lights, cam); //traverse lights node and set camera 
	SetLights(room); //say how many lights in room

	//SHADOW PASS CONDITIONAL ON THE ROOM!
	shadowPass = room->shadowPass;

	//Shadow Pass for each Light
	for(int i = 0; i < shadowLights.size(); i++)
	{
		//Create Depth Map  
		if(shadowPass)
		{
			RenderDepthMapRoom(room, shadowLights[i], shadowLightsIndex[i]);
		}
		//Bind Depth Map  
		shadowLights[i].bindShadowMap(&myShader, shadowLightsIndex[i]);
		shadowLights[i].bindShadowMap(&modelShader, shadowLightsIndex[i]);
		shadowLights[i].bindShadowMap(&instancedShader, shadowLightsIndex[i]);
	}

	//Set DepthMap Sampler id for unused ones  
	for(int i = shadowLights.size(); i < PointLight::MAX_LIGHTS; i++)
	{
		shadowLights[0].bindShadowMap(&myShader, i);
		shadowLights[0].bindShadowMap(&modelShader, i);
		shadowLights[0].bindShadowMap(&instancedShader, i);	
	}

	// std::cout << std::endl;

	room->shadowPass = false;
	shadowPass = false;
	
	//Rendering Pass 
	for(int i = 0; i < room->targets.size(); i++) //New Method Render Targets directly =) 
	{
		float shotTime = room->targets[i]->TimeSinceShot(); 
		modelExplodeShader.use();
		modelExplodeShader.setFloat("deltaTime", shotTime);
		Traverse(room->targets[i]->GetNodeModel(), eModel);
	}

	//Bullet holes -- only need to be done for one of the rooms 
	if((room->ID == 0) && (Weapon::newBullet) && (Room::minBulletDist < Room::maxBulletDist))
	{
		instancedShader.use(); 
		instancedShader.setFloat("radiusImpact", 0.03f);
		
		glm::vec3 bulletImpact = cam->Position + glm::normalize(cam->Front) * Room::minBulletDist;
		bulletHoles[bulletHolesIndex] = bulletImpact;
		bulletHolesIndex++;
		if(bulletHolesIndex > MAX_BULLET_HOLES)
			bulletHolesIndex = 0;
		
		for(int i = 0; i < MAX_BULLET_HOLES; i++)
		{
			std::string str_index = std::to_string(i);
			instancedShader.setVec3("bulletHoles[" + str_index + "]", bulletHoles[i]);
		}
	}
}

void Renderer::RenderDepthMapRoom(Room* room, PointLight light, int lightIndex)
{
	light.shadowPassSetup(&depthShader, lightIndex); //detpthShader is active

	glCullFace(GL_FRONT); //group geometry that needs this kind of rendering
	depthShader.setBool("isInstanced", false);
	for(int i = 0; i < room->targets.size(); i++) //Render Targets directly, no outline since shadowPass  
	{	
		if(!room->targets[i]->IsShot())
		{
			Traverse(room->targets[i]->GetNodeModel(), eModel);
		}
	}
	//ADD WEAPON RENDERING HERE

	depthShader.setBool("isInstanced", true);
	room->i_crate->Render();
	room->i_beam->Render();

	glCullFace(GL_BACK); //back to the default 
	//room->i_floor->Render();
	room->i_wall->Render();
	room->i_door->Render();
	//room->i_ceiling->Render();

	glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void Renderer::SetCamera(Camera* cam)
{
	myShader.setCamera(cam);
	lightShader.setCamera(cam);
	instancedShader.setCamera(cam);
	modelShader.setCamera(cam);
	outlineShader.setCamera(cam);
	modelExplodeShader.setCamera(cam);
}

void Renderer::SetLights(Room* room)
{
	int numLights = room->pointLightPos.size();
	//std::cout << numLights << std::endl;
	myShader.setLightInfo(numLights);
	instancedShader.setLightInfo(numLights);
	modelShader.setLightInfo(numLights);
	modelExplodeShader.setLightInfo(numLights);
}

void Renderer::GetLights(Room* room) //Set Current Lights to be those the player is in, before rendering Weapon!
{
	//RenderGraph(room->Lights, cam); //traverse lights node and set camera 
	Traverse(room->Lights, eRoot); //"this" pointer from room, do not check for NULL as will say it is 
	int numLights = room->pointLightPos.size();
	myShader.setLightInfo(numLights);
	modelShader.setLightInfo(numLights);
}

void Renderer::RenderWeapon(Weapon* weapon, Camera* cam, float deltaTime) //NEED Weapon Specific actions to position it correctly 
{	

	myShader.setCamera(cam);
	modelShader.setCamera(cam);


	weapon->Update(deltaTime);

	glm::vec4 weapon_offsets = weapon->GetADSOffset(); 
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

	modeltr = glm::translate(modeltr,  -1.0f * weapon_axis * 0.02f * weapon->GetRecoilOffset()); //Horizontal recoil
	modeltr = glm::scale(modeltr, weapon->GetScaling());

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
	modeltr = glm::rotate(modeltr, glm::radians(weapon->GetRecoilOffset()), rot_axis); //Vertical recoil -- No Recoil If reloading
	modeltr = glm::rotate(modeltr, glm::radians(90.0f * (float) sin(weapon->GetReloadOffset() * (3.1415f))), rot_axis); //When Reloading -- Rotation by 0 if not reload 

	glClear(GL_DEPTH_BUFFER_BIT); //To Avoid Weapon Clipping into Objects
	

	if(weapon->IsFiring() && !weapon->IsFullyADS())
	{

		float time = weapon->GetFireOffset(); //glfwGetTime();
		float spinSpeed = 1000.0f;
		float rotationMuzzle = time * spinSpeed;
		float frontOffsetMuzzle = 6.0f;

		//Muzzle Flash 
		myShader.setMaterial(weapon->muzzleFlash->getMaterial());
		
		glm::mat4 muzzleStart_V = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), cam->WorldUp);
		muzzleStart_V = glm::rotate(muzzleStart_V, rotationMuzzle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 muzzleStart;

		glm::vec3 undoRotation_V = glm::rotate( glm::vec3(frontOffsetMuzzle, 0.8f, 0.0f), - 1.0f * rotationMuzzle, glm::vec3(1.0f, 0.0f, 0.0f) );
		muzzleStart = glm::translate(muzzleStart_V, undoRotation_V);
		muzzleStart = modeltr * muzzleStart;
		myShader.setTransform(glm::scale(muzzleStart, 1.0f/(3.0f*weapon->GetScaling())));
		Square.Render();


		glm::mat4 muzzleStart_H = glm::rotate(muzzleStart_V, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 undoRotation_H = glm::rotate( glm::vec3(frontOffsetMuzzle, 0.0f, 0.8f), - 1.0f * rotationMuzzle, glm::vec3(1.0f, 0.0f, 0.0f) );
		muzzleStart = glm::translate(muzzleStart_H, undoRotation_H);
		muzzleStart = modeltr * muzzleStart;
		myShader.setTransform(glm::scale(muzzleStart, 1.0f/(3.0f*weapon->GetScaling())));
		Square.Render();	
	}
	

	//Weapon Model
	glEnable(GL_CULL_FACE); 
	glCullFace(GL_BACK);

	modelShader.setTransform(modeltr);
	weapon->GetModel()->Draw(modelShader);

	glDisable(GL_CULL_FACE); 

}


void Renderer::RenderSkybox(Skybox* skybox, Camera* cam)
{
	skyboxShader.use();
	skyboxShader.setInt("skybox", 0);
	glm::mat4 view = glm::mat4(glm::mat3(cam->GetViewMatrix()));  
    skyboxShader.setMat4("viewing", view);
    glm::mat4 proj = glm::perspective(glm::radians(cam->Zoom), cam->ViewRatio, cam->NearPlane, cam->FarPlane);
    skyboxShader.setMat4("projection", proj);
	skybox->Draw();
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

			if(shadowPass)
			{
				depthShader.setTransform(model_transform); //depthShader be active! 
			}
			
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

			if(shadowPass)
			{
				depthShader.setTransform(model_transform);
				Mdl->GetModel()->Draw(depthShader);
			}
			else if(Mdl->HasOutline()) //IF MODEL HAS AN OUTLINE -- disable Outline for Shadow Pass, building depth map
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
			else if (Mdl->GetShader() == eExplode)
			{
				modelExplodeShader.setTransform(model_transform);
				//modelExplodeShader.setFloat("deltaTime", glfwGetTime());
				Mdl->GetModel()->Draw(modelExplodeShader);
				//std::cout << "YOU GOT SHOT" << std::endl;
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

		shadowLights.push_back(*light); //Add this light to the lights to compute depth map for
		shadowLightsIndex.push_back(Lt->GetIndex()); //so that have index of light in shader, can bind correct cubeMap to correct Texture slot

		myShader.setPointLight(light, Lt->GetIndex()); //INDEX FROM SCENEGRAPH
		instancedShader.setPointLight(light, Lt->GetIndex()); //INSTANCED SHADER TOO
		modelShader.setPointLight(light, Lt->GetIndex()); //MODEL SHADER
		modelExplodeShader.setPointLight(light, Lt->GetIndex()); //MODEL EXPLODE CUBE SHADER

		//DRAWING LIGHT CUBE
		//model_transform = glm::scale(model_transform, glm::vec3(2.0f, 0.2f, 1.0f)); //change light cube dimensions
		//lightShader.setTransform(model_transform);
		//lightShader.setVec3("lightColor", glm::vec3(1.0f));
		//Cube.Render();

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