#pragma once

#include "Vector3.h"
#include <stdio.h>

class CProjectile
{
public:
	CProjectile(void);
	~CProjectile(void);
private:
	// Boolean flag to indicate if this projectile is active. If not active, then do not compute/update
	bool m_bActive;
public:
	// Update the status of this projectile
	void Update(const double dt);
	// Activate the projectile. true == active, false == inactive
	void SetActivate(bool m_bActivate);
	// get status of the projectile. true == active, false == inactive
	bool GetStatus(void);
	// Get the position of the projectile
	Vector3 GetPosition();
	// Get the direction of the projectile
	Vector3 GetDirection(void);
	// Set the position of the projectile
	void SetPosition(Vector3 theNewPosition);
	// Set the direction of the projectile
	void SetDirection(Vector3 theNewDirection);
	// Set speed
	void SetSpeed(const float speed);
private:
	// The position of the projectile
	Vector3 thePosition;
	// The direction of the projectile
	Vector3 theDirection;
	// Speed
	float speed;
};
