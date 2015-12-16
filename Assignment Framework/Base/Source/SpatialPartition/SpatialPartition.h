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

	vector<CGrid*>& GetGridList();

private:
	vector<CGrid*> m_gridList;
	int m_numRow; // Number of grid on z axis
	int m_numCol; // Number of grid on x axis
	float m_totalWidth; // X axis
	float m_totalHeight; // Z axis
};

#endif