#include "Enemy.h"

const float CEnemy::S_ENEMY_SPEED = 100.f;
CTileMap* CEnemy::S_MAP_REF = nullptr;

CEnemy::CEnemy()
	: CSceneNode()
	, m_currentFSM(ENEMY_IDLE)
	, m_dir(0, 0, 1)
	, m_destination(nullptr)
	, m_prev(nullptr)
{
}


CEnemy::~CEnemy()
{
	Reset();
}

void CEnemy::Init(E_NODE_TYPE type, Mesh * mesh, CTransform * transform, CTile* currentTile, bool active, bool render)
{
	CSceneNode::Init(type, mesh, transform, currentTile, active, render);
}

void CEnemy::Update(double dt)
{
	CSceneNode::Update(dt);

	switch (m_currentFSM)
	{
	case ENEMY_IDLE:
		{
			m_currentFSM = ENEMY_PATROL;
		}
		break;
	case ENEMY_ATTACK:
		{
		}
		break;
	case ENEMY_PATROL:
		{
			move(dt);
		}
		break;
	case ENEMY_KO:
		{
		}
		break;
	case ENEMY_RESPAWN:
		{
		}
		break;
	}

	// Scene node TRS test (AGDev Assignment 1)
	/*CSceneNode* child = this->Search(CSceneNode::NODE_ENEMY_1);
	if (this)
	{
		static bool goUp = true;
		if (goUp)
		{
			this->GetTransform().m_translate.y += dt * 25.f;
			if (this->GetTransform().m_translate.y >= 50.f)
			{
				goUp = false;
			}
		}
		else
		{
			this->GetTransform().m_translate.y -= dt * 25.f;
			if (this->GetTransform().m_translate.y <= 12.5f)
			{
				goUp = true;
			}
		}
	}
	if (child)
	{
		static bool scaleUp = true;
		child->GetTransform().m_rotate.y += dt * 100.f;
		if (scaleUp)
		{
			child->GetTransform().m_scale += Vector3(dt * 0.5f, dt * 0.5f, dt * 0.5f);
			if (child->GetTransform().m_scale.x >= 2.f)
			{
				scaleUp = false;
			}
		}
		else
		{
			child->GetTransform().m_scale -= Vector3(dt * 0.5f, dt * 0.5f, dt * 0.5f);
			if (child->GetTransform().m_scale.x <= 0.5f)
			{
				scaleUp = true;
			}
		}
	}*/
}

void CEnemy::Reset()
{
	CSceneNode::Reset();
}

void CEnemy::move(double dt)
{
	if (!m_destination || m_destination == m_currentTile)
	{
		// No destination or reached destination
		m_destination = generateDestination();
	}
	else
	{
		m_transform.m_translate = Vector3::MoveToPoint(m_transform.m_translate, m_destination->GetPosition(), S_ENEMY_SPEED * dt);
		if (m_transform.m_translate == m_destination->GetPosition())
		{
			// Reached destination
			m_prev = m_currentTile;
			m_currentTile = m_destination;
		}
	}
}

CTile * CEnemy::generateDestination()
{
	vector<CTile*> possibleRoute;

	CTile* tile;

	// Left
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex(), m_currentTile->GetColIndex() - 1);
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
	}

	// Right
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex(), m_currentTile->GetColIndex() + 1);
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
	}

	// Front
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex() - 1, m_currentTile->GetColIndex());
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
	}

	// Back
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex() + 1, m_currentTile->GetColIndex());
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
	}

	if (possibleRoute.size() > 0)
	{
		int random = Math::RandIntMinMax(0, possibleRoute.size() - 1);
		return possibleRoute[random];
	}

	// Backtrack as no other route is found
	return m_prev;
}
