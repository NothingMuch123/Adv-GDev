#ifndef _MAINMENUSTATE_H_
#define _MAINMENUSTATE_H_

#include "GameState.h"
#include "..\Scene\SceneMainMenu.h"

class CMainMenuState : public CGameState
{
public:
	CMainMenuState();
	virtual ~CMainMenuState();

	// CGameState virtual functions
	virtual void Init(CGameStateManager* GSM, int width, int height);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();

	virtual void Pause();
	virtual void Resume();

	virtual void ProcessKeys(double dt, bool* keys);

private:
	// TODO: Add a scene
	SceneBase* scene;
};

#endif