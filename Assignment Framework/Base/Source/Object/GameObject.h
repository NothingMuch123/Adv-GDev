#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Object.h"
#include "Transform.h"
#include "Collider.h"

class CGameObject : public CObject, public CCollider
{
protected:
	CTransform m_transform;

public:
	CGameObject();
	virtual ~CGameObject();

	virtual void Init(Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	virtual void Update(const double dt);
	virtual void Reset();

	// Setters and Getters
	CTransform& GetTransform();
};

#endif