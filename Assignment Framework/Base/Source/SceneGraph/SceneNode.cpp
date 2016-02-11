#include "SceneNode.h"
#include "..\Scene\AGDev_Assign01.h"


CSceneNode::CSceneNode()
	: m_children(NULL)
	, m_type(NODE_NONE)
	, m_locations(NULL)
	, m_currentTile(nullptr)
	, m_lod(nullptr)
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

void CSceneNode::Init(E_NODE_TYPE type, Mesh * mesh, CTransform * transform, CTile* currentTile, bool active, bool render)
{
	CGameObject::Init(mesh, transform, active, render);
	CSceneNode::m_type = type;
	m_currentTile = currentTile;
}

void CSceneNode::InitLOD(Mesh * resList[CLevelOfDetail::NUM_RES], float m_dist[CLevelOfDetail::NUM_RES])
{
	if (!m_lod)
	{
		m_lod = new CLevelOfDetail();
		m_lod->Init(resList, m_dist);
	}
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

void CSceneNode::UpdateLOD(double dt, CSceneNode * target)
{
	if (m_lod)
	{
		Mesh* newMesh = m_lod->Update(dt, &m_transform, &target->GetTransform());
		m_mesh = newMesh;
		for (vector<CSceneNode*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
		{
			CSceneNode* n = *it;
			n->SetMesh(newMesh);
		}
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

void CSceneNode::AddLocation(CGrid * grid)
{
	m_locations.push_back(grid);
}

vector<CGrid*> & CSceneNode::GetLocations()
{
	return m_locations;
}

void CSceneNode::ClearLocations()
{
	m_locations.clear();
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

/*
CSceneNode* CSceneNode::CheckForCollision(Vector3 position)
{
	if (!active)
	{
		return nullptr;
	}

	for (auto node = theChildren.begin(); node != theChildren.end(); ++node)
	{
		CSceneNode* sNode = dynamic_cast<CSceneNode*>(*node);

		if (sNode)
		{
			// Calculate the relative position of the "position"
			Vector3 relativePos = theTransform->GetTransform().GetInverse() * position;

			if (CSceneNode* node = sNode->CheckForCollision(relativePos))
			{
				return node;
			}
		}
	}

	Vector3 ObjectTopLeft = GetTopLeft();
	Vector3 ObjectBottomRight = GetBottomRight();

	if (((ObjectTopLeft.x > position.x) && (ObjectTopLeft.y > position.y) && (ObjectTopLeft.z > position.z)) &&
		((ObjectBottomRight.x < position.x) && (ObjectBottomRight.y < position.y) && (ObjectBottomRight.z < position.z)))
	{
		return this;
	}
	return nullptr;
}
*/

bool CSceneNode::CheckForCollision(Vector3 pos)
{
	if (!CObject::m_active)
	{
		return false;
	}

	/*for (vector<CSceneNode*>::iterator it = m_children.begin(); it != m_children.end(); ++it)
	{
		CSceneNode* node = *it;
		if (node)
		{
			Vector3 relativePos = GetTransform().GetMtx().GetInverse() * pos;
			if (node->CheckForCollision(relativePos))
			{
				return true;
			}
		}
	}*/

	Vector3 ObjMax = GetTransform().GetMtx() * Vector3(1, 1, 1);
	Vector3 ObjMin = GetTransform().GetMtx() * Vector3(-1, -1, -1);

	if (pos.x < ObjMin.x || pos.x > ObjMax.x || pos.y < ObjMin.y || pos.y > ObjMax.y || pos.z < ObjMin.z || pos.z > ObjMax.z)
	{
		return false;
	}
	return true;
}

int CSceneNode::CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 & Hit)
{
	if (!CObject::m_active)
	{
		return false;
	}
	Vector3 ObjMax = GetTransform().GetMtx() * Vector3(1, 1, 1);
	Vector3 ObjMin = GetTransform().GetMtx() * Vector3(-1, -1, -1);

	if (position_end.x < ObjMin.x && position_start.x < ObjMin.x)
		return false;
	if (position_end.x > ObjMax.x && position_start.x > ObjMax.x)
		return false;
	if (position_end.y < ObjMin.y && position_start.y < ObjMin.y)
		return false;
	if (position_end.y > ObjMax.y && position_start.y > ObjMax.y)
		return false;
	if (position_end.z < ObjMin.z && position_start.z < ObjMin.z)
		return false;
	if (position_end.z > ObjMax.z && position_start.z > ObjMax.z)
		return false;
	if (position_start.x > ObjMin.x && position_start.x < ObjMax.x &&
		position_start.y > ObjMin.y && position_start.y < ObjMax.y &&
		position_start.z > ObjMin.z && position_start.z < ObjMax.z)
	{
		Hit = position_start;
		return true;
	}
	if ((GetIntersection(position_start.x - ObjMin.x, position_end.x - ObjMin.x, position_start, position_end, Hit) && InBox(Hit, ObjMin, ObjMax, 1))
		|| (GetIntersection(position_start.y - ObjMin.y, position_end.y - ObjMin.y, position_start, position_end, Hit) && InBox(Hit, ObjMin, ObjMax, 2))
		|| (GetIntersection(position_start.z - ObjMin.z, position_end.z - ObjMin.z, position_start, position_end, Hit) && InBox(Hit, ObjMin, ObjMax, 3))
		|| (GetIntersection(position_start.x - ObjMax.x, position_end.x - ObjMax.x, position_start, position_end, Hit) && InBox(Hit, ObjMin, ObjMax, 1))
		|| (GetIntersection(position_start.y - ObjMax.y, position_end.y - ObjMax.y, position_start, position_end, Hit) && InBox(Hit, ObjMin, ObjMax, 2))
		|| (GetIntersection(position_start.z - ObjMax.z, position_end.z - ObjMax.z, position_start, position_end, Hit) && InBox(Hit, ObjMin, ObjMax, 3)))
		return true;

	return false;
}

void CSceneNode::SetTile(CTile * tile)
{
	m_currentTile = tile;
}

CTile * CSceneNode::GetTile()
{
	return m_currentTile;
}

int CSceneNode::GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 & Hit)
{
	if ((fDst1 * fDst2) >= 0.f)
	{
		return 0;
	}
	if (fDst1 == fDst2)
	{
		return 0;
	}
	Hit = P1 + (P2 - P1) * (-fDst1 / (fDst2 - fDst1));
	return 1;
}

int CSceneNode::InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis)
{
	if (Axis == 1 && Hit.z > B1.z && Hit.z < B2.z && Hit.y > B1.y && Hit.y < B2.y)
	{
		return 1;
	}
	if (Axis == 2 && Hit.z > B1.z && Hit.z < B2.z && Hit.x > B1.x && Hit.x < B2.x)
	{
		return 1;
	}
	if (Axis == 3 && Hit.x > B1.x && Hit.x < B2.x && Hit.y > B1.y && Hit.y < B2.y)
	{
		return 1;
	}
	return 0;
}
