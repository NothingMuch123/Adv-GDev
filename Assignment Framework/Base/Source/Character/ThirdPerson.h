#ifndef _THIRDPERSON_H_
#define _THIRDPERSON_H_

#include "FirstPerson.h"

class CThirdPerson : public CFirstPerson
{
public:
	static const float S_MIN_OFFSET_TARGET;
	static const float S_MAX_OFFSET_TARGET;
	static float S_OFFSET_TARGET;

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
};

#endif