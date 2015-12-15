#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "..\Object\GameObject.h"
#include <vector>

using std::vector;

class CSceneNode : public CGameObject
{
public:
	enum E_NODE_TYPE
	{
		NODE_NONE = 0,
		NUM_NODE,
	};

	CSceneNode();
	virtual ~CSceneNode();

	// Setters and Getters
	vector<CSceneNode*>& GetChildren();
	
	void SetType(E_NODE_TYPE type);
	E_NODE_TYPE GetType();

private:
	vector<CSceneNode*> m_children;
	E_NODE_TYPE m_type;
};

#endif