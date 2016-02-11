#ifndef ENEMY_H
#define ENEMY_H

#include "..\SceneGraph\SceneNode.h"
#include "..\TileSystem\TileMap.h"
#include "..\Lua\Lua_Wrapper.h"
#include "..\Lua\Lua_Serializable.h"

class CEnemy : public CSceneNode, public CLua_Serializable
{
public:
	enum E_ENEMY_SAVE_PROPERTIES
	{
		ENEMY_POS_X,
		ENEMY_POS_Y,
		ENEMY_POS_Z,
		ENEMY_ROTATE_X,
		ENEMY_ROTATE_Y,
		ENEMY_ROTATE_Z,
		ENEMY_SCALE_X,
		ENEMY_SCALE_Y,
		ENEMY_SCALE_Z,
		ENEMY_CURRENT_FSM_STATE,
		ENEMY_DES_ROW_INDEX,
		ENEMY_DES_COL_INDEX,
		ENEMY_PREV_ROW_INDEX,
		ENEMY_PREV_COL_INDEX,
		ENEMY_TARGET_ID,
		ENEMY_DEFAULT_Y,
		ENEMY_CALM_DOWN_TIMER,
		ENEMY_RESPAWN_TIMER,
		NUM_ENEMY_SAVE_PROPERTIES,
	};

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

	static float S_ENEMY_NORMAL_SPEED;
	static float S_ENEMY_ESCAPE_SPEED;
	static float S_ENEMY_CALM_DOWN_TIME;
	static float S_ENEMY_SHOOT_TIME;
	static float S_ENEMY_RESPAWN_TIME;
	static float S_DETECTION_RADIUS;
	static int S_ESCAPE_PROBABILITY;
	static CTileMap* S_MAP_REF;
	static string S_PROPERTIES[NUM_ENEMY_SAVE_PROPERTIES];

	CEnemy();
	virtual ~CEnemy();

	static void InitEnemyDataFromLua();

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, CTile* currentTile = nullptr, bool active = true, bool render = true);
	virtual void Update(double dt);
	virtual void Reset();

	void Alert(CSceneNode* target);
	void CalmDown();
	void Detect(CSceneNode* target);

	void Kill();
	bool IsAlive();

	virtual bool SaveState(fstream* file, int id);
	virtual bool LoadState(CLua_Wrapper* lua, int id);

private:
	void move(double dt);
	void escape(double dt);
	void die(double dt);
	void respawn(double dt);
	void attack(double dt);
	CTile* generateDestination(bool escape = false);

private:
	E_ENEMY_FSM m_currentFSM;
	CTile* m_destination;
	CTile* m_prev;
	CSceneNode* m_target;
	float m_defaultY;

	// Timers
	float m_calmDownTimer;
	float m_respawnTimer;
};

#endif