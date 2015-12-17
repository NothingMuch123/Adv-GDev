#include "SceneMainMenu.h"
#include "GL\glew.h"

#include "..\shader.hpp"
#include "..\MeshBuilder.h"
#include "..\Utility.h"
#include "..\LoadTGA.h"
#include <sstream>

#include "..\Application.h"
#include "..\GameStateManager\GameStateManager.h"

SceneMainMenu::SceneMainMenu(int width, int height)
	: SceneBase(width, height)
{
	for (int i = 0; i < NUM_MESH; ++i)
	{
		m_meshList[i] = NULL;
	}
}

SceneMainMenu::~SceneMainMenu()
{
}

void SceneMainMenu::Init(int screenWidth, int screenHeight)
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

void SceneMainMenu::Update(CGameStateManager* GSM, double dt)
{
	// Update SceneBase
	SceneBase::Update(GSM, dt);

	Vector2 mousePos = GSM->GetMousePos();
	m_menu->MouseUpdate(dt, mousePos.x, mousePos.y);
}

void SceneMainMenu::Render()
{
	// Render SceneBase
	SceneBase::Render();

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

	ostringstream sFPS;
	sFPS << "FPS: " << m_fps;
	RenderTextOnScreen(m_meshList[MESH_TEXT], sFPS.str(), Color(1, 0, 0), 50, 0, 0);

	RenderGameObject(m_menu->GetCurrentMenu()->GetBackground(), m_lightEnabled, true);

	// Not supposed to have any other rendering codes here as Scenebase handles it
	// Alternative solution is to render scenegraph here instead as render list does not take into account parent and child nodes
}

void SceneMainMenu::Exit()
{
	// Exit SceneBase
	SceneBase::Exit();

	// Delete sound
	sound->drop();

	DestroyMesh();
	m_menu->Clear();
}

void SceneMainMenu::Reset()
{
	SceneBase::Reset();
}

void SceneMainMenu::ProcessKeys(CGameStateManager* GSM, double dt, bool* keys, Vector2 mousePos)
{
	if (keys[CGameStateManager::KEY_SHOOT_1])
	{
		m_menu->MouseUpdate(dt, mousePos.x, mousePos.y);
		MenuManager::E_RETURN_STATE result = m_menu->OnClick(GSM, mousePos.x, mousePos.y);
		if (result == MenuManager::RS_CHANGE)
		{
			return;
		}
	}

	if (keys[CGameStateManager::KEY_ENTER])
	{
		MenuManager::E_RETURN_STATE result = m_menu->OnEnter(GSM);
		if (result == MenuManager::RS_CHANGE)
		{
			return;
		}
	}

	if (keys[CGameStateManager::KEY_UP])
	{
		m_menu->KeysUpdate(dt, false);
	}

	if (keys[CGameStateManager::KEY_DOWN])
	{
		m_menu->KeysUpdate(dt, true);
	}
}

void SceneMainMenu::ProcessMouse(CGameStateManager* GSM, double dt, float yaw, float pitch, Vector2 mousePos)
{
}

void SceneMainMenu::UpdateCameraStatus(unsigned char key)
{
}

void SceneMainMenu::UpdateWeaponStatus(unsigned char key, double dt)
{

}

void SceneMainMenu::SetHUD(const bool m_bHUDmode) // Remember to call this in pairs
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

void SceneMainMenu::InitMesh()
{
	m_meshList[MESH_TEXT] = MeshBuilder::GenerateText("Calibri font", 16, 16);
	m_meshList[MESH_TEXT]->textureID[0] = LoadTGA("Image\\calibri.tga");

	// Menu
	m_meshList[MESH_MAINMENU_BG] = MeshBuilder::Generate2DMesh("New game on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_MAINMENU_BG]->textureID[0] = LoadTGA("Image\\Menu\\MenuScreens\\main_menu_bg.tga");

	m_meshList[MESH_NEWGAME_ON] = MeshBuilder::Generate2DMesh("New game on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_NEWGAME_ON]->textureID[0] = LoadTGA("Image\\Menu\\NewGame_On.tga");

	m_meshList[MESH_NEWGAME_OFF] = MeshBuilder::Generate2DMesh("New game off", Color(1,1,1), 0, 0, 1, 1);
	m_meshList[MESH_NEWGAME_OFF]->textureID[0] = LoadTGA("Image\\Menu\\NewGame_Off.tga");

	m_meshList[MESH_INSTRUCTION_ON] = MeshBuilder::Generate2DMesh("Instruction on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_INSTRUCTION_ON]->textureID[0] = LoadTGA("Image\\Menu\\Instructions_On.tga");

	m_meshList[MESH_INSTRUCTION_OFF] = MeshBuilder::Generate2DMesh("Instruction off", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_INSTRUCTION_OFF]->textureID[0] = LoadTGA("Image\\Menu\\Instructions_Off.tga");

	m_meshList[MESH_OPTIONS_ON] = MeshBuilder::Generate2DMesh("Options on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_OPTIONS_ON]->textureID[0] = LoadTGA("Image\\Menu\\Options_On.tga");

	m_meshList[MESH_OPTIONS_OFF] = MeshBuilder::Generate2DMesh("Options off", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_OPTIONS_OFF]->textureID[0] = LoadTGA("Image\\Menu\\Options_Off.tga");

	m_meshList[MESH_CREDITS_ON] = MeshBuilder::Generate2DMesh("Credits on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_CREDITS_ON]->textureID[0] = LoadTGA("Image\\Menu\\Credits_On.tga");

	m_meshList[MESH_CREDITS_OFF] = MeshBuilder::Generate2DMesh("Credits off", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_CREDITS_OFF]->textureID[0] = LoadTGA("Image\\Menu\\Credits_Off.tga");

	m_meshList[MESH_EXIT_ON] = MeshBuilder::Generate2DMesh("Exit on", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_EXIT_ON]->textureID[0] = LoadTGA("Image\\Menu\\Exit_On.tga");

	m_meshList[MESH_EXIT_OFF] = MeshBuilder::Generate2DMesh("Exit off", Color(1, 1, 1), 0, 0, 1, 1);
	m_meshList[MESH_EXIT_OFF]->textureID[0] = LoadTGA("Image\\Menu\\Exit_Off.tga");
}

void SceneMainMenu::DestroyMesh()
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

void SceneMainMenu::initMenu()
{
	m_menu = new CMainMenu();

	int buttonCount = 0;
	const float HEIGHT_OFFSET = m_window_height * 0.1f;
	const Vector3 BUTTON_SIZE(m_window_width * 0.25f, m_window_height * 0.08f);

	// Menu creation
	Vector3 startPos = Vector3((m_window_width * 0.75f) - (BUTTON_SIZE.x * 0.5f), (m_window_height * 0.5f) - (BUTTON_SIZE.y * 0.5f));

	Menu* newMenu = new Menu();
	newMenu->Init(Menu::MENU_MAIN, m_meshList[MESH_MAINMENU_BG], Vector3(), Vector3(m_window_width, m_window_height));
	// Button creation
	newMenu->AddButton(new UIButton(UIButton::BUTTON_NEW_GAME, m_meshList[MESH_NEWGAME_OFF], m_meshList[MESH_NEWGAME_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;

	/*newMenu->AddButton(new UIButton(UIButton::BUTTON_LEVEL_SELECT, m_meshList[MESH_LEVEL_SELECT_OFF], m_meshList[MESH_LEVEL_SELECT_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;*/

	newMenu->AddButton(new UIButton(UIButton::BUTTON_INSTRUCTIONS, m_meshList[MESH_INSTRUCTION_OFF], m_meshList[MESH_INSTRUCTION_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;

	newMenu->AddButton(new UIButton(UIButton::BUTTON_OPTIONS, m_meshList[MESH_OPTIONS_OFF], m_meshList[MESH_OPTIONS_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;

	newMenu->AddButton(new UIButton(UIButton::BUTTON_CREDITS, m_meshList[MESH_CREDITS_OFF], m_meshList[MESH_CREDITS_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;

	newMenu->AddButton(new UIButton(UIButton::BUTTON_EXIT, m_meshList[MESH_EXIT_OFF], m_meshList[MESH_EXIT_ON], startPos - Vector3(0.f, HEIGHT_OFFSET * buttonCount), BUTTON_SIZE));
	++buttonCount;

	m_menu->AddMenu(newMenu);

	m_menu->AssignCurrent(Menu::MENU_MAIN, UIButton::BUTTON_NEW_GAME);
}
