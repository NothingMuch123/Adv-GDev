#include "OptionsState.h"
#include "..\GameStateManager\GameStateManager.h"


COptionsState::COptionsState()
{
}


COptionsState::~COptionsState()
{
}

void COptionsState::Init(CGameStateManager * GSM, int width, int height)
{
	CGameState::Init(GSM, width, height);
	scene = new SceneOptions(width, height);
	scene->Init(width, height);
}

void COptionsState::Update(double dt)
{
	scene->Update(m_GSM, dt);
}

void COptionsState::Render()
{
	scene->Render();
}

void COptionsState::Exit()
{
	scene->Exit();
}

void COptionsState::Reset()
{
	scene->Reset();
}

void COptionsState::Pause()
{
}

void COptionsState::Resume()
{
}

void COptionsState::ProcessKeys(double dt, bool * keys)
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
