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

	m_char = new CThirdPerson();
	Camera3* view = new Camera3();
	view->Init(Vector3(0, 0, 10), Vector3(0, 0, 0), Vector3(0, 1, 0));
	transform = new CTransform();
	transform->Init(Vector3(0, 0, 10), Vector3(), Vector3(8,8,8));
	m_char->Init(view, m_meshList[MESH_CONE], transform);

	m_camera = m_char->GetTPView();
}

void AGDev_Assign01::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(dt);

	m_camera->Update(dt);

	m_renderList.push(test);
	m_renderList.push(m_char);

	m_fps = (float)(1.f / dt);
}

void AGDev_Assign01::Render()
{
	// Render SceneBase
	SceneBase::Render();


	SetHUD(true);
	ostringstream sFPS;
	sFPS << "FPS: " << m_fps;
	RenderTextOnScreen(m_meshList[MESH_TEXT], sFPS.str(), Color(0, 1, 0), 20, 0, 0);
	SetHUD(false);

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

void AGDev_Assign01::ProcessKeys(bool* keys)
{
	if (keys[CGameStateManager::KEY_MOVE_FORWARD])
	{
		m_camera->UpdateStatus('w');
	}
	if (keys[CGameStateManager::KEY_MOVE_BACKWARD])
	{
		m_camera->UpdateStatus('s');
	}
	if (keys[CGameStateManager::KEY_MOVE_LEFT])
	{
		m_camera->UpdateStatus('a');
	}
	if (keys[CGameStateManager::KEY_MOVE_RIGHT])
	{
		m_camera->UpdateStatus('d');
	}
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
}
