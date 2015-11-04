
#include "Application.h"

//Include GLEW
#include <GL/glew.h>

//Include GLFW
#include <GLFW/glfw3.h>

//Include the standard C++ headers
#include <stdio.h>
#include <stdlib.h>

GLFWwindow* m_window;
const unsigned char FPS = 120; // FPS of this game
const unsigned int frameTime = 1000 / FPS; // time for each frame

double Application::mouse_last_x = 0.0, Application::mouse_last_y = 0.0, Application::mouse_current_x = 0.0, Application::mouse_current_y = 0.0, Application::mouse_diff_x = 0.0, Application::mouse_diff_y = 0.0;
double Application::camera_yaw = 0.0, Application::camera_pitch = 0.0;

//Define an error callback
static void error_callback(int error, const char* description)
{
	fputs(description, stderr);
	_fgetchar();
}

//Define the key input callback
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void resize_callback(GLFWwindow* window, int w, int h)
{
	glViewport(0, 0, w, h);
}

bool Application::IsKeyPressed(unsigned short key)
{
    return ((GetAsyncKeyState(key) & 0x8001) != 0);
}

Application::Application()
{
}

Application::~Application()
{
}

void Application::Init()
{
	//Set the error callback
	glfwSetErrorCallback(error_callback);

	//Initialize GLFW
	if (!glfwInit())
	{
		exit(EXIT_FAILURE);
	}

	//Set the GLFW window creation hints - these are optional
	glfwWindowHint(GLFW_SAMPLES, 4); //Request 4x antialiasing
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3); //Request a specific OpenGL version
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3); //Request a specific OpenGL version
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); //We don't want the old OpenGL 


	//Create a window and create its OpenGL context
	//m_window = glfwCreateWindow(1920, 1080, "DM2231_Framework", NULL/*glfwGetPrimaryMonitor()*/, NULL);
	m_window = glfwCreateWindow(m_window_width, m_window_height, "DM2231_Framework", NULL, NULL);

	//If the window couldn't be created
	if (!m_window)
	{
		fprintf( stderr, "Failed to open GLFW window.\n" );
		glfwTerminate();
		exit(EXIT_FAILURE);
	}

	//This function makes the context of the specified window current on the calling thread. 
	glfwMakeContextCurrent(m_window);

	//Sets the key callback
	//glfwSetKeyCallback(m_window, key_callback);
	glfwSetWindowSizeCallback(m_window, resize_callback);

	glewExperimental = true; // Needed for core profile
	//Initialize GLEW
	GLenum err = glewInit();

	//If GLEW hasn't initialized
	if (err != GLEW_OK) 
	{
		fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
		//return -1;
	}

	// Hide cursor
	glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

	// Variables
	m_dElapsedTime = m_dAccumulatedTime_ThreadOne = m_dAccumulatedTime_ThreadTwo = 0.0;
}

void Application::Run()
{
	//Main Loop
	scene = new GDev_Assignment01();
	scene->Init();

	m_timer.startTimer();    // Start timer to calculate how long it takes to render this frame
	while (!glfwWindowShouldClose(m_window) && !IsKeyPressed(VK_ESCAPE))
	{
		// Get elapsed time
		m_dElapsedTime = m_timer.getElapsedTime();
		m_dAccumulatedTime_ThreadOne += m_dElapsedTime;
		m_dAccumulatedTime_ThreadTwo += m_dElapsedTime;
		// Player's update
		if (m_dAccumulatedTime_ThreadOne > 0.01)
		{
			GetMouseUpdate(m_dElapsedTime);
			GetKeyBoardUpdate();
			m_dAccumulatedTime_ThreadOne = 0.0;
		}
		// Scene update
		if (m_dAccumulatedTime_ThreadTwo > 0.01)
		{
			scene->Update(m_dElapsedTime);
			m_dAccumulatedTime_ThreadTwo = 0.0;
		}
		scene->Render();
		//Swap buffers
		glfwSwapBuffers(m_window);
		//Get and organize events, like keyboard and mouse input, window resizing, etc...
		glfwPollEvents();
        m_timer.waitUntil(frameTime);       // Frame rate limiter. Limits each frame to a specified time in ms.   

	} //Check if the ESC key had been pressed or if the window had been closed
	scene->Exit();
	delete scene;
}

void Application::Exit()
{
	//Close OpenGL window and terminate GLFW
	glfwDestroyWindow(m_window);
	//Finalize and clean up GLFW
	glfwTerminate();
}

bool Application::GetMouseUpdate(double dt)
{
	glfwGetCursorPos(m_window, &mouse_current_x, &mouse_current_y);

	// Calculate the difference in positions
	mouse_diff_x = mouse_current_x - mouse_last_x;
	mouse_diff_y = mouse_current_y - mouse_last_y;
	
	static bool firstFrame = true;
	if (firstFrame)
	{
		mouse_diff_x = mouse_diff_y = 0;
		firstFrame = false;
	}

	// Calculate the yaw and pitch
	camera_yaw = (float) mouse_diff_x * 0.174555555555556f;	// 3.142f / 180.0f
	camera_pitch = mouse_diff_y * 0.174555555555556f;		// 3.142f / 180.0f

	// Do a wraparound if the mouse cursor has gone out of the deadzone
	if ((mouse_current_x < m_window_deadzone) || (mouse_current_x > m_window_width - m_window_deadzone))
	{
		mouse_current_x = m_window_width >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}
	if ((mouse_current_y < m_window_deadzone) || (mouse_current_y > m_window_height - m_window_deadzone))
	{
		mouse_current_y = m_window_height >> 1;
		glfwSetCursorPos(m_window, mouse_current_x, mouse_current_y);
	}

	// Store the current position as the last position
	mouse_last_x = mouse_current_x;
	mouse_last_y = mouse_current_y;

	if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		scene->UpdateWeaponStatus(GDev_Assignment01::WA_FIRE, dt);
	}
	if(glfwGetMouseButton(m_window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		scene->UpdateWeaponStatus(GDev_Assignment01::WA_SCOPE);
	}

	return false;
}

bool Application::GetKeyBoardUpdate()
{
	if (IsKeyPressed('W'))					// Walk forward
	{
		scene->UpdateCameraStatus('w');
	}
	if (IsKeyPressed('S'))					// Walk backward
	{
		scene->UpdateCameraStatus('s');
	}
	if (IsKeyPressed('A'))					// Strafe left
	{
		scene->UpdateCameraStatus('a');
	}
	if (IsKeyPressed('D'))					// Strafe right
	{
		scene->UpdateCameraStatus('d');
	}
	if (IsKeyPressed('Q'))					// Fly up
	{
		scene->UpdateCameraStatus('q');
	}
	if (IsKeyPressed('E'))					// Fly down
	{
		scene->UpdateCameraStatus('e');
	}
	if (IsKeyPressed('Z'))					// Swap previous weapon
	{
		scene->UpdateWeaponStatus(GDev_Assignment01::WA_CHANGEWEAPON_PREV);
	}
	if (IsKeyPressed('C'))					// Swap next weapon
	{
		scene->UpdateWeaponStatus(GDev_Assignment01::WA_CHANGEWEAPON_NEXT);
	}
	if (IsKeyPressed(VK_SPACE))				// Character jump
	{
		scene->UpdateCameraStatus(' ');
	}
	if (IsKeyPressed('R'))					// Reload weapon
	{
		scene->UpdateWeaponStatus(GDev_Assignment01::WA_RELOAD);
	}
	if (IsKeyPressed(VK_SHIFT))				// Character sprint
	{
		scene->UpdateCameraStatus(1);
	}
	if (IsKeyPressed(VK_CONTROL))			// Character crouch
	{
		scene->UpdateCameraStatus(2);
	}
	if (IsKeyPressed('T'))					// Reset scene
	{
		scene->UpdateCameraStatus('t');
	}
	return true;
}