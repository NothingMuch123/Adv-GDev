#include "SpatialPartition.h"



CSpatialPartition::CSpatialPartition()
	: m_gridList(NULL)
	, m_numRow(0)
	, m_numCol(0)
	, m_totalWidth(0.f)
	, m_totalHeight(0.f)
{
}


CSpatialPartition::~CSpatialPartition()
{
}

void CSpatialPartition::Init(int numRow, int numCol, float totalWidth, float totalHeight, Mesh* mesh)
{
	float gridSizeWidth = totalWidth / numCol;
	float gridSizeHeight = totalHeight / numRow;
	Vector3 size(gridSizeWidth, 1.f, gridSizeHeight);

	Vector3 defaultStartPos(-totalWidth * 0.5f, 0.f, -totalHeight * 0.5f); // Top left of the grid
	defaultStartPos += size * 0.5f; // Middle of the grid
	Vector3 startPos;

	for (int row = 0; row < numRow; ++row)
	{
		for (int col = 0; col < numCol; ++col)
		{
			// Set start pos
			startPos.x = defaultStartPos.x + (col * size.x);
			startPos.y = 0.f;
			startPos.z = (-totalHeight * 0.5f) + (row * size.z);

			CGrid* newGrid = new CGrid();
			CTransform* transform = new CTransform();
			transform->Init(startPos, Vector3(), size);
			newGrid->CGrid::Init(mesh, transform);
			m_gridList.push_back(newGrid);
		}
	}

}

void CSpatialPartition::Update(const double dt)
{
	for (int i = 0; i < m_gridList.size(); ++i)
	{
		CGrid* grid = m_gridList[i];
		grid->Update(dt);
	}
}

void CSpatialPartition::AddObject(CSceneNode * object)
{
	Vector3 objPos = object->GetTransform().m_translate;
	float gridSizeWidth = m_totalWidth / m_numCol;
	float gridSizeHeight = m_totalHeight / m_numRow;

	int grid_index_row = objPos.z / gridSizeHeight;
	int grid_index_col = objPos.x / gridSizeWidth;

	if (grid_index_row * m_numRow + grid_index_col < m_gridList.size())
	{
		m_gridList[grid_index_row * m_numRow + grid_index_col]->AddToList(object);
	}
}

vector<CGrid*>& CSpatialPartition::GetGridList()
{
	return m_gridList;
}
