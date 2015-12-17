#include "PlayState.h"
#include "..\GameStateManager\GameStateManager.h"


CPlayState::CPlayState()
{
}


CPlayState::~CPlayState()
{
}

void CPlayState::Init(CGameStateManager * GSM, int width, int height)
{
	CGameState::Init(GSM, width, height);
	scene = new AGDev_Assign01(width, height);
	scene->Init(width, height);
}

void CPlayState::Update(double dt)
{
	scene->Update(m_GSM, dt);
}

void CPlayState::Render()
{
	scene->Render();
}

void CPlayState::Exit()
{
	scene->Exit();
}

void CPlayState::Reset()
{
	scene->Reset();
}

void CPlayState::Pause()
{
}

void CPlayState::Resume()
{
}

void CPlayState::ProcessKeys(double dt, bool * keys)
{
	// TODO: Add codes to change to another state
	scene->ProcessKeys(m_GSM, dt, keys, m_GSM->GetMousePos());

	float yaw = m_GSM->GetPendingYaw();
	float pitch = m_GSM->GetPendingPitch();
	if (yaw != 0.f || pitch != 0.f)
	{
		scene->ProcessMouse(m_GSM, dt, yaw, pitch, m_GSM->GetMousePos());
	}

	// Reset yaw and pitch after process
	m_GSM->SetPendingYaw(0.f);
	m_GSM->SetPendingPitch(0.f);
}
