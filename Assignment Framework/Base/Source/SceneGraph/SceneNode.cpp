#include "SceneNode.h"



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
