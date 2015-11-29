#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include "Object.h"
#include "Transform.h"
#include "Collider.h"

class CGameObject : public CObject, public CCollider
{
private:
	CTransform transform;

public:
	CGameObject();
	virtual ~CGameObject();

	virtual void Init(Mesh* mesh, bool active, bool render);
	virtual void Update(const double dt);
	virtual void Reset();

	// Setters and Getters
	CTransform& GetTransform();
};

#endif