#include "GameObject.h"

CGameObject::CGameObject() : CObject(), CCollider(), transform()
{
}

CGameObject::~CGameObject()
{
}

void CGameObject::Init(Mesh* mesh, bool active, bool render)
{
	CObject::Init(mesh, active, render);
}

void CGameObject::Update(const double dt)
{
}

void CGameObject::Reset()
{
	CObject::Reset();
	CCollider::Reset();
	transform.Reset();
}

CTransform & CGameObject::GetTransform()
{
	return transform;
}
