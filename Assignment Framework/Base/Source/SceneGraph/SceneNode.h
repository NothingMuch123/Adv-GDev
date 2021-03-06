#ifndef _SCENENODE_H_
#define _SCENENODE_H_

#include "..\Object\GameObject.h"
#include "..\TileSystem\Tile.h"
#include "..\LevelOfDetail\LevelOfDetail.h"
#include <vector>

using std::vector;

class CGrid;
class AGDev_Assign01;

class CSceneNode : public CGameObject
{
public:
	enum E_NODE_TYPE
	{
		NODE_NONE = 0,
		NODE_TEST,
		NODE_WALL,
		NODE_END,
		NODE_ENEMY,
		NODE_ENEMY_1,
		NODE_ENEMY_2,
		NUM_NODE,
	};

	CSceneNode();
	virtual ~CSceneNode();

	virtual void Init(E_NODE_TYPE type, Mesh* mesh, CTransform* transform, CTile* currentTile = nullptr, bool active = true, bool render = true);
	void InitLOD(Mesh* resList[CLevelOfDetail::NUM_RES], float m_dist[CLevelOfDetail::NUM_RES]);
	virtual void Update(const double dt);
	void UpdateLOD(double dt, CSceneNode* target);
	virtual void Reset();

	// Setters and Getters
	vector<CSceneNode*>& GetChildren();

	void AddLocation(CGrid* grid);
	vector<CGrid*>& GetLocations();
	void ClearLocations();
	
	void SetType(E_NODE_TYPE type);
	E_NODE_TYPE GetType();

	void Draw(AGDev_Assign01* scene);

	void AddChild(CSceneNode* newNode);
	void AddChild(E_NODE_TYPE type, CSceneNode* newNode);

	CSceneNode* Search(E_NODE_TYPE searchType);

	// Check a position for collision with objects and its child
	bool CheckForCollision(Vector3 pos);

	// Check two positions for collision with objects in any of the grid
	int CheckForCollision(Vector3 position_start, Vector3 position_end, Vector3 &Hit);

	void SetTile(CTile* tile);
	CTile* GetTile();

private:
	// Check where a line segment between two positions intersects a plane
	int GetIntersection(float fDst1, float fDst2, Vector3 P1, Vector3 P2, Vector3 &Hit);
	// Check two positions are within a box region
	int InBox(Vector3 Hit, Vector3 B1, Vector3 B2, const int Axis);

protected:
	vector<CSceneNode*> m_children;
	E_NODE_TYPE m_type;
	vector<CGrid*> m_locations; // Grid that stores this gameobject
	CTile* m_currentTile;

	// Level of Details
	CLevelOfDetail* m_lod;
};

#endif