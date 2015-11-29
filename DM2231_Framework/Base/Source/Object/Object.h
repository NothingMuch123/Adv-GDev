#ifndef OBJECT_H
#define OBJECT_H

#include "../Mesh.h"

class CObject
{
private:
	Mesh* m_mesh;

public:
	CObject();
	~CObject();

	virtual void Init(Mesh* mesh);
	virtual void Update(const double dt);
	virtual void Reset();

	// Setters and Getters
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();
};

#endif