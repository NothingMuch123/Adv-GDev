#include "AGDev_Assign01.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>

#include "..\Application.h"
#include "..\GameStateManager\GameStateManager.h"

AGDev_Assign01::AGDev_Assign01(int width, int height)
	: SceneBase(width, height)
	, test(NULL)
	, nodeTest(NULL)
	, m_char(NULL)
	, m_spatialPartition(NULL)
	, m_projList(NULL)
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		m_meshList[i] = NULL;
	}
}

AGDev_Assign01::~AGDev_Assign01()
{
	Exit();
}

void AGDev_Assign01::Init(int screenWidth, int screenHeight)
{
	Application::HideMouse(true);

	// Initialise SceneBase
	SceneBase::Init(screenWidth, screenHeight);

	// Init sound
	sound = irrklang::createIrrKlangDevice();
	if (!sound)
	{
		std::cout << "Could not start sound engine" << std::endl;
	}

	InitMesh();

	test = new CGameObject();
	CTransform* transform = new CTransform();
	test->Init(m_meshList[MESH_CUBE], transform);
	test->CCollider::Init(CCollider::CT_AABB, test->GetTransform(), CCollider::X_MIDDLE, CCollider::Y_MIDDLE, true);

	nodeTest = new CSceneNode();
	transform = new CTransform();
	transform->Init(Vector3(0, 10, -20), Vector3(), Vector3(20,20,20));
	nodeTest->Init(CSceneNode::NODE_TEST, m_meshList[MESH_CUBE], transform);

	// Third person player
	m_char = new CThirdPerson();
	Camera3* view = new Camera3();
	view->Init(Vector3(0, 10, CThirdPerson::S_OFFSET_TARGET), Vector3(0, 10, 0), Vector3(0, 1, 0));
	transform = new CTransform();
	transform->Init(Vector3(0, 10, CThirdPerson::S_OFFSET_TARGET), Vector3(), Vector3(10,10,10));
	m_char->Init(CSceneNode::NODE_TEST, view, m_meshList[MESH_CHARACTER], transform);

	// Spatial partition
	m_spatialPartition = new CSpatialPartition();
	m_spatialPartition->Init(5, 5, 1000.f, 1000.f, m_meshList[MESH_CUBE]);

	m_spatialPartition->AddObject(m_char);
	m_spatialPartition->AddObject(nodeTest);

	m_camera = m_char->GetTPView();
}

void AGDev_Assign01::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(GSM, dt);

	for (vector<CProjectile*>::iterator it = m_projList.begin(); it != m_projList.end(); ++it)
	{
		CProjectile* proj = *it;
		if (proj)
		{
			proj->Update(dt);
			Vector3 projPos_Start = proj->GetTransform().m_translate;
			if (m_spatialPartition->CheckForCollision(projPos_Start))
			{
				proj->Reset();
			}
		}
	}

	m_fps = (float)(1.f / dt);
}

void AGDev_Assign01::Render()
{
	// Render SceneBase
	SceneBase::Render();

	// Render target
	CGameObject* target = new CGameObject();
	CTransform* transform = new CTransform();
	transform->Init(m_char->GetTPView()->target, Vector3(), Vector3(5, 5, 5));
	target->Init(m_meshList[MESH_CUBE], transform);
	RenderGameObject(target, m_lightEnabled);

	//RenderGameObject(nodeTest, m_lightEnabled);
	nodeTest->Draw(this);

	RenderSkybox();
	RenderGround();

	RenderGameObject(test, m_lightEnabled);
	RenderGameObject(m_char, m_lightEnabled);
	RenderGameObject(m_char->GetLocation(), m_lightEnabled);

	// Render projectile
	for (vector<CProjectile*>::iterator it = m_projList.begin(); it != m_projList.end(); ++it)
	{
		CProjectile* proj = *it;
		RenderGameObject(proj, m_lightEnabled);
	}

	// Render spatial partition
	/*vector<CGrid*> gridList = m_spatialPartition->GetGridList();
	for (int i = 0; i < gridList.size(); ++i)
	{
		RenderGameObject(gridList[i], m_lightEnabled);
	}*/

	//SetHUD(true);
	ostringstream sFPS;
	sFPS << "FPS: " << m_fps;
	RenderTextOnScreen(m_meshList[MESH_TEXT], sFPS.str(), Color(1, 0, 0), 50, 0, 0);
	//SetHUD(false);

	// Not supposed to have any other rendering codes here as Scenebase handles it
	// Alternative solution is to render scenegraph here instead as render list does not take into account parent and child nodes
}

void AGDev_Assign01::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

	if (test)
	{
		delete test;
		test = NULL;
	}
	if (nodeTest)
	{
		delete nodeTest;
		nodeTest = NULL;
	}
	if (m_spatialPartition)
	{
		delete m_spatialPartition;
		m_spatialPartition = NULL;
	}
	if (m_char)
	{
		delete m_char;
		m_char = NULL;
	}
	if (m_spatialPartition)
	{
		delete m_spatialPartition;
		m_spatialPartition = NULL;
	}

	for (int i = 0; i < m_projList.size(); ++i)
	{
		CProjectile* proj = m_projList[i];
		if (proj)
		{
			delete proj;
			proj = NULL;
		}
	}

	// Delete sound
	sound->drop();
}

void AGDev_Assign01::Reset()
{
	SceneBase::Reset();
}

void AGDev_Assign01::ProcessKeys(CGameStateManager* GSM, double dt, bool* keys, Vector2 mousePos)
{
	if (keys[CGameStateManager::KEY_P])
	{
		GSM->PopState();
		return;
	}
	bool charMovement = false;
	if (keys[CGameStateManager::KEY_MOVE_FORWARD])
	{
		m_char->MoveForward(dt);
		charMovement = true;
	}
	if (keys[CGameStateManager::KEY_MOVE_BACKWARD])
	{
		m_char->MoveBackward(dt);
		charMovement = true;
	}
	if (keys[CGameStateManager::KEY_MOVE_LEFT])
	{
		m_char->MoveLeft(dt);
		charMovement = true;
	}
	if (keys[CGameStateManager::KEY_MOVE_RIGHT])
	{
		m_char->MoveRight(dt);
		charMovement = true;
	}
	if (keys[CGameStateManager::KEY_MOVE_UP])
	{
		m_char->MoveUp(dt);
		charMovement = true;
	}
	if (keys[CGameStateManager::KEY_MOVE_DOWN])
	{
		m_char->MoveDown(dt);
		charMovement = true;
	}
	if (keys[CGameStateManager::KEY_SPRINT])
	{
		m_char->Sprint(true);
	}
	else
	{
		m_char->Sprint(false);
	}
	if (!charMovement)
	{
		m_char->changeTargetOffset(dt, false);
	}
	m_spatialPartition->UpdateObject(m_char);

	if (keys[CGameStateManager::KEY_SHOOT_1])
	{
		CProjectile* bullet = new CProjectile();
		Vector3 dir = (m_char->GetTPView()->target - m_char->GetTPView()->position).Normalized();
		CTransform* transform = new CTransform();
		transform->Init(m_char->GetTPView()->position, Vector3(), Vector3(5, 5, 5));
		bullet->Init(dir, 100.f, m_meshList[MESH_SPHERE], transform);
		m_projList.push_back(bullet);
	}
}

void AGDev_Assign01::ProcessMouse(CGameStateManager* GSM, double dt, float yaw, float pitch, Vector2 mousePos)
{
	m_char->Look(dt, yaw, pitch);
}

void AGDev_Assign01::UpdateCameraStatus(unsigned char key)
{

}

void AGDev_Assign01::UpdateWeaponStatus(unsigned char key, double dt)
{

}

void AGDev_Assign01::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
{
	if(m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10); // Origin at bottom left
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		glUniform1f(m_parameters[U_FOG_ENABLE], 0);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
		glUniform1f(m_parameters[U_FOG_ENABLE], fogEnabled);
	}
}

void AGDev_Assign01::InitMesh()
{
	m_meshList[MESH_TEXT] = MeshBuilder::GenerateText("Calibri font", 16, 16);
	m_meshList[MESH_TEXT]->textureID[0] = LoadTGA("Image\\calibri.tga");
	m_meshList[MESH_CUBE] = MeshBuilder::GenerateCube("Cube", Color(0, 1, 0), 1.f);
	m_meshList[MESH_CONE] = MeshBuilder::GenerateCone("Cone", Color(1, 0, 0), 36, 0.5f, 1.f);
	m_meshList[MESH_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0, 0, 1), 18, 36, 0.5f);

	m_meshList[MESH_CHARACTER] = MeshBuilder::GenerateOBJ("Character", "OBJ\\Object\\Monster.obj");
	m_meshList[MESH_CHARACTER]->textureID[0] = LoadTGA("Image\\Object\\Monster.tga");

	// Skybox
	m_meshList[GEO_LEFT] = MeshBuilder::GenerateQuad("LEFT", Color(1, 1, 1), 1.f);
	m_meshList[GEO_LEFT]->textureID[0] = LoadTGA("Image//left.tga");
	m_meshList[GEO_RIGHT] = MeshBuilder::GenerateQuad("RIGHT", Color(1, 1, 1), 1.f);
	m_meshList[GEO_RIGHT]->textureID[0] = LoadTGA("Image//right.tga");
	m_meshList[GEO_TOP] = MeshBuilder::GenerateQuad("TOP", Color(1, 1, 1), 1.f);
	m_meshList[GEO_TOP]->textureID[0] = LoadTGA("Image//top.tga");
	m_meshList[GEO_BOTTOM] = MeshBuilder::GenerateQuad("BOTTOM", Color(1, 1, 1), 1.f);
	m_meshList[GEO_BOTTOM]->textureID[0] = LoadTGA("Image//bottom.tga");
	m_meshList[GEO_FRONT] = MeshBuilder::GenerateQuad("FRONT", Color(1, 1, 1), 1.f);
	m_meshList[GEO_FRONT]->textureID[0] = LoadTGA("Image//front.tga");
	m_meshList[GEO_BACK] = MeshBuilder::GenerateQuad("BACK", Color(1, 1, 1), 1.f);
	m_meshList[GEO_BACK]->textureID[0] = LoadTGA("Image//back.tga");
	m_meshList[GEO_GRASS_DARKGREEN] = MeshBuilder::GenerateQuad("GRASS_DARKGREEN", Color(1, 1, 1), 1.f);
	m_meshList[GEO_GRASS_DARKGREEN]->textureID[0] = LoadTGA("Image//grass_darkgreen.tga");
	m_meshList[GEO_GRASS_LIGHTGREEN] = MeshBuilder::GenerateQuad("GEO_GRASS_LIGHTGREEN", Color(1, 1, 1), 1.f);
	m_meshList[GEO_GRASS_LIGHTGREEN]->textureID[0] = LoadTGA("Image//grass_lightgreen.tga");

	m_meshList[MESH_FLOOR] = MeshBuilder::GenerateQuad("Floor", Color(1,1,1), 1.f);
	m_meshList[MESH_FLOOR]->textureID[0] = LoadTGA("Image//Object//Floor.tga");
}


void AGDev_Assign01::RenderSkybox()
{
	const float SKYBOXSIZE = 1000.f;
	//left
	modelStack.PushMatrix();
	modelStack.Rotate(90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(m_meshList[GEO_LEFT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(m_meshList[GEO_RIGHT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(m_meshList[GEO_FRONT], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(180, 0, 1, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(m_meshList[GEO_BACK], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(m_meshList[GEO_TOP], false);
	modelStack.PopMatrix();

	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -SKYBOXSIZE / 2 + 2.f);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(SKYBOXSIZE, SKYBOXSIZE, SKYBOXSIZE);
	RenderMesh(m_meshList[GEO_BOTTOM], false);

	modelStack.PopMatrix();
}

void AGDev_Assign01::RenderGround()
{
	modelStack.PushMatrix();
	modelStack.Rotate(-90, 1, 0, 0);
	modelStack.Translate(0, 0, -10);
	modelStack.Rotate(-90, 0, 0, 1);
	modelStack.Scale(100.0f, 100.0f, 100.0f);

	for (int x = 0; x<10; x++)
	{
		for (int z = 0; z<10; z++)
		{
			modelStack.PushMatrix();
			modelStack.Translate(x - 5.0f, z - 5.0f, 0.0f);
			if (((x * 9 + z) % 2) == 0)
				RenderMesh(m_meshList[GEO_GRASS_DARKGREEN], false);
			else
				RenderMesh(m_meshList[GEO_GRASS_LIGHTGREEN], false);
			modelStack.PopMatrix();
		}
	}
	modelStack.PopMatrix();
}

void AGDev_Assign01::PreRendering(Vector3 translate, Vector3 rotate, Vector3 scale, bool enableLight)
{
	modelStack.PushMatrix();
	modelStack.Translate(translate.x, translate.y, translate.z);
	modelStack.Rotate(rotate.x, 1, 0, 0);
	modelStack.Rotate(rotate.y, 0, 1, 0);
	modelStack.Rotate(rotate.z, 0, 0, 1);
	modelStack.Scale(scale.x, scale.y, scale.z);

	Mtx44 MVP, modelView, modelView_inverse_transpose;

	MVP = projectionStack.Top() * viewStack.Top() * modelStack.Top();
	glUniformMatrix4fv(m_parameters[U_MVP], 1, GL_FALSE, &MVP.a[0]);
	if (enableLight && m_lightEnabled)
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 1);
		modelView = viewStack.Top() * modelStack.Top();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW], 1, GL_FALSE, &modelView.a[0]);
		modelView_inverse_transpose = modelView.GetInverse().GetTranspose();
		glUniformMatrix4fv(m_parameters[U_MODELVIEW_INVERSE_TRANSPOSE], 1, GL_FALSE, &modelView.a[0]);
	}
	else
	{
		glUniform1i(m_parameters[U_LIGHTENABLED], 0);
	}
}

void AGDev_Assign01::PostRendering()
{
	glBindTexture(GL_TEXTURE_2D, 0);
	modelStack.PopMatrix();
}

void AGDev_Assign01::InitMap()
{
	/*std::vector<std::string> map;
	std::string data;
	std::ifstream fMap;
	fMap.open("map.txt");
	while (!fMap.eof())
	{
		fMap >> data;
		map.push_back(data);
	}
	map.pop_back();
	skyboxSize.Set(map[0].length() * 100, 60, map.size() * 100);
	skyboxOffset = 5;

	meshList[GEO_WALL] = MeshBuilder::GenerateOBJ("Wall", "OBJ\\wall.obj");
	meshList[GEO_WALL]->textureID = LoadTGA("Image\\MazeWall.tga");
	meshList[GEO_WALL]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_WALL]->material.kDiffuse.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_WALL]->material.kShininess = 1.f;

	meshList[GEO_MONSTER] = MeshBuilder::GenerateOBJ("Monster", "OBJ\\Monster.obj");
	meshList[GEO_MONSTER]->textureID = LoadTGA("Image\\Monster.tga");
	meshList[GEO_MONSTER]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_MONSTER]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_MONSTER]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_MONSTER]->material.kShininess = 1.f;

	meshList[GEO_CHEST] = MeshBuilder::GenerateOBJ("Chest", "OBJ\\Chest.obj");
	meshList[GEO_CHEST]->textureID = LoadTGA("Image\\Chest.tga");
	meshList[GEO_CHEST]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_CHEST]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_CHEST]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_CHEST]->material.kShininess = 1.f;

	meshList[GEO_TROPHY] = MeshBuilder::GenerateOBJ("Trophy", "OBJ\\Trophy.obj");
	meshList[GEO_TROPHY]->textureID = LoadTGA("Image\\Trophy.tga");
	meshList[GEO_TROPHY]->material.kAmbient.Set(0.2f, 0.2f, 0.2f);
	meshList[GEO_TROPHY]->material.kDiffuse.Set(0.5f, 0.5f, 0.5f);
	meshList[GEO_TROPHY]->material.kSpecular.Set(0.1f, 0.1f, 0.1f);
	meshList[GEO_TROPHY]->material.kShininess = 1.f;

	float startX = -(skyboxSize.x / 2) + 50, startZ = -(skyboxSize.z / 2) + 50; // X starts from left and Z starts from top
	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map[0].length(); col++)
		{
			if (map[row][col] == '#') // Only change translate X & Z
			{
				InitObj(meshList[GEO_WALL], startX + (col * 100), 0, startZ + (row * 100), 0, 0, 0, 100, skyboxSize.y, 100, CObj::SHAPES::S_CUBE);
			}
			else if (map[row][col] == 'M')
			{
				InitObj(meshList[GEO_MONSTER], startX + (col * 100), (-7.5 * 2.5) - 22, startZ + (row * 100), 0, 180, 0, 2.5, 2.5, 2.5, CObj::SHAPES::S_MONSTER);
			}
			else if (map[row][col] == 'C')
			{
				InitObj(meshList[GEO_CHEST], startX + (col * 100), -skyboxSize.y / 2, startZ + (row * 100), 0, 180, 0, 20, 20, 20, CObj::SHAPES::S_CHEST);
			}
			else if (map[row][col] == 'S')
			{
				modelPosition.Set(startX + (col * 100), 10, startZ + (row * 100));
				modelTarget.Set(modelPosition.x - 1, modelPosition.y, modelPosition.z);
				modelUp.Set(0, 1, 0);
				modelView = (modelTarget - modelPosition).Normalized();
				startMaxBound.Set(startX + (col * 100) + 50, 30, startZ + (row * 100) + 50);
				startMinBound.Set(startX + (col * 100) - 50, -30, startZ + (row * 100) - 50);
			}
			else if (map[row][col] == 'E')
			{
				InitObj(meshList[GEO_TROPHY], startX + (col * 100), -skyboxSize.y / 2, startZ + (row * 100), 0, 0, 0, 1, 1, 1, CObj::SHAPES::S_TROPHY);
				endMaxBound.Set(startX + (col * 100) + 50, 30, startZ + (row * 100) + 50);
				endMinBound.Set(startX + (col * 100) - 50, -30, startZ + (row * 100) - 50);
			}
		}
	}*/
}