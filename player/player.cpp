#include "player.h"

Player::Player(Camera* camera, Weapon* weapon)
{
	this->camera = camera;
	this->weapon = weapon;
	this->weapon->GetModel()->cullingOFF();
}

void Player::Update(float deltaTime)
{
	weapon->Update(deltaTime);

	glm::vec4 weapon_offsets = weapon->GetADSOffset(); 
	camera->Zoom = weapon_offsets.w;

	glm::mat4 modeltr = glm::mat4(1.0f);
	modeltr = glm::translate(modeltr, camera->Position);
	
	float front_offset = weapon_offsets.x; 
	float right_offset = weapon_offsets.y;
	float down_offset = weapon_offsets.z;

	glm::vec3 offset_weapon = camera->Front * front_offset + camera->Right * right_offset + camera->Up * -down_offset; 
	modeltr = glm::translate(modeltr, offset_weapon);

	glm::vec3 center_weapon = camera->Position + offset_weapon;
	float point_offset = 20.0f; //how far ahead weapon points
	glm::vec3 point_to = camera->Position + camera->Front * point_offset;
	glm::vec3 weapon_axis = glm::normalize(point_to - center_weapon); 

	modeltr = glm::translate(modeltr,  -1.0f * weapon_axis * 0.02f * weapon->GetRecoilOffset()); //Horizontal recoil
	modeltr = glm::scale(modeltr, weapon->GetScaling());

	//ALIGN FRONT - POINT TO CAM FRONT DIR 
	glm::vec3 front_loaded = glm::vec3(0.0f, 0.0f, 1.0f); //Dir Vector of loaded weapom -- same for all 
	float horiz_angle = atan2(weapon_axis.z, weapon_axis.x) - atan2(front_loaded.z, front_loaded.x); //Horiz angle between loaded front and where weapon should be 
	if(horiz_angle < 0.0f)																		     //Note special arctan2 to get [0,2pi] angle
		horiz_angle += 2.0f * 3.1415f; 
	modeltr = glm::rotate(modeltr, -horiz_angle, camera->WorldUp); //Align weapon horizontally 
	
	//ALIGN SIDE - POINT TO CAM FRONT HEIGHT
	//glm::vec3 right_weapon = glm::normalize(glm::cross( weapon_axis, camera->WorldUp)); //right of weapon	
	glm::vec3 front_flat = glm::normalize(glm::vec3(camera->Front.x, 0.0f, camera->Front.z)); //horiz plane point to
	glm::vec3 rot_axis = glm::normalize(glm::cross(front_flat, camera->WorldUp));
	rot_axis = glm::rotate(rot_axis, horiz_angle, camera->WorldUp); //Align vertical rotation axis to new reference
	float vert_angle = acos(glm::dot(front_flat, camera->Front)); //angle in [-pi/2, pi/2]
	if(camera->Front.y < 0.0f) //SMG  
		vert_angle *= -1.0f;
	modeltr = glm::rotate(modeltr, vert_angle, rot_axis);//glm::vec3(right_weapon.x, 0.0f, right_weapon.z)); //Align weapon vertically 
	modeltr = glm::rotate(modeltr, glm::radians(weapon->GetRecoilOffset()), rot_axis); //Vertical recoil -- No Recoil If reloading
	modeltr = glm::rotate(modeltr, glm::radians(90.0f * (float) sin(weapon->GetReloadOffset() * (3.1415f))), rot_axis); //When Reloading -- Rotation by 0 if not reload 

	weapon->SetModelTransform(modeltr);
}

void Player::addLayer(AbstractLayer* layer, bool flash)
{

	nNode* Player = new nNode();
	glm::mat4 modeltr = weapon->GetModelTransform();

	if(!flash)
	{
		nModel* model = new nModel(weapon->GetModel(), eModelshdr);
		model->SetTransform(modeltr);
		Player->AddChildren(model);
	}

	if(flash && weapon->IsFiring() && !weapon->IsFullyADS())
	{

		float time = weapon->GetFireOffset(); //glfwGetTime();
		float spinSpeed = 1000.0f;
		float rotationMuzzle = time * spinSpeed;
		float scaleMuzzle = 10.0f * weapon->GetScaling().x;
		float frontOffsetMuzzle = 6.0f;

		//Muzzle Flash 
		glm::mat4 muzzleStart_V = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), camera->WorldUp);
		muzzleStart_V = glm::rotate(muzzleStart_V, rotationMuzzle, glm::vec3(1.0f, 0.0f, 0.0f));
		glm::mat4 muzzleStart;

		glm::vec3 undoRotation_V = glm::rotate( glm::vec3(frontOffsetMuzzle, 0.8f, 0.0f), - 1.0f * rotationMuzzle, glm::vec3(1.0f, 0.0f, 0.0f) );
		muzzleStart = glm::translate(muzzleStart_V, undoRotation_V);
		muzzleStart = modeltr * muzzleStart;
		muzzleStart = glm::scale(muzzleStart, scaleMuzzle * 1.0f/(3.0f*weapon->GetScaling()));
		nAsset* flash1 = new nAsset(weapon->muzzleFlash, eObject);
		flash1->SetTransform(muzzleStart);
		Player->AddChildren(flash1);


		glm::mat4 muzzleStart_H = glm::rotate(muzzleStart_V, glm::radians(-90.0f), glm::vec3(1.0f, 0.0f, 0.0f));
		glm::vec3 undoRotation_H = glm::rotate( glm::vec3(frontOffsetMuzzle, 0.0f, 0.8f), - 1.0f * rotationMuzzle, glm::vec3(1.0f, 0.0f, 0.0f) );
		muzzleStart = glm::translate(muzzleStart_H, undoRotation_H);
		muzzleStart = modeltr * muzzleStart;
		muzzleStart = glm::scale(muzzleStart, scaleMuzzle * 1.0f/(3.0f*weapon->GetScaling()));
		nAsset* flash2 = new nAsset(weapon->muzzleFlash, eObject);
		flash2->SetTransform(muzzleStart);
		Player->AddChildren(flash2);
	}

	Group* g_player = new Group(Player);
	layer->Add(g_player);
}

void Player::addParticle(ParticleSystem* particle_system)
{
	float frontOffsetMuzzle = 4.5f;
	float heightOffsetMuzzle = 0.6f;
	glm::vec4 muzzleFront = glm::vec4(0.0f, heightOffsetMuzzle, frontOffsetMuzzle, 1.0f); //Weapon Loaded Facing Z axis
	glm::vec4 muzzleBack = glm::vec4(0.0f, heightOffsetMuzzle, 0.8f * frontOffsetMuzzle, 1.0f);
	glm::mat4 modeltr = weapon->GetModelTransform();
	
	glm::vec4 emissionPoint = modeltr * muzzleFront;
	glm::vec3 emissionFront = glm::vec3(emissionPoint.x, emissionPoint.y, emissionPoint.z)/emissionPoint.w;
	
	glm::vec4 emissionPointBack = modeltr * muzzleBack;
	glm::vec3 emissionBack = glm::vec3(emissionPointBack.x, emissionPointBack.y, emissionPointBack.z)/emissionPointBack.w;

	glm::vec3 emissionDirection = emissionFront - emissionBack;

	/*if(weapon->IsFlashing() && !weapon->IsFullyADS())
	{
		Particle* base = new Particle(weapon->baseFlash, emissionPoint, emissionDirection);
		particle_system->Add(base, pCube, 100);			
	}*/

	if(weapon->GetMagazineCapacity() == 0.0f || weapon->IsReloading())
	{
		Particle* base = new Particle(weapon->baseSmokeStill, emissionPoint, emissionDirection);
		// float smokeFactor = 1.0f - ();
		particle_system->Add(base, pPoint, 16); 
	}
	/*else if(weapon->IsFlashing())
	{
		Particle* base = new Particle(weapon->baseSmokeFire, emissionPoint, emissionDirection);
		particle_system->Add(base, pPoint, 4); 
	}*/

	if(weapon->NewBullet())
	{
		Particle* base = new Particle(weapon->baseBullet, emissionPoint, emissionDirection);
		particle_system->Add(base, pLine, 1);
	}
	
}