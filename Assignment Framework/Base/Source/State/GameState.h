#ifndef _GAMESTATE_H_
#define _GAMESTATE_H_

class CGameStateManager;

class CGameState
{
public:
	CGameState();
	virtual ~CGameState();

	virtual void Init(CGameStateManager* GSM, int width, int height) = 0;
	virtual void Update(double dt) = 0;
	virtual void Render() = 0;
	virtual void Exit() = 0;
	virtual void Reset() = 0;

	virtual void Pause();
	virtual void Resume();

	virtual void ProcessKeys(double dt, bool* keys) = 0;

protected:
	bool m_pause;
	CGameStateManager* m_GSM; // Handle to gsm to change state from state itself
};

#endif