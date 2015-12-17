#ifndef _SPATIALPARTITION_H_
#define _SPATIALPARTITION_H_

#include "Grid.h"

class CSpatialPartition
{
public:
	CSpatialPartition();
	~CSpatialPartition();

	void Init(int numRow, int numCol, float totalWidth, float totalHeight, Mesh* mesh = NULL);
	void Update(const double dt);

	void AddObject(CSceneNode* object);
	void UpdateObject(CSceneNode* object);

	Vector3 GetGridIndex(Vector3 pos);

	CGrid* GetGrid(int col, int row);
	vector<CGrid*>& GetGridList();

	// Collision
	bool CheckForCollision(Vector3 pos);
	bool CheckForCollision(Vector3 pos_start, Vector3 pos_End);

	float GetTotalWidth();
	float GetTotalHeight();

private:
	vector<CGrid*> m_gridList;
	int m_numRow; // Number of grid on z axis
	int m_numCol; // Number of grid on x axis
	float m_totalWidth; // X axis
	float m_totalHeight; // Z axis
};

#endif