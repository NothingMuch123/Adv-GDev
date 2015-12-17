#ifndef _INSTRUCTIONSTATE_H_
#define _INSTRUCTIONSTATE_H_

#include "GameState.h"
#include "..\Scene\SceneInstructions.h"

class CInstructionsState : public CGameState
{
public:
	CInstructionsState();
	virtual ~CInstructionsState();

	// CGameState virtual functions
	virtual void Init(CGameStateManager* GSM, int width, int height);
	virtual void Update(double dt);
	virtual void Render();
	virtual void Exit();
	virtual void Reset();

	virtual void Pause();
	virtual void Resume();

	virtual void ProcessKeys(double dt, bool* keys);
	SceneInstructions* GetScene();

private:
	// TODO: Add a scene
	SceneInstructions* scene;
};

#endif