#ifndef _THIRDPERSON_H_
#define _THIRDPERSON_H_

#include "FirstPerson.h"
#include "..\Lua\Lua_Serializable.h"

class CThirdPerson : public CFirstPerson, public CLua_Serializable
{
public:
	enum E_PLAYER_SAVE_PROPERTIES
	{
		PLAYER_POS_X,
		PLAYER_POS_Y,
		PLAYER_POS_Z,
		PLAYER_ROTATE_X,
		PLAYER_ROTATE_Y,
		PLAYER_ROTATE_Z,
		PLAYER_SCALE_X,
		PLAYER_SCALE_Y,
		PLAYER_SCALE_Z,
		PLAYER_TARGET_X,
		PLAYER_TARGET_Y,
		PLAYER_TARGET_Z,
		PLAYER_UP_X,
		PLAYER_UP_Y,
		PLAYER_UP_Z,
		PLAYER_TP_POS_X,
		PLAYER_TP_POS_Y,
		PLAYER_TP_POS_Z,
		PLAYER_TP_UP_X,
		PLAYER_TP_UP_Y,
		PLAYER_TP_UP_Z,
		PLAYER_TOTAL_YAW,
		PLAYER_TOTAL_PITCH,
		NUM_PLAYER_SAVE_PROPERTIES,
	};

	static const float S_MIN_OFFSET_TARGET;
	static const float S_MAX_OFFSET_TARGET;
	static float S_OFFSET_TARGET;
	static string S_PROPERTIES[NUM_PLAYER_SAVE_PROPERTIES];

	CThirdPerson();
	virtual ~CThirdPerson();

	virtual void Init(E_NODE_TYPE type, Camera3* view, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	
	void Look(double dt, float yaw, float pitch);
	void MoveForward(double dt);
	void MoveBackward(double dt);
	void MoveLeft(double dt);
	void MoveRight(double dt);
	void MoveUp(double dt);
	void MoveDown(double dt);

	void Sprint(bool mode);
	void changeTargetOffset(double dt, bool mode); // 0 = Not moving | 1 = Moving

	Camera3* GetTPView();

	void Injure(int damage);
	bool IsAlive();

	virtual bool SaveState(fstream* file, int id);
	virtual bool LoadState(CLua_Wrapper* lua, int id);

protected:
	Vector3 calcTPviewPos();
	void generateTPview(Camera3* view);
	void updateTarget();

protected:
	// Offset between first person view position and third person view position by rotation except for back
	static const float S_OFFSET_BACK;	// Offset for z axis
	static const float S_OFFSET_LEFT;	// Offset for x axis
	static const float S_OFFSET_UP;		// Offset for y axis
	
	// Movement speed
	static const float S_NORMAL_SPEED;
	static const float S_SPRINT_SPEED;

	float m_speed;
	float m_targetSpeed; // The speed in which the target offset increases and decrease when moving

	Camera3* m_TPview; // Third person view camera

	// Temp variable
	int m_health;
};

#endif