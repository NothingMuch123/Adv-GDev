#ifndef _TEMPLATESTATE_H
#define _TEMPLATESTATE_H

#include "GameState.h"
#include "..\Scene\AGDev_Assign01.h"

class CTemplateState : public CGameState
{
public:
	CTemplateState();
	virtual ~CTemplateState();

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