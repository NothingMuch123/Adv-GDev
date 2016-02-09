#ifndef ENEMY_H
#define ENEMY_H

#include "..\SceneGraph\SceneNode.h"

class CEnemy : public CSceneNode
{
public:
	enum E_ENEMY_FSM
	{
		ENEMY_IDLE,
		ENEMY_ATTACK,
		ENEMY_PATROL,
		ENEMY_KO,
		ENEMY_RESPAWN,
		NUM_ENEMY_FSM,
	};

	CEnemy();
	virtual ~CEnemy();

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	virtual void Update(double dt);
	virtual void Reset();

private:
	E_ENEMY_FSM m_currentFSM;
};

#endif