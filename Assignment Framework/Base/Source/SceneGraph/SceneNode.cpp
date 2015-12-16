#include "SceneNode.h"
#include "..\Scene\AGDev_Assign01.h"


CSceneNode::CSceneNode()
	: m_children(NULL)
	, m_type(NODE_NONE)
{
}


CSceneNode::~CSceneNode()
{
	while (m_children.size() > 0)
	{
		if (m_children.back())
		{
			delete m_children.back();
			m_children.pop_back();
		}
	}
}

void CSceneNode::Init(E_NODE_TYPE type, Mesh * mesh, CTransform * transform, bool active, bool render)
{
	CGameObject::Init(mesh, transform, active, render);
	CSceneNode::m_type = type;
}

void CSceneNode::Update(const double dt)
{
	CGameObject::Update(dt);
	for (int i = 0; i < m_children.size(); ++i)
	{
		CSceneNode* child = m_children[i];
		child->CSceneNode::Update(dt);
	}
}

void CSceneNode::Reset()
{
	CGameObject::Reset();
	for (int i = 0; i < m_children.size(); ++i)
	{
		CSceneNode* child = m_children[i];
		child->CGameObject::Reset();
	}
}

vector<CSceneNode*>& CSceneNode::GetChildren()
{
	return m_children;
}

void CSceneNode::SetType(E_NODE_TYPE type)
{
	m_type = type;
}

CSceneNode::E_NODE_TYPE CSceneNode::GetType()
{
	return m_type;
}

void CSceneNode::Draw(AGDev_Assign01 * scene)
{
	scene->PreRendering(m_transform.m_translate, m_transform.m_rotate, m_transform.m_scale, false);
	if (m_mesh)
	{
		m_mesh->Render();
	}

	if (m_children.size() > 0) // Children nodes exists
	{
		CSceneNode* child = NULL;
		for (int i = 0; i < m_children.size(); ++i)
		{
			child = m_children[i];
			child->Draw(scene);
		}
	}

	scene->PostRendering();
}

void CSceneNode::AddChild(CSceneNode * newNode)
{
	if (newNode)
	{
		m_children.push_back(newNode);
	}
}

void CSceneNode::AddChild(E_NODE_TYPE type, CSceneNode * newNode)
{
	CSceneNode* addTo = Search(type);
	if (addTo) // Found node
	{
		addTo->AddChild(newNode);
	}
}

CSceneNode * CSceneNode::Search(E_NODE_TYPE searchType)
{
	if (this->CSceneNode::GetType() == searchType)
	{
		return this;
	}

	if (m_children.size() > 0)
	{
		CSceneNode* child = NULL;
		CSceneNode* result = NULL;
		for (int i = 0; i < m_children.size(); ++i)
		{
			child = m_children[i];
			result = child->Search(searchType);
			if (result)
			{
				return result;
			}
		}
	}
}
