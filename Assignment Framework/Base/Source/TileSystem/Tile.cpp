#include "Tile.h"



CTile::CTile(Vector3 position, int rowIndex, int colIndex, bool walkable)
	: m_position(position)
	, m_walkable(walkable)
	, m_rowIndex(rowIndex)
	, m_colIndex(colIndex)
{
}


CTile::~CTile()
{
	Exit();
}

void CTile::Init(Vector3 position, int rowIndex, int colIndex, bool walkable)
{
	m_position = position;
	m_walkable = walkable;
	m_rowIndex = rowIndex;
	m_colIndex = colIndex;
}

void CTile::Update(double dt)
{
}

void CTile::Exit()
{
}

void CTile::Reset()
{
	m_position = Vector3::ZERO_VECTOR;
	m_walkable = true;
	m_rowIndex = m_colIndex = -1;
}

void CTile::SetPosition(Vector3 position)
{
	m_position = position;
}

Vector3 & CTile::GetPosition()
{
	return m_position;
}

void CTile::SetWalkable(bool walkable)
{
	m_walkable = walkable;
}

bool CTile::GetWalkable()
{
	return m_walkable;
}

void CTile::SetRowIndex(int rowIndex)
{
	m_rowIndex = rowIndex;
}

int CTile::GetRowIndex()
{
	return m_rowIndex;
}

void CTile::SetColIndex(int colIndex)
{
	m_colIndex = colIndex;
}

int CTile::GetColIndex()
{
	return m_colIndex;
}
