#ifndef _PLAYSTATE_H
#define _PLAYSTATE_H

#include "GameState.h"
#include "..\Scene\AGDev_Assign01.h"

class CPlayState : public CGameState
{
public:
	CPlayState();
	virtual ~CPlayState();

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
	AGDev_Assign01* scene;
};

#endif