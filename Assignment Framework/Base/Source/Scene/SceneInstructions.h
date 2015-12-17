#ifndef _SCENE_INSTRUCTIONS_H_
#define _SCENE_INSTRUCTIONS_H_

#include "..\SceneBase.h"
#include "..\MenuManager\Instructions.h"

class CGameStateManager;

class SceneInstructions : public SceneBase
{
public:
	enum MESH_TYPE
	{
		MESH_TEXT = 0,

		// Menus
		MESH_RETURN_TO_MAIN_MENU_ON,
		MESH_RETURN_TO_MAIN_MENU_OFF,

		NUM_MESH,
	};
	SceneInstructions(int width = 1280, int height = 720);
	virtual ~SceneInstructions();

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
	CInstructions* m_menu;

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif