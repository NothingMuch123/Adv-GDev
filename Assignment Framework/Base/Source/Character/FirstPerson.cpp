#include "FirstPerson.h"



CFirstPerson::CFirstPerson()
{
}


CFirstPerson::~CFirstPerson()
{
}

void CFirstPerson::Init(Camera3 * view, Mesh * mesh, CTransform * transform, bool active, bool render)
{
	m_view = view;
	CGameObject::Init(mesh, transform, active, render);
}

void CFirstPerson::SetView(Camera3 * view)
{
	m_view = view;
}

Camera3 * CFirstPerson::GetView()
{
	return m_view;
}

void CFirstPerson::updateViewPos()
{
	m_view->position = m_transform.m_translate;
}

void CFirstPerson::updateTransformPos()
{
	m_transform.m_translate = m_view->position;
}
