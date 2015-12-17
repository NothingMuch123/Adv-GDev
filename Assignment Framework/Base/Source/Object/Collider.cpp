#include "Collider.h"

CCollider::CCollider()
	: m_maxBound(Vector3::ZERO_VECTOR)
	, m_minBound(Vector3::ZERO_VECTOR)
	, m_position(Vector3::ZERO_VECTOR)
	, m_diameter(1.f, 1.f, 1.f)
	, m_type(CT_AABB)
	, m_yStart(Y_BOTTOM)
	, m_xStart(X_MIDDLE)
{
	for (int i = 0; i < NUM_IGNORE; ++i) // Set ignores to false
	{
		m_ignore[i] = false;
	}
}

CCollider::~CCollider()
{
}

void CCollider::SetType(E_COLLIDER_TYPE type, CTransform& transform)
{
	this->m_type = type;
	calcAABB(transform);
	calcDist(transform);
}

CCollider::E_COLLIDER_TYPE CCollider::GetType()
{
	return m_type;
}

void CCollider::SetActive(bool active)
{
	this->m_active = active;
}

bool CCollider::GetActive()
{
	return m_active;
}

Vector3 & CCollider::GetMinBound()
{
	return m_minBound;
}

Vector3 & CCollider::GetMaxBound()
{
	return m_maxBound;
}

Vector3 & CCollider::GetPosition()
{
	return m_position;
}

Vector3 & CCollider::GetDiameter()
{
	return m_diameter;
}

void CCollider::SetIgnore(bool x, bool y, bool z)
{
	m_ignore[IGNORE_X] = x;
	m_ignore[IGNORE_Y] = y;
	m_ignore[IGNORE_Z] = z;
}

float CCollider::GetIgnore(E_IGNORE_AXIS index)
{
	return m_ignore[index];
}

void CCollider::Init(E_COLLIDER_TYPE type, CTransform & transform, E_X_START xStart,  E_Y_START yStart, bool active)
{
	this->m_type = type;
	calcAABB(transform);
	calcDist(transform);
	this->m_xStart = xStart;
	this->m_yStart = yStart;
	this->m_active = active;
}

void CCollider::Update(CTransform & transform)
{
	calcAABB(transform);
	calcDist(transform);
}

void CCollider::Reset()
{
	m_minBound = m_maxBound = m_position = Vector3::ZERO_VECTOR;
	m_diameter = Vector3(1.f, 1.f, 1.f);
	m_type = CT_AABB;
	m_yStart = Y_BOTTOM;
	for (int i = 0; i < NUM_IGNORE; ++i)
	{
		m_ignore[i] = false;
	}
	m_active = true;
}

bool CCollider::CollideWith(CCollider& other, const double dt)
{
	if (!this->m_active || other.GetActive()) // If one of the collider does not collide, no collision will occur, hence false
	{
		return false;
	}

	if (this->m_type == CT_AABB)
	{
		if (other.GetType() == CT_AABB)
		{
			// AABB - AABB collision
			return AABBCollision(other, dt);
		}
		else if (other.GetType() == CT_DIST)
		{
			// AABB - Dist collision
			return AABBCollision(other, dt); // Use AABB - AABB collision
		}
	}
	else if (this->m_type == CT_DIST)
	{
		if (other.GetType() == CT_AABB)
		{
			// Dist - AABB collision
			return AABBCollision(other, dt); // Use AABB - AABB collision
		}
		else if (other.GetType() == CT_DIST)
		{
			// Dist - Dist collision
			return distCollision(other, dt);
		}
	}
}

void CCollider::calcAABB(CTransform & transform)
{
	Vector3 scale = transform.GetScale();

	Vector3 centerPos;
	centerPos.z = transform.GetTranslate().z;
	// Determine center x pos
	if (m_xStart == X_LEFT)
	{
		centerPos.x = transform.GetTranslate().x + m_diameter.x * 0.5f;
	}
	else if (m_xStart == X_MIDDLE)
	{
		centerPos.x = transform.GetTranslate().x;
	}
	else if (m_xStart == X_RIGHT)
	{
		centerPos.x = transform.GetTranslate().x - m_diameter.x * 0.5f;
	}

	// Determine center y pos
	if (m_yStart == Y_BOTTOM)
	{
		centerPos.y = transform.GetTranslate().y + m_diameter.y * 0.5f;
	}
	else if (m_yStart == Y_MIDDLE)
	{
		centerPos.y = transform.GetTranslate().y;
	}
	else if (m_yStart == Y_TOP)
	{
		centerPos.y = transform.GetTranslate().y - m_diameter.y * 0.5f;
	}
	Vector3 pos = centerPos;

	m_minBound.Set(pos.x - (scale.x * 0.5f), pos.y - (scale.y * 0.5f), pos.z - (scale.z * 0.5f));
	m_maxBound.Set(pos.x + (scale.x * 0.5f), pos.y + (scale.y * 0.5f), pos.z + (scale.z * 0.5f));
}

void CCollider::calcDist(CTransform & transform)
{
	m_diameter = transform.GetScale();
	Vector3 centerPos;
	centerPos.z = transform.GetTranslate().z;
	// Determine center x pos
	if (m_xStart == X_LEFT)
	{
		centerPos.x = transform.GetTranslate().x + m_diameter.x * 0.5f;
	}
	else if (m_xStart == X_MIDDLE)
	{
		centerPos.x = transform.GetTranslate().x;
	}
	else if (m_xStart == X_RIGHT)
	{
		centerPos.x = transform.GetTranslate().x - m_diameter.x * 0.5f;
	}

	// Determine center y pos
	if (m_yStart == Y_BOTTOM)
	{
		centerPos.y = transform.GetTranslate().y + m_diameter.y * 0.5f;
	}
	else if (m_yStart == Y_MIDDLE)
	{
		centerPos.y = transform.GetTranslate().y;
	}
	else if (m_yStart == Y_TOP)
	{
		centerPos.y = transform.GetTranslate().y - m_diameter.y * 0.5f;
	}
	m_position = centerPos;
}

bool CCollider::AABBCollision(CCollider & other, const double dt)
{
	Vector3 oMin = other.GetMinBound();
	Vector3 oMax = other.GetMaxBound();
	if  (	( !(m_ignore[IGNORE_X] || other.GetIgnore(IGNORE_X)) && (m_maxBound.x < oMin.x || m_minBound.x > oMax.x) ) ||
			( !(m_ignore[IGNORE_Y] || other.GetIgnore(IGNORE_Y)) && (m_maxBound.y < oMin.y || m_minBound.y > oMax.y) ) ||
			( !(m_ignore[IGNORE_Z] || other.GetIgnore(IGNORE_Z)) && (m_maxBound.z < oMin.z || m_minBound.z > oMax.z) )
		)
	{
		return false;
	}
	return true;
}

bool CCollider::distCollision(CCollider & other, const double dt)
{
	// TODO: Make exceptions for oval

	// Temp dist collision (Only works with circles)
	float distSquared = (other.GetPosition() - m_position).LengthSquared();
	float thisRadius = m_diameter.x * 0.5f, oRadius = other.GetDiameter().x * 0.5f;

	if (((thisRadius * thisRadius) + (oRadius * oRadius)) < distSquared)
	{
		return true;
	}
	return false;
}

bool CCollider::AABB_Dist_Collision(CCollider & other, const double dt)
{
	// TODO: Do AABB - Dist collision check
	return false;
}
