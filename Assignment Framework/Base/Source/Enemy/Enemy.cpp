#include "Enemy.h"

CEnemy::CEnemy()
	: CSceneNode()
	, m_currentFSM(ENEMY_IDLE)
{
}


CEnemy::~CEnemy()
{
	Reset();
}

void CEnemy::Init(E_NODE_TYPE type, Mesh * mesh, CTransform * transform, bool active, bool render)
{
	CSceneNode::Init(type, mesh, transform, active, render);
}

void CEnemy::Update(double dt)
{
}

void CEnemy::Reset()
{
	CSceneNode::Reset();
}
