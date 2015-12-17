#include "Template_Scene.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>

#include "..\GameStateManager\GameStateManager.h"

Template_Scene::Template_Scene(int width, int height)
	: SceneBase(width, height)
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		m_meshList[i] = NULL;
	}
}

Template_Scene::~Template_Scene()
{
}

void Template_Scene::Init(int screenWidth, int screenHeight)
{
	// Initialise SceneBase
	SceneBase::Init(screenWidth, screenHeight);

	// Init sound
	sound = irrklang::createIrrKlangDevice();
	if (!sound)
	{
		std::cout << "Could not start sound engine" << std::endl;
	}

	InitMesh();

	m_camera = new Camera3();
	m_camera->Init(Vector3(0, 0, 10), Vector3(), Vector3(0, 1, 0));
}

void Template_Scene::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(GSM, dt);
}

void Template_Scene::Render()
{
	// Render SceneBase
	SceneBase::Render();

	ostringstream sFPS;
	sFPS << "FPS: " << m_fps;
	RenderTextOnScreen(m_meshList[MESH_TEXT], sFPS.str(), Color(1, 0, 0), 50, 0, 0);

	// Not supposed to have any other rendering codes here as Scenebase handles it
	// Alternative solution is to render scenegraph here instead as render list does not take into account parent and child nodes
}

void Template_Scene::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

	// Delete sound
	sound->drop();
}

void Template_Scene::Reset()
{
	SceneBase::Reset();
}

void Template_Scene::ProcessKeys(double dt, bool* keys, Vector2 mousePos)
{
}

void Template_Scene::ProcessMouse(double dt, float yaw, float pitch, Vector2 mousePos)
{
}

void Template_Scene::UpdateCameraStatus(unsigned char key)
{
}

void Template_Scene::UpdateWeaponStatus(unsigned char key, double dt)
{

}

void Template_Scene::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
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

void Template_Scene::InitMesh()
{
	m_meshList[MESH_TEXT] = MeshBuilder::GenerateText("Calibri font", 16, 16);
	m_meshList[MESH_TEXT]->textureID[0] = LoadTGA("Image\\calibri.tga");
}