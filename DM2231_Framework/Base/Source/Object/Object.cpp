#include "Object.h"

CObject::CObject() : m_mesh(NULL)
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

void CObject::Init(Mesh * mesh)
{
	this->m_mesh = mesh;
}

void CObject::Update(const double dt)
{
}

void CObject::Reset()
{
	m_mesh = NULL;
}

void CObject::SetMesh(Mesh * mesh)
{
	this->m_mesh = mesh;
}

Mesh * CObject::GetMesh()
{
	return m_mesh;
}
