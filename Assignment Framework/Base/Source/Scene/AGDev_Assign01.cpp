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
{
}

AGDev_Assign01::~AGDev_Assign01()
{
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