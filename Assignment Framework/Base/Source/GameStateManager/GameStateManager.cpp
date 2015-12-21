#include "GameStateManager.h"

bool CGameStateManager::S_MUSIC = true;
float CGameStateManager::S_HIGHSCORE = 0.f;
const float CGameStateManager::S_CLICK_COOLDOWN = 0.5f;
const float CGameStateManager::S_ENTER_COOLDOWN = 0.5f;

 CGameStateManager::CGameStateManager()
	: m_states(NULL)
	, m_running(true)
	, m_window_width(800)
	, m_window_height(600)
	, m_mousePos()
	, m_shouldRun(true)
	, m_clickTimer(S_CLICK_COOLDOWN)
	, m_enterTimer(S_ENTER_COOLDOWN)
{
	resetKeys();
}

CGameStateManager::~CGameStateManager()
{
	CleanUp();
}

void CGameStateManager::Init(int width, int height)
{
	m_window_width = width;
	m_window_height = height;
}

void CGameStateManager::CleanUp()
{
	while (m_states.size() > 0)
	{
		if (m_states.back())
		{
			delete m_states.back();
			m_states.pop_back();
		}
	}
}

void CGameStateManager::ChangeState(CGameState * newState)
{
	if (!m_states.empty())
	{
		m_states.back()->Exit();
		m_states.pop_back();
	}

	// Store and init new state
	m_states.push_back(newState);
	m_states.back()->Init(this, m_window_width, m_window_height);
	resetInputData();
	m_shouldRun = false;
}

void CGameStateManager::PushState(CGameState * newState)
{
	if (!m_states.empty()) // There is a state currently running
	{
		m_states.back()->Pause();
	}
	m_states.back()->Exit();
	m_states.push_back(newState);
	m_states.back()->Init(this, m_window_width, m_window_height);
	resetInputData();
	m_shouldRun = false;
}

void CGameStateManager::PopState()
{
	if (!m_states.empty()) // Check for states before deleting
	{
		m_states.back()->Exit();
		m_states.pop_back();
	}
	resetInputData();

	if (!m_states.empty()) // Resume any state that is paused
	{
		//m_states.back()->Resume();
		m_states.back()->Init(this, m_window_width, m_window_height);
	}
	else
	{
		Quit();
	}
	m_shouldRun = false;
}

void CGameStateManager::HandleEvents(const KEYS key, const bool status)
{
	m_keys[key] = status;
}

void CGameStateManager::HandleEvents(const double yaw, const double pitch)
{
	m_pending_pitch = pitch;
	m_pending_yaw = yaw;
}

void CGameStateManager::Update(const double dt)
{
	m_shouldRun = true;
	if (!m_states.empty())
	{
		m_states.back()->ProcessKeys(dt, m_keys);
		if (m_shouldRun)
		{
			resetKeys();
			m_states.back()->Update(dt);
		}
	}
}

void CGameStateManager::Draw()
{
	if (!m_states.empty() && m_shouldRun)
	{
		m_states.back()->Render();
	}
}

bool CGameStateManager::isRunning()
{
	return m_running;
}

void CGameStateManager::Quit()
{
	m_running = false;
}

void CGameStateManager::SetPendingYaw(float pending_yaw)
{
	m_pending_yaw = pending_yaw;
}

float CGameStateManager::GetPendingYaw()
{
	return m_pending_yaw;
}

void CGameStateManager::SetPendingPitch(float pending_pitch)
{
	m_pending_pitch = pending_pitch;
}

float CGameStateManager::GetPendingPitch()
{
	return m_pending_pitch;
}

void CGameStateManager::SetShouldRun(bool shouldRun)
{
	m_shouldRun = shouldRun;
}

bool CGameStateManager::GetShouldRun()
{
	return m_shouldRun;
}

void CGameStateManager::SetMousePos(float x, float y)
{
	m_mousePos.x = x;
	m_mousePos.y = y;
}

Vector2 CGameStateManager::GetMousePos()
{
	return m_mousePos;
}

void CGameStateManager::SetEnterTimer(float enterTimer)
{
	m_enterTimer = enterTimer;
}

float CGameStateManager::GetEnterTimer()
{
	return m_enterTimer;
}

void CGameStateManager::SetClickTimer(float clickTimer)
{
	m_clickTimer = clickTimer;
}

float CGameStateManager::GetClickTimer()
{
	return m_clickTimer;
}

void CGameStateManager::resetKeys()
{
	for (int i = 0; i < NUM_KEY; ++i)
	{
		m_keys[i] = false;
	}
}

void CGameStateManager::resetInputData()
{
	resetKeys();
	m_pending_pitch = m_pending_yaw = 0.f;
	m_clickTimer = S_CLICK_COOLDOWN;
	m_enterTimer = S_ENTER_COOLDOWN;
}
