#ifndef APPLICATION_H
#define APPLICATION_H

#include "timer.h"
#include "Scene\AGDev_Assign01.h"
#include "GameStateManager\GameStateManager.h"

// Lua wrapper
#include "Lua\Lua_Wrapper.h"

class Application
{
public:
	static Application& GetInstance()
	{
		static Application app;
		return app;
	}
	void Init();
	void Run();
	void Exit();
	static bool IsKeyPressed(unsigned short key);
	bool GetMouseUpdate(double dt);
	bool GetKeyBoardUpdate();

	static void HideMouse(bool hide);

	static double mouse_last_x, mouse_last_y, mouse_current_x, mouse_current_y, mouse_diff_x, mouse_diff_y;
	static double camera_yaw, camera_pitch;

private:
	Application();
	~Application();

	bool loadlua();

	const static int m_window_deadzone = 100;
	static int m_window_width;
	static int m_window_height;

	//Declare a window object
	StopWatch m_timer;
	double m_dElapsedTime;
	double m_dAccumulatedTime_ThreadOne;
	double m_dAccumulatedTime_ThreadTwo;

	static bool m_hideMouse;
	//Scene
	//AGDev_Assign01 *scene;

	// Game state manager
	CGameStateManager* m_GSM;
};

#endif