#ifndef _CREDITSSTATE_H_
#define _CREDITSSTATE_H_

#include "GameState.h"
#include "..\Scene\SceneCredits.h"

class CCreditsState : public CGameState
{
public:
	CCreditsState();
	virtual ~CCreditsState();

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
	SceneCredits* scene;
};

#endif