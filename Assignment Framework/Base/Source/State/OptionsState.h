#ifndef _LEVELSELECTSTATE_H_
#define _LEVELSELECTSTATE_H_

#include "GameState.h"
#include "..\Scene\SceneOptions.h"

class COptionsState : public CGameState
{
public:
	COptionsState();
	virtual ~COptionsState();

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
	SceneOptions* scene;
};

#endif