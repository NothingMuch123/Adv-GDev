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
	m_numCol = numCol;
	m_numRow = numRow;
	m_totalWidth = totalWidth;
	m_totalHeight = totalHeight;

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
			startPos.z = defaultStartPos.z + (row * size.z);

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

	int grid_index_row = Math::FAbs((objPos.z + m_totalHeight * 0.5f) / gridSizeHeight);
	int grid_index_col = Math::FAbs((objPos.x - m_totalWidth * 0.5f) / gridSizeWidth);

	if (grid_index_row * m_numCol + grid_index_col < m_gridList.size())
	{
		m_gridList[grid_index_row * m_numRow + grid_index_col]->AddToList(object);
	}
}

void CSpatialPartition::UpdateObject(CSceneNode * object)
{
	if (!object->GetLocation())
	{
		return;
	}
	Vector3 objPos = object->GetTransform().m_translate;
	float gridSizeWidth = m_totalWidth / m_numCol;
	float gridSizeHeight = m_totalHeight / m_numRow;

	int grid_index_row = Math::FAbs((objPos.z + m_totalHeight * 0.5f) / gridSizeHeight);
	int grid_index_col = Math::FAbs((objPos.x + m_totalWidth * 0.5f) / gridSizeWidth);

	CGrid* grid = GetGrid(grid_index_col, grid_index_row);
	if (grid && object->GetLocation() != grid)
	{
		object->GetLocation()->Remove(object); // Remove from grid
		if (grid_index_row * m_numCol + grid_index_col < m_gridList.size())
		{
			m_gridList[grid_index_row * m_numRow + grid_index_col]->AddToList(object);
		}
	}
}

CGrid * CSpatialPartition::GetGrid(int col, int row)
{
	if (row * m_numCol + col >= m_gridList.size())
	{
		return NULL;
	}
	return m_gridList[row * m_numCol + col];
}

vector<CGrid*>& CSpatialPartition::GetGridList()
{
	return m_gridList;
}

bool CSpatialPartition::CheckForCollision(Vector3 pos)
{
	float gridSizeWidth = m_totalWidth / m_numCol;
	float gridSizeHeight = m_totalHeight / m_numRow;
	int grid_index_row = Math::FAbs((pos.z + m_totalHeight * 0.5f) / gridSizeHeight);
	int grid_index_col = Math::FAbs((pos.x + m_totalWidth * 0.5f) / gridSizeWidth);

	//int GridIndex_x = ((int)pos.x / (xSize*xNumOfGrid));
	//int GridIndex_z = ((int)pos.x / (ySize*yNumOfGrid));

	// Calculate the index of each position
	int GridIndex = grid_index_col* m_numRow + grid_index_row;

	// Add them to each grid
	if ((GridIndex >= 0) && (GridIndex< m_numCol * m_numRow))
	{
		vector<CSceneNode*> theListOfObjects = m_gridList[GridIndex]->GetList();

		Vector3 ObjectTopLeft, ObjectBottomRight;
		for (int i = 0; i<(int)theListOfObjects.size(); i++)
		{
			bool result = theListOfObjects[i]->CheckForCollision(pos);
			if (result)
			{
				return result;
			}
		}
	}

	return false;
}

bool CSpatialPartition::CheckForCollision(Vector3 pos_start, Vector3 pos_End)
{
	float gridSizeWidth = m_totalWidth / m_numCol;
	float gridSizeHeight = m_totalHeight / m_numRow;
	int grid_index_row = Math::FAbs((pos_start.z + m_totalHeight * 0.5f) / gridSizeHeight);
	int grid_index_col = Math::FAbs((pos_start.x + m_totalWidth * 0.5f) / gridSizeWidth);

	//int GridIndex_x = ((int)pos_start.x / (xSize*xNumOfGrid));
	//int GridIndex_z = ((int)pos_start.x / (ySize*yNumOfGrid));

	// Calculate the index of each position
	int GridIndex = grid_index_col*m_numRow + grid_index_row;
	pos_start.y = 0.0f;
	// Add them to each grid
	if ((GridIndex >= 0) && (GridIndex<m_numCol*m_numRow))
	{
		vector<CSceneNode*> theListOfObjects = m_gridList[GridIndex]->GetList();

		Vector3 ObjectTopLeft, ObjectBottomRight;
		for (int i = 0; i<(int)theListOfObjects.size(); i++)
		{
			Vector3 hits = Vector3(0, 0, 0);
			return theListOfObjects[i]->CheckForCollision(pos_start, pos_End, hits);
		}
	}

	return false;
}
