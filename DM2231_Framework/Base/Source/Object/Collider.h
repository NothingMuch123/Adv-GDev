#ifndef COLLIDER_H
#define COLLIDER_H

#include "Transform.h"

class CCollider
{
public:
	enum E_COLLIDER_TYPE
	{
		CT_NONE = 0, // No collision
		CT_AABB, // AABB Collision
		CT_DIST, // Distance-based collision
		NUM_CT, // Total types of collision
	};

	enum E_IGNORE_AXIS
	{
		IGNORE_X = 0,
		IGNORE_Y,
		IGNORE_Z,
		NUM_IGNORE,
	};

	enum E_Y_START
	{
		Y_BOTTOM = 0,
		Y_MIDDLE,
		NUM_Y_START,
	};

private:
	E_COLLIDER_TYPE m_type; // Determine which collision method to use
	Vector3 m_minBound, m_maxBound; // AABB Collision data
	Vector3 m_diameter, m_position; // Distance-based collision data (Store as vector for irregular diameter like oval)
	bool m_ignore[NUM_IGNORE];
	E_Y_START m_yStart;
	bool m_active;

public:
	CCollider();
	~CCollider();


	// Setters and Getters
	void SetType(E_COLLIDER_TYPE type, CTransform& transform);
	E_COLLIDER_TYPE GetType();

	void SetActive(bool active);
	bool GetActive();

	Vector3& GetMinBound();
	Vector3& GetMaxBound();
	Vector3& GetPosition();
	Vector3& GetDiameter();

	void SetIgnore(bool x, bool y, bool z); // Set ignore for axis
	float GetIgnore(E_IGNORE_AXIS index);


	// Main functions
	void Init(E_COLLIDER_TYPE type, CTransform& transform, E_Y_START yStart, bool active); // Create collision bounds
	void Update(CTransform& transform); // Updates collision details
	void Reset();


	// Collision functions
	bool CollideWith(CCollider& other, const double dt);

private:
	// Methods to calculate bound data
	void calcAABB(CTransform& transform);
	void calcDist(CTransform& transform);


	// Methods for collision
	bool AABBCollision(CCollider& other, const double dt);
	bool distCollision(CCollider& other, const double dt);
	bool AABB_Dist_Collision(CCollider& other, const double dt); // Other MUST be dist
};

#endif