#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "..\Object\GameObject.h"
#include <vector>

using std::vector;

class AGDev_Assign01;

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

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	virtual void Update(const double dt);
	virtual void Reset();

	// Setters and Getters
	vector<CSceneNode*>& GetChildren();
	
	void SetType(E_NODE_TYPE type);
	E_NODE_TYPE GetType();

	void Draw(AGDev_Assign01* scene);

	void AddChild(CSceneNode* newNode);
	void AddChild(E_NODE_TYPE type, CSceneNode* newNode);

	CSceneNode* Search(E_NODE_TYPE searchType);

private:
	// Check where a line segment between two positions intersects a plane
	int GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	// Check two positions are within a box region
	int InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

private:
	vector<CSceneNode*> m_children;
	E_NODE_TYPE m_type;
};

#endif