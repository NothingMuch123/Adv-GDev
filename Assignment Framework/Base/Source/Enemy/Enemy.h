#ifndef ENEMY_H
#define ENEMY_H

#include "..\SceneGraph\SceneNode.h"
#include "..\TileSystem\TileMap.h"

class CEnemy : public CSceneNode
{
public:
	static const float S_ENEMY_NORMAL_SPEED;
	static const float S_ENEMY_ESCAPE_SPEED;
	static const float S_ENEMY_CALM_DOWN_TIME;
	static const float S_ENEMY_SHOOT_TIME;
	static const float S_ENEMY_RESPAWN_TIME;
	static const float S_DETECTION_RADIUS;
	static CTileMap* S_MAP_REF;

	enum E_ENEMY_FSM
	{
		ENEMY_IDLE,
		ENEMY_ATTACK,
		ENEMY_PATROL,
		ENEMY_ESCAPE,
		ENEMY_KO,
		ENEMY_RESPAWN,
		NUM_ENEMY_FSM,
	};

	CEnemy();
	virtual ~CEnemy();

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, CTile* currentTile = nullptr, bool active = true, bool render = true);
	virtual void Update(double dt);
	virtual void Reset();

	void Alert(CSceneNode* target);
	void CalmDown();
	void Detect(CSceneNode* target);

	void Kill();

private:
	void move(double dt);
	void escape(double dt);
	void die(double dt);
	void respawn(double dt);
	CTile* generateDestination(bool escape = false);

private:
	E_ENEMY_FSM m_currentFSM;
	CTile* m_destination;
	CTile* m_prev;
	Vector3 m_dir;
	CSceneNode* m_target;
	float m_defaultY;

	// Timers
	float m_calmDownTimer;
	float m_respawnTimer;
};

#endif