#include "GameState.h"
#include "..\GameStateManager\GameStateManager.h"


CGameState::CGameState()
	: m_pause(false)
{
}


CGameState::~CGameState()
{
}

void CGameState::Init(CGameStateManager* gsm, int width, int height)
{
	m_GSM = gsm;
}

void CGameState::Update(double dt)
{
}

void CGameState::Render()
{
}

void CGameState::Exit()
{
}

void CGameState::Reset()
{
}

void CGameState::Pause()
{
	m_pause = true;
}

void CGameState::Resume()
{
	m_pause = false;
}

void CGameState::ProcessKeys(bool* keys)
{
}
