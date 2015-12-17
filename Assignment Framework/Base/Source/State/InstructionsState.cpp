#include "InstructionsState.h"
#include "..\GameStateManager\GameStateManager.h"


CInstructionsState::CInstructionsState()
{
}


CInstructionsState::~CInstructionsState()
{
}

void CInstructionsState::Init(CGameStateManager * GSM, int width, int height)
{
	CGameState::Init(GSM, width, height);
	scene = new SceneInstructions(width, height);
	scene->Init(width, height);
}

void CInstructionsState::Update(double dt)
{
	scene->Update(m_GSM, dt);
}

void CInstructionsState::Render()
{
	scene->Render();
}

void CInstructionsState::Exit()
{
	scene->Exit();
}

void CInstructionsState::Reset()
{
	scene->Reset();
}

void CInstructionsState::Pause()
{
}

void CInstructionsState::Resume()
{
}

void CInstructionsState::ProcessKeys(double dt, bool * keys)
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

SceneInstructions * CInstructionsState::GetScene()
{
	return scene;
}
