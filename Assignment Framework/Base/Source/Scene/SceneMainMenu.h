#ifndef _TEMPLATE_SCENE_H_
#define _TEMPLATE_SCENE_H_

#include "..\SceneBase.h"
#include "..\MenuManager\MainMenu.h"

class CGameStateManager;

class SceneMainMenu : public SceneBase
{
public:
	enum MESH_TYPE
	{
		MESH_TEXT = 0,

		// Menus
		MESH_MAINMENU_BG,
		MESH_NEWGAME_ON,
		MESH_NEWGAME_OFF,
		MESH_INSTRUCTION_ON,
		MESH_INSTRUCTION_OFF,
		MESH_OPTIONS_ON,
		MESH_OPTIONS_OFF,
		MESH_CREDITS_ON,
		MESH_CREDITS_OFF,
		MESH_EXIT_ON,
		MESH_EXIT_OFF,
		
		MESH_HIGHSCORE,

		NUM_MESH,
	};
	SceneMainMenu(int width = 1280, int height = 720);
	virtual ~SceneMainMenu();

	// Functions from scenebase, do not add parameters else override will not work
	virtual void Init(int width, int height);
	virtual void Update(CGameStateManager* GSM, double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void ProcessKeys(CGameStateManager* GSM, double dt, bool* keys, Vector2 mousePos);
	virtual void ProcessMouse(CGameStateManager* GSM, double dt, float yaw, float pitch, Vector2 mousePos);

	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key, double dt = 0.0);

	void SetHUD(const bool m_bHUDmode);
	void InitMesh();
	void DestroyMesh();

private:
	void initMenu();

private:
	Mesh* m_meshList[NUM_MESH];
	CMainMenu* m_menu;

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif