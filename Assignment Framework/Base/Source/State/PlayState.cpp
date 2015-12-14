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

void CPlayState::ProcessKeys(bool * keys)
{
	// TODO: Add codes to change to another state
	scene->ProcessKeys(keys);
}
