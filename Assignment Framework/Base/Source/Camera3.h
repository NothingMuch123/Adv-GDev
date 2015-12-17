#ifndef CAMERA_3_H
#define CAMERA_3_H

#include "Camera.h"
#include "LoadHmap.h"

class Camera3 : public Camera
{
public:
	static float TERRAIN_OFFSET;
	static float CAMERA_SPEED;
	static float CROUCH_SPEED;
	static float CROUCH_OFFSET;
	enum CAM_TYPE
	{
		LAND_CAM,
		AIR_CAM,
		NUM_CAM_TYPE,
	};

	Vector3 defaultPosition;
	Vector3 defaultTarget;
	Vector3 defaultUp;

	Camera3();
	~Camera3();
	virtual void Init(const Vector3& pos, const Vector3& target, const Vector3& up);
	virtual void Update(double dt);
	virtual void Reset();
	virtual void UpdateStatus(unsigned char key);

	void SetCameraType(CAM_TYPE type);
	CAM_TYPE GetCameraType();

	virtual void MoveForward_Backward(double dt, bool dir, float speed = CAMERA_SPEED);	// 0 - Forward	| 1 - Backwards
	virtual void MoveLeft_Right(double dt, bool dir, float speed = CAMERA_SPEED);		// 0 - Left		| 1 - Right
	virtual void MoveUp_Down(double dt, bool dir, float speed = CAMERA_SPEED);			// 0 - Up		| 1 - Down

	// Basic methods
	virtual void LookUp(const double dt, float camera_pitch);
	virtual void LookDown(const double dt, float camera_pitch);
	virtual void TurnLeft(const double dt, float camera_yaw);
	virtual void TurnRight(const double dt, float camera_yaw);
	virtual void SpinClockwise(const double dt);
	virtual void SpinCounterClockwise(const double dt);
	virtual void Crouch(const double dt);

	// Applied methods
	virtual void Pitch(const double dt, float camera_pitch);
	virtual void Yaw(const double dt, float camera_yaw);
	virtual void Roll(const double dt);
	virtual void Walk(const double dt);
	virtual void Strafe(const double dt);
	virtual void Jump(const double dt);

	virtual void UpdateJump(const double dt);

private:
	bool myKeys[255];
	/*CAM_TYPE type;

	// Jump
	bool m_bJumping;
	float GRAVITY;
	float JumpVel;
	float JUMPMAXSPEED, JUMPACCEL;

	// Sprint
	bool sprint;
	bool crouch;*/
};

#endif