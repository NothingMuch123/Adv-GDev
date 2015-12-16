#include "AGDev_Assign01.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>

#include "..\GameStateManager\GameStateManager.h"

AGDev_Assign01::AGDev_Assign01(int width, int height)
	: SceneBase(width, height)
	, test(NULL)
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		m_meshList[i] = NULL;
	}
}

AGDev_Assign01::~AGDev_Assign01()
{
	if (test)
	{
		delete test;
		test = NULL;
	}
}

void AGDev_Assign01::Init(int screenWidth, int screenHeight)
{
	// Initialise SceneBase
	SceneBase::Init(screenWidth, screenHeight);

	// Init sound
	sound = irrklang::createIrrKlangDevice();
	if (!sound)
	{
		std::cout << "Could not start sound engine" << std::endl;
	}

	initMesh();

	test = new CGameObject();
	CTransform* transform = new CTransform();
	test->Init(m_meshList[MESH_CUBE], transform);
	test->CCollider::Init(CCollider::CT_AABB, test->GetTransform(), CCollider::X_MIDDLE, CCollider::Y_MIDDLE, true);

	// Third person player
	m_char = new CThirdPerson();
	Camera3* view = new Camera3();
	view->Init(Vector3(0, 10, CThirdPerson::S_OFFSET_TARGET), Vector3(0, 10, 0), Vector3(0, 1, 0));
	transform = new CTransform();
	transform->Init(Vector3(0, 10, 10), Vector3(), Vector3(20,20,20));
	m_char->Init(view, m_meshList[MESH_CONE], transform);

	// Spatial partition
	m_spatialPartition = new CSpatialPartition();
	m_spatialPartition->Init(5, 5, 1000.f, 1000.f, m_meshList[MESH_CUBE]);

	m_camera = m_char->GetTPView();
}

void AGDev_Assign01::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(dt);

	m_fps = (float)(1.f / dt);
}

void AGDev_Assign01::Render()
{
	// Render SceneBase
	SceneBase::Render();

	CGameObject* target = new CGameObject();
	CTransform* transform = new CTransform();
	transform->Init(m_char->GetTPView()->target, Vector3(), Vector3(5, 5, 5));
	target->Init(m_meshList[MESH_CUBE], transform);
	RenderGameObject(target, m_lightEnabled);

	RenderSkybox();
	RenderGround();

	RenderGameObject(test, m_lightEnabled);
	RenderGameObject(m_char, m_lightEnabled);

	// Render spatial partition
	/*vector<CGrid*> gridList = m_spatialPartition->GetGridList();
	for (int i = 0; i < gridList.size(); ++i)
	{
		RenderGameObject(gridList[i], m_lightEnabled);
		//m_renderList.push(gridList[i]);
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

	// Delete sound
	sound->drop();
}

void AGDev_Assign01::Reset()
{
	SceneBase::Reset();
}

void AGDev_Assign01::ProcessKeys(double dt, bool* keys)
{
	if (keys[CGameStateManager::KEY_MOVE_FORWARD])
	{
		m_char->MoveForward(dt);
	}
	if (keys[CGameStateManager::KEY_MOVE_BACKWARD])
	{
		m_char->MoveBackward(dt);
	}
	if (keys[CGameStateManager::KEY_MOVE_LEFT])
	{
		m_char->MoveLeft(dt);
	}
	if (keys[CGameStateManager::KEY_MOVE_RIGHT])
	{
		m_char->MoveRight(dt);
	}
}

void AGDev_Assign01::ProcessMouse(double dt, float yaw, float pitch)
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

void AGDev_Assign01::initMesh()
{
	m_meshList[MESH_TEXT] = MeshBuilder::GenerateText("Calibri font", 16, 16);
	m_meshList[MESH_TEXT]->textureID[0] = LoadTGA("Image\\calibri.tga");
	m_meshList[MESH_CUBE] = MeshBuilder::GenerateCube("Cube", Color(0, 1, 0), 1.f);
	m_meshList[MESH_CONE] = MeshBuilder::GenerateCone("Cone", Color(1, 0, 0), 36, 0.5f, 1.f);

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