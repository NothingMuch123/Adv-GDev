#ifndef _GAMESTATEMANAGER_H_
#define _GAMESTATEMANAGER_H_

#include <vector>
#include "..\State\GameState.h"
#include "..\State\PlayState.h"
#include "..\State\MainMenuState.h"
#include "..\State\InstructionsState.h"
#include "..\State\CreditsState.h"
#include "..\State\OptionsState.h"
#include "Vector2.h"

using std::vector;

class CGameStateManager
{
public:
	// Menus
	static const float S_CLICK_COOLDOWN;
	static const float S_ENTER_COOLDOWN;
	static bool S_LOAD;
	static bool S_MUSIC;
	static float S_HIGHSCORE;

	enum KEYS
	{
		KEY_MOVE_FORWARD = 0,
		KEY_MOVE_BACKWARD,
		KEY_MOVE_LEFT,
		KEY_MOVE_RIGHT,
		KEY_MOVE_UP,
		KEY_MOVE_DOWN,

		KEY_SPRINT,

		KEY_SHOOT_1, // Left mouse shoot
		KEY_SHOOT_2, // Right mouse shoot

		// Arrow keys
		KEY_UP,
		KEY_DOWN,

		KEY_ENTER,

		KEY_P,

		NUM_KEY,
	};
	CGameStateManager();
	~CGameStateManager();

	void Init(int width = 800, int height = 600);
	void CleanUp();

	void ChangeState(CGameState* newState);
	void PushState(CGameState* newState);
	void PopState();

	void HandleEvents(const KEYS key, const bool status = true); // Input updates
	void HandleEvents(const double yaw, const double pitch); // Mouse updates | Yaw is left and right | Pitch is up and down

	void Update(const double dt);
	void Draw();

	bool isRunning();
	void Quit();

	void SetPendingYaw(float pending_yaw);
	float GetPendingYaw();
	void SetPendingPitch(float pending_pitch);
	float GetPendingPitch();

	void SetShouldRun(bool shouldRun);
	bool GetShouldRun();

	void SetMousePos(float x, float y);
	Vector2 GetMousePos();

	void SetEnterTimer(float enterTimer);
	float GetEnterTimer();

	void SetClickTimer(float clickTimer);
	float GetClickTimer();

private:
	void resetKeys();
	void resetInputData();

	vector<CGameState*> m_states; // Works in a form of a stack

	bool m_keys[NUM_KEY]; // Stores all inputs (Keyboard and mouse)
	Vector2 m_mousePos;
	float m_pending_yaw, m_pending_pitch; // Yaw and pitch that are pending for changes

	bool m_running; // Determines if any states are currently running
	bool m_shouldRun; // Determines if the state should run (Toggle when changing state to stop all function calls)

	// Window width and height
	int m_window_width, m_window_height;

	// Mouse and key press timer (For menus)
	float m_enterTimer;
	float m_clickTimer;
};

#endif