#ifndef TILE_H
#define TILE_H

#include "Vector3.h"

class CTile
{
public:
	CTile(Vector3 position, int rowIndex, int colIndex, bool walkable = true);
	~CTile();

	void Init(Vector3 position, int rowIndex, int colIndex, bool walkable = true);
	void Update(double dt);
	void Exit();
	void Reset();

	// Setters and Getters
	void SetPosition(Vector3 position);
	Vector3& GetPosition();

	void SetWalkable(bool walkable);
	bool GetWalkable();

private:
	Vector3 m_position;
	bool m_walkable;
	int m_rowIndex, m_colIndex;
};

#endif