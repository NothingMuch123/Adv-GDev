#ifndef _THIRDPERSON_H_
#define _THIRDPERSON_H_

#include "FirstPerson.h"

class CThirdPerson : public CFirstPerson
{
public:
	static const float S_OFFSET_TARGET;

	CThirdPerson();
	virtual ~CThirdPerson();

	virtual void Init(E_NODE_TYPE type, Camera3* view, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	
	void Look(double dt, float yaw, float pitch);
	void MoveForward(double dt);
	void MoveBackward(double dt);
	void MoveLeft(double dt);
	void MoveRight(double dt);

	Camera3* GetTPView();

protected:
	Vector3 calcTPviewPos();
	void generateTPview(Camera3* view);

protected:
	// Offset between first person view position and third person view position by rotation except for back
	static const float S_OFFSET_BACK;	// Offset for z axis
	static const float S_OFFSET_LEFT;	// Offset for x axis
	static const float S_OFFSET_UP;		// Offset for y axis

	Camera3* m_TPview; // Third person view camera
};

#endif