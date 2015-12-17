#include "CreditsState.h"
#include "..\GameStateManager\GameStateManager.h"


CCreditsState::CCreditsState()
{
}


CCreditsState::~CCreditsState()
{
}

void CCreditsState::Init(CGameStateManager * GSM, int width, int height)
{
	CGameState::Init(GSM, width, height);
	scene = new SceneCredits(width, height);
	scene->Init(width, height);
}

void CCreditsState::Update(double dt)
{
	scene->Update(m_GSM, dt);
}

void CCreditsState::Render()
{
	scene->Render();
}

void CCreditsState::Exit()
{
	scene->Exit();
}

void CCreditsState::Reset()
{
	scene->Reset();
}

void CCreditsState::Pause()
{
}

void CCreditsState::Resume()
{
}

void CCreditsState::ProcessKeys(double dt, bool * keys)
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
