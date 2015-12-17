#ifndef _PROJECTILE_H_
#define _PROJECTILE_H_

#include "..\Object\GameObject.h"
class CProjectile : public CGameObject
{
public:
	CProjectile();
	virtual ~CProjectile();

	virtual void Init(Vector3 dir, float speed, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	virtual void Update(const double dt);
	virtual void Reset();

	// Setters and Getters
	void SetDir(Vector3 dir);
	Vector3& GetDir();

	void SetLength(float length);
	float GetLength();

private:
	Vector3 m_dir;
	float m_speed;
	float m_length;
};

#endif