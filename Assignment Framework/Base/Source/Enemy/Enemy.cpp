#include "Enemy.h"
#include "..\Character\ThirdPerson.h"

float CEnemy::S_ENEMY_NORMAL_SPEED = 100.f;
float CEnemy::S_ENEMY_ESCAPE_SPEED = S_ENEMY_NORMAL_SPEED * 2.f;
float CEnemy::S_ENEMY_CALM_DOWN_TIME = 3.f;
float CEnemy::S_ENEMY_SHOOT_TIME = 2.f;
float CEnemy::S_ENEMY_RESPAWN_TIME = 3.f;
float CEnemy::S_DETECTION_RADIUS = 200.f;
int CEnemy::S_ESCAPE_PROBABILITY = 50;
CTileMap* CEnemy::S_MAP_REF = nullptr;

CEnemy::CEnemy()
	: CSceneNode()
	, m_currentFSM(ENEMY_IDLE)
	, m_dir(0, 0, 1)
	, m_destination(nullptr)
	, m_prev(nullptr)
	, m_target(nullptr)
	, m_calmDownTimer(0.f)
	, m_respawnTimer(0.f)
	, m_attackTimer(0.f)
	, m_defaultY(0.f)
{
}


CEnemy::~CEnemy()
{
	Reset();
}

void CEnemy::InitEnemyDataFromLua()
{
	CLua_Wrapper* lua = new CLua_Wrapper();
	if (!lua->OpenLua("Lua_Scripts//enemy.lua"))
	{
		return;
	}
	double* data = nullptr;

	// Normal speed
	if (data = lua->GetNumber("ENEMY_NORMAL_SPEED"))
	{
		S_ENEMY_NORMAL_SPEED = (float)(*data);
	}

	// Escape speed
	if (data = lua->GetNumber("ENEMY_ESCAPE_SPEED"))
	{
		S_ENEMY_ESCAPE_SPEED = (float)(*data);
	}

	// Calm down time
	if (data = lua->GetNumber("ENEMY_CALM_DOWN_TIME"))
	{
		S_ENEMY_CALM_DOWN_TIME = (float)(*data);
	}

	// Shoot time
	if (data = lua->GetNumber("ENEMY_SHOOT_TIME"))
	{
		S_ENEMY_SHOOT_TIME = (float)(*data);
	}

	// Respawn time
	if (data = lua->GetNumber("ENEMY_RESPAWN_TIME"))
	{
		S_ENEMY_RESPAWN_TIME = (float)(*data);
	}

	// Detection radius
	if (data = lua->GetNumber("ENEMY_DETECTION_RADIUS"))
	{
		S_DETECTION_RADIUS = (float)(*data);
	}

	// Escape probability
	if (data = lua->GetNumber("ENEMY_ESCAPE_PROBABILITY"))
	{
		S_ESCAPE_PROBABILITY = (int)(*data);
	}

	lua->CloseLua();
}

void CEnemy::Init(E_NODE_TYPE type, Mesh * mesh, CTransform * transform, CTile* currentTile, bool active, bool render)
{
	CSceneNode::Init(type, mesh, transform, currentTile, active, render);
	m_defaultY = transform->m_translate.y;
}

void CEnemy::Update(double dt)
{
	CSceneNode::Update(dt);

	if (m_calmDownTimer > 0.f)
	{
		m_calmDownTimer -= dt;
		if (m_calmDownTimer <= 0.f)
		{
			m_calmDownTimer = 0.f;
			m_currentFSM = ENEMY_PATROL;
			m_target = nullptr;
		}
	}

	switch (m_currentFSM)
	{
	case ENEMY_IDLE:
		{
			m_currentFSM = ENEMY_PATROL;
		}
		break;
	case ENEMY_ATTACK:
		{
			attack(dt);
		}
		break;
	case ENEMY_PATROL:
		{
			move(dt);
		}
		break;
	case ENEMY_ESCAPE:
		{
			escape(dt);
		}
		break;
	case ENEMY_KO:
		{
			if (m_respawnTimer > 0.f)
			{
				m_respawnTimer -= dt;
				if (m_respawnTimer <= 0.f)
				{
					m_currentFSM = ENEMY_RESPAWN;
				}
			}
			die(dt);
		}
		break;
	case ENEMY_RESPAWN:
		{
			respawn(dt);
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

void CEnemy::Alert(CSceneNode * target)
{
	if (m_currentFSM == ENEMY_ESCAPE || m_currentFSM == ENEMY_ATTACK)
	{
		return;
	}

	m_target = target;
	int random = Math::RandIntMinMax(1, 100);
	if (random <= S_ESCAPE_PROBABILITY)
	{
		// Escape
		m_currentFSM = ENEMY_ESCAPE;
	}
	else
	{
		// Attack
		m_currentFSM = ENEMY_ATTACK;
	}
	m_calmDownTimer = 0.f;
}

void CEnemy::CalmDown()
{
	m_calmDownTimer = S_ENEMY_CALM_DOWN_TIME;
}

void CEnemy::Detect(CSceneNode * target)
{
	if (m_currentFSM == ENEMY_KO || m_currentFSM == ENEMY_RESPAWN)
	{
		return;
	}

	float radiusSquared = 0.f;

	switch (m_currentFSM)
	{
	case ENEMY_IDLE:
		{
			float halfRadius = S_DETECTION_RADIUS * 0.5f;
			radiusSquared = halfRadius * halfRadius;
		}
		break;
	default:
		{
			radiusSquared = S_DETECTION_RADIUS * S_DETECTION_RADIUS;
		}
		break;
	}

	if ((m_transform.m_translate - target->GetTransform().m_translate).LengthSquared() < radiusSquared)
	{
		Alert(target);
	}
	else
	{
		CalmDown();
	}
}

void CEnemy::Kill()
{
	m_currentFSM = ENEMY_KO;
	m_respawnTimer = S_ENEMY_RESPAWN_TIME;
	m_calmDownTimer = 0.f;
}

bool CEnemy::IsAlive()
{
	if (m_currentFSM == ENEMY_KO || m_currentFSM == ENEMY_RESPAWN)
	{
		return false;
	}
	return true;
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
		Vector3 des = m_destination->GetPosition();
		des.y = m_transform.m_translate.y;
		m_transform.m_translate = Vector3::MoveToPoint(m_transform.m_translate, des, S_ENEMY_NORMAL_SPEED * dt);
		CCollider::Update(m_transform);
		if (m_transform.m_translate == des)
		{
			// Reached destination
			m_prev = m_currentTile;
			m_currentTile = m_destination;
		}
	}
}

void CEnemy::escape(double dt)
{
	if (!m_destination || m_destination == m_currentTile)
	{
		// No destination or reached destination
		m_destination = generateDestination(true);
	}
	else
	{
		Vector3 des = m_destination->GetPosition();
		des.y = m_transform.m_translate.y;
		m_transform.m_translate = Vector3::MoveToPoint(m_transform.m_translate, des, S_ENEMY_ESCAPE_SPEED * dt);
		CCollider::Update(m_transform);
		if (m_transform.m_translate == des)
		{
			// Reached destination
			m_prev = m_currentTile;
			m_currentTile = m_destination;
		}
	}
}

void CEnemy::die(double dt)
{
	float targetY = -(m_transform.m_scale.y * 0.6f);
	if (m_transform.m_translate.y > targetY)
	{
		m_transform.m_translate = Vector3::MoveToPoint(m_transform.m_translate, Vector3(m_transform.m_translate.x, targetY, m_transform.m_translate.z), S_ENEMY_NORMAL_SPEED * 0.5f * dt);
	}
}

void CEnemy::respawn(double dt)
{
	if (m_transform.m_translate.y < m_defaultY)
	{
		m_transform.m_translate = Vector3::MoveToPoint(m_transform.m_translate, Vector3(m_transform.m_translate.x, m_defaultY, m_transform.m_translate.z), S_ENEMY_NORMAL_SPEED * 0.5f * dt);
	}
	else
	{
		m_currentFSM = ENEMY_PATROL;
	}
}

void CEnemy::attack(double dt)
{
	if (CollideWith(*m_target, dt))
	{
		dynamic_cast<CThirdPerson*>(m_target)->Injure(1);
		Kill();
		m_target = nullptr;
		return;
	}
	Vector3 des = m_target->GetTransform().m_translate;
	des.y = m_transform.m_translate.y;
	m_transform.m_translate = Vector3::MoveToPoint(m_transform.m_translate, des, S_ENEMY_ESCAPE_SPEED * dt);
	CCollider::Update(m_transform);
}

CTile * CEnemy::generateDestination(bool escape)
{
	vector<CTile*> possibleRoute;

	CTile* tile = nullptr;

	CTile* selectedTile = nullptr;
	float distSquared = 0.f;

	// Left
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex(), m_currentTile->GetColIndex() - 1);
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
		if (escape)
		{
			distSquared = (tile->GetPosition() - m_target->GetTransform().m_translate).LengthSquared();
			selectedTile = tile;
		}
	}

	// Right
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex(), m_currentTile->GetColIndex() + 1);
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
		if (escape)
		{
			float tempDistSquared = (tile->GetPosition() - m_target->GetTransform().m_translate).LengthSquared();
			if (tempDistSquared > distSquared)
			{
				distSquared = tempDistSquared;
				selectedTile = tile;
			}
		}
	}

	// Front
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex() - 1, m_currentTile->GetColIndex());
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
		if (escape)
		{
			float tempDistSquared = (tile->GetPosition() - m_target->GetTransform().m_translate).LengthSquared();
			if (tempDistSquared > distSquared)
			{
				distSquared = tempDistSquared;
				selectedTile = tile;
			}
		}
	}

	// Back
	tile = S_MAP_REF->FetchTile(m_currentTile->GetRowIndex() + 1, m_currentTile->GetColIndex());
	if (tile->GetWalkable() && tile != m_prev)
	{
		// Can walk into
		possibleRoute.push_back(tile);
		if (escape)
		{
			float tempDistSquared = (tile->GetPosition() - m_target->GetTransform().m_translate).LengthSquared();
			if (tempDistSquared > distSquared)
			{
				distSquared = tempDistSquared;
				selectedTile = tile;
			}
		}
	}

	if (escape)
	{
		if (selectedTile)
		{
			return selectedTile;
		}
	}
	else
	{
		if (possibleRoute.size() > 0)
		{
			int random = Math::RandIntMinMax(0, possibleRoute.size() - 1);
			return possibleRoute[random];
		}
	}

	// Backtrack as no other route is found
	return m_prev;
}
