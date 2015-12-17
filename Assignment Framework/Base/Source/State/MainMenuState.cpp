#include "MainMenuState.h"
#include "..\GameStateManager\GameStateManager.h"


CMainMenuState::CMainMenuState()
{
}


CMainMenuState::~CMainMenuState()
{
}

void CMainMenuState::Init(CGameStateManager * GSM, int width, int height)
{
	CGameState::Init(GSM, width, height);
	scene = new SceneMainMenu(width, height);
	scene->Init(width, height);
}

void CMainMenuState::Update(double dt)
{
	scene->Update(m_GSM, dt);
}

void CMainMenuState::Render()
{
	scene->Render();
}

void CMainMenuState::Exit()
{
	scene->Exit();
}

void CMainMenuState::Reset()
{
	scene->Reset();
}

void CMainMenuState::Pause()
{
}

void CMainMenuState::Resume()
{
}
void CMainMenuState::ProcessKeys(double dt, bool * keys)
{
	// TODO: Add codes to change to another state
	if (m_GSM->GetShouldRun())
	{
		scene->ProcessKeys(m_GSM, dt, keys, m_GSM->GetMousePos());

		float yaw = m_GSM->GetPendingYaw();
		float pitch = m_GSM->GetPendingPitch();
		scene->ProcessMouse(m_GSM, dt, yaw, pitch, m_GSM->GetMousePos());

		// Reset yaw and pitch after process
		m_GSM->SetPendingYaw(0.f);
		m_GSM->SetPendingPitch(0.f);
	}
}
