#include "Projectile.h"



CProjectile::CProjectile()
	: m_dir(0,0,1)
	, m_speed(0.f)
{
}


CProjectile::~CProjectile()
{
}

void CProjectile::Init(Vector3 dir, float speed, Mesh * mesh, CTransform * transform, bool active, bool render)
{
	CGameObject::Init(mesh, transform, active, render);
	m_dir = dir;
	m_speed = speed;
}

void CProjectile::Update(const double dt)
{
	CGameObject::Update(dt);

	if (!m_dir.IsZero())
	{
		m_transform.m_translate += m_dir * (m_speed * dt);
	}
}

void CProjectile::Reset()
{
	CGameObject::Reset();
	m_dir.Set(0, 0, 1);
	m_speed = 0.f;
}

void CProjectile::SetDir(Vector3 dir)
{
	m_dir = dir;
}

Vector3 & CProjectile::GetDir()
{
	return m_dir;
}
