#include "AGDev_Assign01.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>

#include "..\Application.h"
#include "..\GameStateManager\GameStateManager.h"

const float AGDev_Assign01::S_SHOOT_COUNTDOWN = 1.f;

AGDev_Assign01::AGDev_Assign01(int width, int height)
	: SceneBase(width, height)
	, m_char(NULL)
	, m_spatialPartition(NULL)
	, m_projList(NULL)
	, m_toggleSPgrid(false)
	, m_shootTimer(S_SHOOT_COUNTDOWN)
	, m_score(0.f)
	, m_rayLength(5.f)
	, m_enemyList(NULL)
	, m_tilemap(nullptr)
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
	Math::InitRNG();

	// Initialise SceneBase
	SceneBase::Init(screenWidth, screenHeight);

	// Init sound
	sound = irrklang::createIrrKlangDevice();
	if (!sound)
	{
		std::cout << "Could not start sound engine" << std::endl;
	}

	if (CGameStateManager::S_MUSIC)
	{
		sound->play2D("Sound\\HolFix_Savior.mp3", true);
	}

	InitMesh();
	initProjList();

	// Set up enemy resolution distance from lua
	CLua_Wrapper* lua = new CLua_Wrapper();
	if (lua->OpenLua("Lua_Scripts//enemy.lua"))
	{
		double* data = nullptr;

		if (data = lua->GetNumber("ENEMY_RES_LOW_DIST"))
		{
			m_enemyResDist[CLevelOfDetail::RES_LOW] = (float)(*data);
		}
		if (data = lua->GetNumber("ENEMY_RES_MID_DIST"))
		{
			m_enemyResDist[CLevelOfDetail::RES_MID] = (float)(*data);
		}
		if (data = lua->GetNumber("ENEMY_RES_HIGH_DIST"))
		{
			m_enemyResDist[CLevelOfDetail::RES_HIGH] = (float)(*data);
		}
		if (data = lua->GetNumber("ENEMY_RES_ULTRA_DIST"))
		{
			m_enemyResDist[CLevelOfDetail::RES_ULTRA] = (float)(*data);
		}
		lua->CloseLua();
		delete lua;
	}

	InitMap();
	CEnemy::InitEnemyDataFromLua();
	if (CGameStateManager::S_LOAD)
	{
		load();
	}

	m_camera = m_char->GetTPView();
}

void AGDev_Assign01::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(GSM, dt);
	m_score += dt;

	if (m_end->CollideWith(*m_char, dt))
	{
		GSM->PopState();
		if (CGameStateManager::S_HIGHSCORE == 0.f || m_score < CGameStateManager::S_HIGHSCORE)
		{
			CGameStateManager::S_HIGHSCORE = m_score;
		}
		return;
	}

	if (m_shootTimer > 0.f)
	{
		m_shootTimer -= dt;
	}
	
	// LoD Update
	/*vector<CGrid*> gridList = m_char->GetLocations();
	for (vector<CGrid*>::iterator it = gridList.begin(); it != gridList.end(); ++it)
	{
		CGrid* g = *it;
		if (g)
		{
			vector<CSceneNode*> nodeList = g->GetList();
			for (vector<CSceneNode*>::iterator it2 = nodeList.begin(); it2 != nodeList.end(); ++it2)
			{
				CSceneNode* node = *it2;
				if (node)
				{
					node->UpdateLOD(dt, m_char);
				}
			}
		}
	}*/

	// Enemy update
	for (vector<CEnemy*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		CEnemy* e = *it;
		if (e)
		{
			e->Detect(m_char);
			e->Update(dt);
			m_spatialPartition->UpdateObject(e);
			e->UpdateLOD(dt, m_char);
			if (e->IsAlive() && e->CollideWith(*m_char, dt))
			{
				m_char->Injure(1);
				e->Kill();
			}
		}
	}

	for (vector<CProjectile*>::iterator it = m_projList.begin(); it != m_projList.end(); ++it)
	{
		CProjectile* proj = *it;
		if (proj && proj->CObject::GetActive())
		{
			proj->Update(dt);
			Vector3 projPos_Start = proj->GetTransform().m_translate;
			if (proj->GetLength() <= 0)
			{
				if (proj->CObject::GetActive() && m_spatialPartition->CheckForCollision(projPos_Start) || proj->GetTransform().m_translate.y > 100.f || proj->GetTransform().m_translate.y < 0.f)
				{
					proj->Reset();
				}
				else
				{
					for (int i = 0; i < m_enemyList.size(); ++i)
					{
						CEnemy* enemy = m_enemyList[i];
						if (enemy && enemy->CObject::GetActive())
						{
							if (proj->CollideWith(*enemy, dt))
							{
								proj->Reset();
								enemy->Kill();
								break;
							}
						}
					}
					if (proj->CObject::GetActive())
					{
						for (int i = 0; i < m_wallList.size(); ++i)
						{
							CSceneNode* wall = m_wallList[i];
							if (wall && wall->CObject::GetActive())
							{
								if (proj->CollideWith(*wall, dt))
								{
									proj->Reset();
									break;
								}
							}
						}
					}
				}
			}
			else
			{
				Vector3 projPos_End = projPos_Start + (proj->GetDir() * proj->GetLength());
				if (m_spatialPartition->CheckForCollision(projPos_Start, projPos_End) || proj->GetTransform().m_translate.y > 100.f || proj->GetTransform().m_translate.y < 0.f)
				{
					proj->Reset();
				}
				else
				{
					for (int i = 0; i < m_enemyList.size(); ++i)
					{
						CEnemy* enemy = m_enemyList[i];
						if (enemy && enemy->CObject::GetActive())
						{
							if (proj->CollideWith(*enemy, dt))
							{
								proj->Reset();
								enemy->Kill();
								break;
							}
						}
					}
				}
			}
		}
	}

	if (!m_char->IsAlive())
	{
		GSM->PopState();
		return;
	}

	m_fps = (float)(1.f / dt);
}

void AGDev_Assign01::Render()
{
	// Render SceneBase
	SceneBase::Render();

	// Render target
	/*CGameObject* target = new CGameObject();
	CTransform* transform = new CTransform();
	transform->Init(m_char->GetTPView()->target, Vector3(), Vector3(5, 5, 5));
	target->Init(m_meshList[MESH_CUBE], transform);
	RenderGameObject(target, m_lightEnabled);*/

	RenderGameObject(m_end, m_lightEnabled);

	//RenderSkybox();
	RenderGround();

	RenderGameObject(m_char, m_lightEnabled);

	if (m_toggleSPgrid)
	{
		vector<CGrid*> locations = m_char->GetLocations();
		for (vector<CGrid*>::iterator it = locations.begin(); it != locations.end(); ++it)
		{
			CGrid* grid = *it;
			RenderGameObject(grid, m_lightEnabled);
		}
	}

	// Render projectile
	for (vector<CProjectile*>::iterator it = m_projList.begin(); it != m_projList.end(); ++it)
	{
		CProjectile* proj = *it;
		if (proj->GetLength() == 0) // Discrete
		{
			RenderGameObject(proj, m_lightEnabled);
		}
		else
		{
			glLineWidth(5.f);
			RenderGameObject(proj, m_lightEnabled);
			glLineWidth(1.f);
		}
	}
	
	// Render enemies
	for (vector<CEnemy*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		CEnemy* enemy = *it;
		//RenderGameObject(enemy, m_lightEnabled);
		enemy->Draw(this);
	}

	// Render walls
	for (vector<CSceneNode*>::iterator it = m_wallList.begin(); it != m_wallList.end(); ++it)
	{
		CSceneNode* wall = *it;
		RenderGameObject(wall, m_lightEnabled);
	}

	// Render spatial partition
	/*vector<CGrid*> gridList = m_spatialPartition->GetGridList();
	for (int i = 0; i < gridList.size(); ++i)
	{
		RenderGameObject(gridList[i], m_lightEnabled);
	}*/

	ostringstream sTime;
	sTime << "Time: " << m_score;
	RenderTextOnScreen(m_meshList[MESH_TEXT], sTime.str(), Color(1, 0, 0), 40, 0, 0);

	ostringstream sLives;
	sLives << "Lives: " << m_char->GetHealth();
	RenderTextOnScreen(m_meshList[MESH_TEXT], sLives.str(), Color(1, 0, 0), 40, 0, 40);

	// Not supposed to have any other rendering codes here as Scenebase handles it
	// Alternative solution is to render scenegraph here instead as render list does not take into account parent and child nodes
}

void AGDev_Assign01::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

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
	if (m_end)
	{
		delete m_end;
		m_end = NULL;
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
	for (vector<CEnemy*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		CSceneNode* enemy = *it;
		if (enemy)
		{
			delete enemy;
			enemy = NULL;
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
		save();
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
	else
	{
		m_char->CCollider::Update(m_char->GetTransform());
		m_spatialPartition->UpdateObject(m_char);
	}

	if (keys[CGameStateManager::KEY_SHOOT_1] && m_shootTimer < 0.f)
	{
		CProjectile* bullet = fetchProj();
		if (bullet)
		{
			Vector3 pos = m_char->GetView()->position;
			pos.y += m_char->GetTransform().m_scale.y;
			Vector3 target = m_char->GetView()->target;
			Vector3 dir = (target - pos).Normalized();

			CTransform* transform = new CTransform();
			transform->Init(pos, Vector3(), Vector3(1, 1, 1));
			bullet->Init(dir, 500.f, m_meshList[MESH_SPHERE], transform);
			bullet->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_MIDDLE, true);
			m_shootTimer = S_SHOOT_COUNTDOWN;
		}
	}

	if (keys[CGameStateManager::KEY_SHOOT_2] && m_shootTimer < 0.f)
	{
		CProjectile* bullet = fetchProj();
		if (bullet)
		{
			Vector3 pos = m_char->GetView()->position;
			pos.y += m_char->GetTransform().m_scale.y;
			Vector3 target = m_char->GetView()->target;
			Vector3 dir = (target - pos).Normalized();

			CTransform* transform = new CTransform();
			transform->Init(pos, m_char->GetTransform().m_rotate, Vector3(1, 1, 1));
			bullet->Init(dir, 500.f, m_meshList[MESH_RAY], transform);
			bullet->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_MIDDLE, true);
			bullet->SetLength(m_rayLength);
			m_shootTimer = S_SHOOT_COUNTDOWN;
		}
	}

	if (keys[CGameStateManager::KEY_UP])
	{
		m_toggleSPgrid = true;
	}
	if (keys[CGameStateManager::KEY_DOWN])
	{
		m_toggleSPgrid = false;
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
	m_meshList[MESH_SPHERE] = MeshBuilder::GenerateSphere("Sphere", Color(0, 0, 1), 18, 36, 0.5f);
	m_meshList[MESH_RAY] = MeshBuilder::GenerateRay("Ray", m_rayLength);
	m_meshList[MESH_CONE] = MeshBuilder::GenerateCone("Cone", Color(1, 0, 0), 36, 0.5f, 1.f);

	m_meshList[MESH_CHARACTER] = MeshBuilder::GenerateOBJ("Character", "OBJ\\Object\\Monster.obj");
	m_meshList[MESH_CHARACTER]->textureID[0] = LoadTGA("Image\\Object\\Monster.tga");

	m_meshList[MESH_WALL] = MeshBuilder::GenerateOBJ("Wall", "OBJ\\Object\\Wall.obj");
	m_meshList[MESH_WALL]->textureID[0] = LoadTGA("Image\\Object\\MazeWall.tga");

	m_meshList[MESH_END] = MeshBuilder::GenerateOBJ("Wall", "OBJ\\Object\\Trophy.obj");
	m_meshList[MESH_END]->textureID[0] = LoadTGA("Image\\Object\\Trophy.tga");

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

	// Enemy
	Color low, mid, high, ultra;
	CLua_Wrapper* lua = new CLua_Wrapper();
	lua->OpenLua("Lua_Scripts//enemy.lua");
	double* data = nullptr;

	// Low res colour
	if (data = lua->GetNumber("ENEMY_RES_LOW_R"))
	{
		low.r = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_LOW_G"))
	{
		low.g = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_LOW_B"))
	{
		low.b = (float)(*data);
	}
	m_meshList[MESH_ENEMY_LOW_RES] = MeshBuilder::GenerateSphere("Sphere", low, 6, 12, 0.5f);

	// Mid res colour
	if (data = lua->GetNumber("ENEMY_RES_MID_R"))
	{
		mid.r = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_MID_G"))
	{
		mid.g = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_MID_B"))
	{
		mid.b = (float)(*data);
	}
	m_meshList[MESH_ENEMY_MID_RES] = MeshBuilder::GenerateSphere("Sphere", mid, 9, 18, 0.5f);

	// High res colour
	if (data = lua->GetNumber("ENEMY_RES_HIGH_R"))
	{
		high.r = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_HIGH_G"))
	{
		high.g = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_HIGH_B"))
	{
		high.b = (float)(*data);
	}
	m_meshList[MESH_ENEMY_HIGH_RES] = MeshBuilder::GenerateSphere("Sphere", high, 18, 36, 0.5f);

	// Ultra res colour
	if (data = lua->GetNumber("ENEMY_RES_ULTRA_R"))
	{
		ultra.r = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_ULTRA_G"))
	{
		ultra.g = (float)(*data);
	}
	if (data = lua->GetNumber("ENEMY_RES_ULTRA_B"))
	{
		ultra.b = (float)(*data);
	}
	m_meshList[MESH_ENEMY_ULTRA_RES] = MeshBuilder::GenerateSphere("Sphere", ultra, 27, 54, 0.5f);
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
	modelStack.Translate(0, 0, 0);
	modelStack.Scale(m_spatialPartition->GetTotalWidth(), m_spatialPartition->GetTotalHeight(), 1.0f);
	RenderMesh(m_meshList[MESH_FLOOR], m_lightEnabled);
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
	glUniform1i(m_parameters[U_COLOR_TEXTURE_ENABLED], 0);
}

void AGDev_Assign01::PostRendering()
{
	for (int i = 0; i < Mesh::MAX_TEXTURES; ++i)
	{
		//if (mesh->textureID[i] > 0)
		{
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	modelStack.PopMatrix();
}

void AGDev_Assign01::InitMap()
{
	std::vector<std::string> map;
	std::string data;
	std::ifstream fMap;
	fMap.open("Lua_Scripts//map.lua");
	while (!fMap.eof())
	{
		fMap >> data;
		map.push_back(data);
	}
	map.pop_back();

	Vector3 size;
	CLua_Wrapper* lua = new CLua_Wrapper();
	if (!lua->OpenLua("Lua_Scripts//game.lua"))
	{
		//return;
	}
	double* lua_data = nullptr;
	if (lua_data = lua->GetNumber("SIZE_X"))
	{
		size.x = (float)(*lua_data);
	}
	if (lua_data = lua->GetNumber("SIZE_Y"))
	{
		size.y = (float)(*lua_data);
	}
	if (lua_data = lua->GetNumber("SIZE_Z"))
	{
		size.z = (float)(*lua_data);
	}

	const Vector3 SIZE(size);

	lua_getglobal(lua->GetState(), "multiply");
	lua_pushnumber(lua->GetState(), map.size());
	lua_pushnumber(lua->GetState(), SIZE.x);
	lua_call(lua->GetState(), 2, 1);
	float rowLength = (float)lua_tonumber(lua->GetState(), lua_gettop(lua->GetState()));//map.size() * SIZE.x; // X-axis length
	lua_pop(lua->GetState(), 1);

	lua_getglobal(lua->GetState(), "multiply");
	lua_pushnumber(lua->GetState(), map.front().length());
	lua_pushnumber(lua->GetState(), SIZE.z);
	lua_call(lua->GetState(), 2, 1);
	float colLength = (float)lua_tonumber(lua->GetState(), lua_gettop(lua->GetState()));//map.front().length() * SIZE.z; // Z-axis length
	lua_pop(lua->GetState(), 1);

	Vector3 defaultStart = Vector3(-rowLength * 0.5f, 0.f, -colLength * 0.5f);
	defaultStart = defaultStart + SIZE * 0.5f;
	Vector3 startPos;
	CSceneNode* node;
	CTransform* transform;

	lua->CloseLua();
	delete lua;

	// Spatial partition
	m_spatialPartition = new CSpatialPartition();
	m_spatialPartition->Init(5, 5, rowLength, colLength, m_meshList[MESH_CUBE]);

	// Tile map
	m_tilemap = new CTileMap(map.size(), map.front().length());
	CEnemy::S_MAP_REF = m_tilemap;

	for (int row = 0; row < map.size(); row++)
	{
		for (int col = 0; col < map[0].length(); col++)
		{
			// Set start pos
			startPos.x = defaultStart.x + (col * SIZE.x);
			startPos.y = 0.f;
			startPos.z = defaultStart.z + (row * SIZE.z);
			
			if (map[row][col] == '#') // Wall
			{
				// Map
				CTile* tile = new CTile(startPos, row, col, false);
				m_tilemap->AddToMap(tile);

				node = new CSceneNode();
				transform = new CTransform();
				transform->Init(startPos, Vector3(), SIZE);
				node->Init(CSceneNode::NODE_WALL, m_meshList[MESH_WALL], transform, tile);
				node->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);
				m_wallList.push_back(node);
				m_spatialPartition->AddObject(node);
			}
			else if (map[row][col] == 'M') // Enemy (Monster)
			{
				// Map
				CTile* tile = new CTile(startPos, row, col, true);
				m_tilemap->AddToMap(tile);

				if (!CGameStateManager::S_LOAD)
				{
					CEnemy* e = new CEnemy();
					transform = new CTransform();
					transform->Init(startPos + Vector3(0, SIZE.y * 0.25f, 0), Vector3(), SIZE * 0.25f);
					e->Init(CSceneNode::NODE_ENEMY, m_meshList[MESH_ENEMY_HIGH_RES], transform, tile);
					e->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_MIDDLE, true);
					e->CCollider::SetIgnore(false, true, false);

					// LOD
					Mesh* resList[CLevelOfDetail::NUM_RES] = {
																m_meshList[MESH_ENEMY_LOW_RES],
																m_meshList[MESH_ENEMY_MID_RES],
																m_meshList[MESH_ENEMY_HIGH_RES],
																m_meshList[MESH_ENEMY_ULTRA_RES],
					};
					e->InitLOD(resList, m_enemyResDist);

					// Child
					CSceneNode* cNode = new CSceneNode();
					transform = new CTransform();
					transform->Init(Vector3(0, 0.5f, 0), Vector3(), Vector3(0.5f, 0.5f, 0.5f));
					cNode->Init(CSceneNode::NODE_ENEMY_1, m_meshList[MESH_CONE], transform);
					cNode->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);
					cNode->InitLOD(resList, m_enemyResDist);
					e->AddChild(cNode);

					/*CSceneNode* c2Node = new CSceneNode();
					transform = new CTransform();
					transform->Init(Vector3(0, 1, 0), Vector3(), Vector3(0.5f, 0.5f, 0.5f));
					c2Node->Init(CSceneNode::NODE_ENEMY_2, m_meshList[MESH_CUBE], transform);
					c2Node->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);

					cNode->AddChild(c2Node);*/

					m_enemyList.push_back(e);
					m_spatialPartition->AddObject(e);
				}
			}
			else if (map[row][col] == 'C') // Chest
			{
				// Map
				CTile* tile = new CTile(startPos, row, col, true);
				m_tilemap->AddToMap(tile);
			}
			else if (map[row][col] == 'S') // Start
			{
				// Map
				CTile* tile = new CTile(startPos, row, col, true);
				m_tilemap->AddToMap(tile);

				if (!CGameStateManager::S_LOAD)
				{
					// Third person player
					m_char = new CThirdPerson();
					Camera3* view = new Camera3();
					view->Init(startPos, startPos - Vector3(0, 0, CThirdPerson::S_OFFSET_TARGET), Vector3(0, 1, 0));
					transform = new CTransform();
					transform->Init(startPos, Vector3(), SIZE * 0.1f);
					m_char->Init(CSceneNode::NODE_TEST, view, m_meshList[MESH_CHARACTER], transform, tile);
					m_char->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);
					m_spatialPartition->AddObject(m_char);
				}
			}
			else if (map[row][col] == 'E') // End
			{
				// Map
				CTile* tile = new CTile(startPos, row, col, true);
				m_tilemap->AddToMap(tile);

				m_end = new CSceneNode();
				transform = new CTransform();
				transform->Init(startPos, Vector3(), SIZE * 0.5f);
				m_end->Init(CSceneNode::NODE_WALL, m_meshList[MESH_END], transform, tile);
				m_end->CCollider::Init(CCollider::CT_AABB, *transform, CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);
				m_spatialPartition->AddObject(m_end);
			}
			else // Empty area
			{
				// Map
				CTile* tile = new CTile(startPos, row, col, true);
				m_tilemap->AddToMap(tile);
			}
		}
	}
}

void AGDev_Assign01::initProjList()
{
	for (int i = 0; i < 50; ++i)
	{
		m_projList.push_back(new CProjectile());
	}
}

CProjectile * AGDev_Assign01::fetchProj()
{
	for (vector<CProjectile*>::iterator it = m_projList.begin(); it != m_projList.end(); ++it)
	{
		CProjectile* proj = *it;
		if (!proj->GetRender())
		{
			return proj;
		}
	}
	return NULL;
}

void AGDev_Assign01::save()
{
	fstream file;

	file.open("Lua_Scripts//save.lua", std::ofstream::out, std::ofstream::trunc);

	// Lives
	file << "LIVES = " + to_string((long long)m_char->GetHealth()) + "\n\n";

	// Score
	file << "SCORE = " + to_string((long double)m_score) + "\n\n";

	// Save player
	m_char->SaveState(&file, 1);

	// Number of enemies
	file << "NUM_ENEMY = " + to_string((long long)m_enemyList.size()) + "\n\n";

	int id = 1;
	// Save enemies
	for (vector<CEnemy*>::iterator it = m_enemyList.begin(); it != m_enemyList.end(); ++it)
	{
		CEnemy* e = *it;
		e->SaveState(&file, id);
		++id;
	}

	CGameStateManager::S_LOAD = true;
	file.close();
}

void AGDev_Assign01::load()
{
	CLua_Wrapper* lua = new CLua_Wrapper();
	if (!lua->OpenLua("Lua_Scripts//save.lua"))
	{
		return;
	}
	double* data = nullptr;

	// Score
	if (data = lua->GetNumber("SCORE"))
	{
		m_score = (float)(*data);
	}

	// Third person player
	m_char = new CThirdPerson();
	Camera3* view = new Camera3();
	m_char->Init(CSceneNode::NODE_TEST, view, m_meshList[MESH_CHARACTER], new CTransform());
	m_char->LoadState(lua, 1);
	m_char->CCollider::Init(CCollider::CT_AABB, m_char->GetTransform(), CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);
	m_spatialPartition->AddObject(m_char);

	int enemyCount = 0;

	if (data = lua->GetNumber("NUM_ENEMY"))
	{
		enemyCount = *data;
	}

	for (int i = 0; i < enemyCount; ++i)
	{
		CEnemy* e = new CEnemy();
		e->Init(CSceneNode::NODE_ENEMY, m_meshList[MESH_ENEMY_HIGH_RES], new CTransform());
		e->LoadState(lua, i + 1);
		e->CCollider::Init(CCollider::CT_AABB, e->GetTransform(), CCollider::X_MIDDLE, CCollider::Y_MIDDLE, true);
		if (e->GetFSM() == CEnemy::ENEMY_KO || e->GetFSM() == CEnemy::ENEMY_RESPAWN)
		{
			e->CCollider::SetIgnore(false, false, false);
		}
		else
		{
			e->CCollider::SetIgnore(false, true, false);
		}

		// LOD
		Mesh* resList[CLevelOfDetail::NUM_RES] = {
			m_meshList[MESH_ENEMY_LOW_RES],
			m_meshList[MESH_ENEMY_MID_RES],
			m_meshList[MESH_ENEMY_HIGH_RES],
			m_meshList[MESH_ENEMY_ULTRA_RES],
		};
		e->InitLOD(resList, m_enemyResDist);

		// Child
		CSceneNode* cNode = new CSceneNode();
		CTransform transform;
		transform.Init(Vector3(0, 0.5f, 0), Vector3(), Vector3(0.5f, 0.5f, 0.5f));
		cNode->Init(CSceneNode::NODE_ENEMY_1, m_meshList[MESH_CONE], &transform);
		cNode->CCollider::Init(CCollider::CT_AABB, transform, CCollider::X_MIDDLE, CCollider::Y_BOTTOM, true);
		cNode->InitLOD(resList, m_enemyResDist);
		e->AddChild(cNode);

		m_enemyList.push_back(e);
		m_spatialPartition->AddObject(e);
	}

	// Lives
	if (data = lua->GetNumber("LIVES"))
	{
		m_char->SetHealth((float)(*data));
	}

	CGameStateManager::S_LOAD = false;

	lua->CloseLua();
	delete lua;
}
