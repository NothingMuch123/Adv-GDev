#ifndef GDEV_ASSIGNMENT01_H
#define GDEV_ASSIGNMENT01_H

#include "..\SceneBase.h"

class CGameStateManager;

class AGDev_Assign01 : public SceneBase
{
public:
	AGDev_Assign01(int width = 1280, int height = 720);
	virtual ~AGDev_Assign01();

	virtual void Init(int width, int height);
	virtual void Update(CGameStateManager* GSM, double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();
	virtual void UpdateCameraStatus(unsigned char key);
	virtual void UpdateWeaponStatus(unsigned char key, double dt = 0.0);

	void SetHUD(const bool m_bHUDmode);

private:

	// Sound
	irrklang::ISoundEngine *sound;
};

#endif