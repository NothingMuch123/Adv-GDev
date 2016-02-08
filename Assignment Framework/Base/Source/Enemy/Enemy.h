#ifndef ENEMY_H
#define ENEMY_H

#include "..\SceneGraph\SceneNode.h"

class CEnemy : public CSceneNode
{
public:
	CEnemy();
	virtual ~CEnemy();

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	virtual void Update(double dt);
	virtual void Reset();

private:

};

#endif