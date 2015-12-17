#ifndef _TEMPLATE_SCENE_H_
#define _TEMPLATE_SCENE_H_

#include "..\SceneBase.h"

class CGameStateManager;

class Template_Scene : public SceneBase
{
public:
	enum MESH_TYPE
	{
		MESH_TEXT = 0,

		NUM_MESH,
	};
	Template_Scene(int width = 1280, int height = 720);
	virtual ~Template_Scene();

	// Functions from scenebase, do not add parameters else override will not work
	virtual void Init(int width, int height);
	virtual void Update(CGameStateManager* GSM, double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void ProcessKeys(double dt, bool* keys, Vector2 mousePos);
	virtual void ProcessMouse(double dt, float yaw, float pitch, Vector2 mousePos);

	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key, double dt = 0.0);

	void SetHUD(const bool m_bHUDmode);

private:
	void InitMesh();

private:
	Mesh* m_meshList[NUM_MESH];

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif