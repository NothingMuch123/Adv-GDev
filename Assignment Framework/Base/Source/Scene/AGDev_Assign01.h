#ifndef GDEV_ASSIGNMENT01_H
#define GDEV_ASSIGNMENT01_H

#include "..\SceneBase.h"
#include "..\Character\ThirdPerson.h"

class CGameStateManager;

class AGDev_Assign01 : public SceneBase
{
public:
	enum MESH_TYPE
	{
		MESH_TEXT = 0,
		MESH_CUBE,
		MESH_CONE,
		NUM_MESH,
	};
	AGDev_Assign01(int width = 1280, int height = 720);
	virtual ~AGDev_Assign01();

	// Functions from scenebase, do not add parameters else override will not work
	virtual void Init(int width, int height);
	virtual void Update(CGameStateManager* GSM, double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void ProcessKeys(bool* keys);

	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key, double dt = 0.0);

	void SetHUD(const bool m_bHUDmode);

private:
	void initMesh();

private:
	Mesh* m_meshList[NUM_MESH];
	CGameObject* test;

	CThirdPerson* m_char;

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif