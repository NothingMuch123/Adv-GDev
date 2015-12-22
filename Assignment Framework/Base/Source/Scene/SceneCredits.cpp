#include "SceneCredits.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>

#include "..\Application.h"
#include "..\GameStateManager\GameStateManager.h"

SceneCredits::SceneCredits(int width, int height)
	: SceneBase(width, height)
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		m_meshList[i] = NULL;
	}
}

SceneCredits::~SceneCredits()
{
}

void SceneCredits::Init(int screenWidth, int screenHeight)
{
	Application::HideMouse(false);
	// Initialise SceneBase
	SceneBase::Init(screenWidth, screenHeight);

	// Init sound
	sound = irrklang::createIrrKlangDevice();
	if (!sound)
	{
		std::cout << "Could not start sound engine" << std::endl;
	}

	InitMesh();

	m_camera = new Camera3();
	m_camera->Init(Vector3(0, 0, 10), Vector3(), Vector3(0, 1, 0));

	initMenu();
	
}

void SceneCredits::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(GSM, dt);

	Vector2 mousePos = GSM->GetMousePos();
	m_menu->MouseUpdate(dt, mousePos.x, mousePos.y);
}

void SceneCredits::Render()
{

	// Render SceneBase
	SceneBase::Render();

	RenderGameObject(m_menu->GetCurrentMenu()->GetBackground(), m_lightEnabled, true);

	vector<Menu*> m_menuList = m_menu->GetMenuList();
	for (vector<Menu*>::iterator it = m_menuList.begin(); it != m_menuList.end(); ++it)
	{
		Menu* menu = *it;
		vector<UIButton*> m_buttonList = menu->GetButtonList();
		for (vector<UIButton*>::iterator it2 = m_buttonList.begin(); it2 != m_buttonList.end(); ++it2)
		{
			UIButton* button = *it2;
			RenderGameObject(button, m_lightEnabled, true);
		}
	}
	
	ostringstream text;
	text << "Done by me";
	RenderTextOnScreen(m_meshList[MESH_TEXT], text.str(), Color(1, 0, 0), 40, m_window_width * 0.5f - (text.str().length() * 0.5f * 40.f), m_window_height * 0.5f);

	ostringstream soundCredits;
	soundCredits << "BGM by Holfix";
	RenderTextOnScreen(m_meshList[MESH_TEXT], soundCredits.str(), Color(1, 0, 0), 40, m_window_width * 0.5f - (soundCredits.str().length() * 0.5f * 40.f), m_window_height * 0.5f - 40.f);

	// Not supposed to have any other rendering codes here as Scenebase handles it
	// Alternative solution is to render scenegraph here instead as render list does not take into account parent and child nodes
}

void SceneCredits::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

	// Delete sound
	sound->drop();

	m_menu->Clear();
	DestroyMesh();
}

void SceneCredits::Reset()
{
	SceneBase::Reset();
}

void SceneCredits::ProcessKeys(CGameStateManager* GSM, double dt, bool* keys, Vector2 mousePos)
{
	float clickTimer = GSM->GetClickTimer();
	float enterTimer = GSM->GetEnterTimer();

	if (clickTimer > 0.f)
	{
		clickTimer -= dt;
		GSM->SetClickTimer(clickTimer);
	}
	if (enterTimer > 0.f)
	{
		enterTimer -= dt;
		GSM->SetEnterTimer(enterTimer);
	}

	if (keys[CGameStateManager::KEY_SHOOT_1] && clickTimer <= 0.f)
	{
		m_menu->MouseUpdate(dt, mousePos.x, mousePos.y);
		m_menu->OnClick(GSM, mousePos.x, mousePos.y);
		GSM->SetClickTimer(CGameStateManager::S_CLICK_COOLDOWN);
	}

	if (keys[CGameStateManager::KEY_ENTER] && enterTimer <= 0.f)
	{
		m_menu->OnEnter(GSM);
		GSM->SetEnterTimer(CGameStateManager::S_ENTER_COOLDOWN);
	}

	if (keys[CGameStateManager::KEY_UP] && enterTimer <= 0.f)
	{
		m_menu->KeysUpdate(dt, false);
		GSM->SetEnterTimer(CGameStateManager::S_ENTER_COOLDOWN);
	}

	if (keys[CGameStateManager::KEY_DOWN] && enterTimer <= 0.f)
	{
		m_menu->KeysUpdate(dt, true);
		GSM->SetEnterTimer(CGameStateManager::S_ENTER_COOLDOWN);
	}
}

void SceneCredits::ProcessMouse(CGameStateManager* GSM, double dt, float yaw, float pitch, Vector2 mousePos)
{
}

void SceneCredits::UpdateCameraStatus(unsigned char key)
{
}

void SceneCredits::UpdateWeaponStatus(unsigned char key, double dt)
{

}

void SceneCredits::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
{
	if(m_bHUDmode)
	{
		glDisable(GL_DEPTH_TEST);
		Mtx44 ortho;
		ortho.SetToOrtho(0, m_window_width, 0, m_window_height, -10, 10); // Origin at bottom left
		projectionStack.PushMatrix();
		projectionStack.LoadMatrix(ortho);
		glUniform1f(m_parameters[U_FOG_ENABLE], 0);
	}
	else
	{
		projectionStack.PopMatrix();
		glEnable(GL_DEPTH_TEST);
		glUniform1f(m_parameters[U_FOG_ENABLE], fogEnabled);
	}
}

void SceneCredits::InitMesh()
{
	m_meshList[MESH_TEXT] = MeshBuilder::GenerateText("Calibri font", 16, 16);
	m_meshList[MESH_TEXT]->textureID[0] = LoadTGA("Image\\calibri.tga");

	// Menu
	m_meshList[MESH_RETURN_TO_MAIN_MENU_ON] = MeshBuilder::Generate2DMesh("New game on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_RETURN_TO_MAIN_MENU_ON]->textureID[0] = LoadTGA("Image\\Menu\\BackToMainMenu_On.tga");

	m_meshList[MESH_RETURN_TO_MAIN_MENU_OFF] = MeshBuilder::Generate2DMesh("New game off", Color(1,1,1), 0, 0, 1, 1);
	m_meshList[MESH_RETURN_TO_MAIN_MENU_OFF]->textureID[0] = LoadTGA("Image\\Menu\\BackToMainMenu_Off.tga");
}

void SceneCredits::DestroyMesh()
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		if (m_meshList[i])
		{
			delete m_meshList[i];
			m_meshList[i] = NULL;
		}
	}
}

void SceneCredits::initMenu()
{
	m_menu = new CCredits();

	int buttonCount = 0;
	const float HEIGHT_OFFSET = m_window_height * 0.1f;
	const Vector3 BUTTON_SIZE(m_window_width * 0.25f, m_window_height * 0.08f);

	// Menu creation
	Vector3 startPos = Vector3((m_window_width * 0.15f) - (BUTTON_SIZE.x * 0.5f), (m_window_height * 0.05f));// -(BUTTON_SIZE.y * 0.5f));

	Menu* newMenu = new Menu();
	newMenu->Init(Menu::MENU_CREDITS, NULL);
	// Button creation
	newMenu->AddButton(new UIButton(UIButton::BUTTON_RETURN_TO_MAIN_MENU, m_meshList[MESH_RETURN_TO_MAIN_MENU_OFF], m_meshList[MESH_RETURN_TO_MAIN_MENU_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;

	m_menu->AddMenu(newMenu);

	m_menu->AssignCurrent(Menu::MENU_CREDITS, UIButton::BUTTON_RETURN_TO_MAIN_MENU);
}
