#include "TemplateState.h"
#include "..\GameStateManager\GameStateManager.h"


CTemplateState::CTemplateState()
{
}


CTemplateState::~CTemplateState()
{
}

void CTemplateState::Init(CGameStateManager * GSM, int width, int height)
{
	CGameState::Init(GSM, width, height);
	scene = new SceneBase(width, height);
	scene->Init(width, height);
}

void CTemplateState::Update(double dt)
{
	scene->Update(m_GSM, dt);
}

void CTemplateState::Render()
{
	scene->Render();
}

void CTemplateState::Exit()
{
	scene->Exit();
}

void CTemplateState::Reset()
{
	scene->Reset();
}

void CTemplateState::Pause()
{
}

void CTemplateState::Resume()
{
}

void CTemplateState::ProcessKeys(double dt, bool * keys)
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
