#ifndef _GRID_H_
#define _GRID_H_

#include "..\SceneGraph\SceneNode.h"

class CGrid : public CGameObject
{
public:
	CGrid();
	virtual ~CGrid();

	virtual void Init(Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	virtual void Update(const double dt);
	virtual void Reset();

	void Remove(CSceneNode* object);

	vector<CSceneNode*>& GetList();

	void AddToList(CSceneNode* node);

private:
	vector<CSceneNode*> m_nodeList;
};

#endif