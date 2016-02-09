#ifndef TILE_MAP_H
#define TILE_MAP_H

#include "Tile.h"
#include <vector>

using std::vector;

class CTileMap
{
public:
	CTileMap(int numRow, int numCol);
	~CTileMap();

	void Init(int numRow, int numCol);
	void Update(double dt);
	void Exit();
	void Reset();

	void AddToMap(CTile* tile);
	CTile* FetchTile(int rowIndex, int colIndex);

	// Setters and Getters
	void SetRow(int numRow);
	int GetRow();

	void SetCol(int numCol);
	int GetCol();

private:
	vector<CTile*> m_map;
	int m_numRow, m_numCol;
};

#endif