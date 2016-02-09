#include "TileMap.h"



CTileMap::CTileMap(int numRow, int numCol)
	: m_numRow(numRow)
	, m_numCol(numCol)
	, m_map(NULL)
{
}


CTileMap::~CTileMap()
{
	Exit();
}

void CTileMap::Init(int numRow, int numCol)
{
	m_numRow = numRow;
	m_numCol = numCol;
}

void CTileMap::Update(double dt)
{
	for (vector<CTile*>::iterator it = m_map.begin(); it != m_map.end(); ++it)
	{
		CTile* tile = *it;
		tile->Update(dt);
	}
}

void CTileMap::Exit()
{
	while (m_map.size() > 0)
	{
		CTile* tile = m_map.back();
		if (tile)
		{
			delete tile;
		}
		m_map.pop_back();
	}
}

void CTileMap::Reset()
{
	Exit();
	m_numRow = m_numCol = 0;
}

void CTileMap::AddToMap(CTile * tile)
{
	m_map.push_back(tile);
}

CTile * CTileMap::FetchTile(int rowIndex, int colIndex)
{
	if (rowIndex >= m_numRow || colIndex >= m_numCol || rowIndex < 0 || colIndex < 0)
	{
		return nullptr;
	}
	return m_map[(rowIndex * m_numCol) + colIndex];
}

void CTileMap::SetRow(int numRow)
{
	m_numRow = numRow;
}

int CTileMap::GetRow()
{
	return m_numRow;
}

void CTileMap::SetCol(int numCol)
{
	m_numCol = numCol;
}

int CTileMap::GetCol()
{
	return m_numCol;
}
