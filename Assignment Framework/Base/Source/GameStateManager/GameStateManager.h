#ifndef _GAMESTATEMANAGER_H_
#define _GAMESTATEMANAGER_H_

#include <vector>
#include "..\State\GameState.h"
#include "..\State\PlayState.h"
#include "Vector2.h"

using std::vector;

class CGameStateManager
{
public:
	enum KEYS
	{
		KEY_MOVE_FORWARD = 0,
		KEY_MOVE_BACKWARD,
		KEY_MOVE_LEFT,
		KEY_MOVE_RIGHT,

		KEY_SHOOT_1, // Left mouse shoot
		KEY_SHOOT_2, // Right mouse shoot
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

	void SetMousePos(float x, float y);
	Vector2 GetMousePos();

private:
	void resetKeys();
	void resetInputData();

	vector<CGameState*> m_states; // Works in a form of a stack

	bool m_keys[NUM_KEY]; // Stores all inputs (Keyboard and mouse)
	Vector2 m_mousePos;
	float m_pending_yaw, m_pending_pitch; // Yaw and pitch that are pending for changes

	bool m_running; // Determines if any states are currently running

	// Window width and height
	int m_window_width, m_window_height;
};

#endif