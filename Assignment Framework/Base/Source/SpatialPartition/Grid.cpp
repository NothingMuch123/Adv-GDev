#include "Grid.h"

CGrid::CGrid()
	: m_nodeList(NULL)
{
}

CGrid::~CGrid()
{
}

void CGrid::Init(Mesh * mesh, CTransform * transform, bool active, bool render)
{
	CGameObject::Init(mesh, transform, active, render);
}

void CGrid::Update(const double dt)
{
	for (int i = 0; i < m_nodeList.size(); ++i)
	{
		CSceneNode* node = m_nodeList[i];
		node->CGameObject::Update(dt);
	}
}

void CGrid::Reset()
{
	CGameObject::Reset();
	m_nodeList.clear();
}

vector<CSceneNode*>& CGrid::GetList()
{
	return m_nodeList;
}

void CGrid::AddToList(CSceneNode * node)
{
	if (!node)
	{
		return;
	}
	m_nodeList.push_back(node);
}
