#include "Object.h"

CObject::CObject() : m_mesh(NULL), m_active(false), m_render(false)
{
}

CObject::~CObject()
{
	if (m_mesh)
	{
		delete m_mesh;
		m_mesh = NULL;
	}
}

void CObject::Init(Mesh* mesh, bool active, bool render)
{
	this->m_mesh = mesh;
	this->m_active = active;
	this->m_render = render;
}

void CObject::Update(const double dt)
{
}

void CObject::Reset()
{
	m_mesh = NULL;
	m_active = m_render = false;
}

void CObject::SetMesh(Mesh * mesh)
{
	this->m_mesh = mesh;
}

Mesh * CObject::GetMesh()
{
	return m_mesh;
}

void CObject::SetRender(bool render)
{
	this->m_render = render;
}

bool CObject::GetRender()
{
	return m_render;
}

void CObject::SetActive(bool active)
{
	m_active = active;
}

bool CObject::GetActive()
{
	return m_active;
}
