#ifndef ENEMY_H
#define ENEMY_H

#include "..\SceneGraph\SceneNode.h"
#include "..\TileSystem\TileMap.h"

class CEnemy : public CSceneNode
{
public:
	static const float S_ENEMY_SPEED;
	static CTileMap* S_MAP_REF;

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

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, CTile* currentTile = nullptr, bool active = true, bool render = true);
	virtual void Update(double dt);
	virtual void Reset();

private:
	void move(double dt);
	CTile* generateDestination();

private:
	E_ENEMY_FSM m_currentFSM;
	CTile* m_destination;
	CTile* m_prev;
	Vector3 m_dir;
};

#endif