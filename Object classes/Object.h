#ifndef OBJECT_H
#define OBJECT_H

#include "../Mesh.h"

class CObject
{
private:
	Mesh* m_mesh;
	bool m_render, m_active;

public:
	CObject();
	~CObject();

	virtual void Init(Mesh* mesh, bool active, bool render);
	virtual void Update(const double dt);
	virtual void Reset();

	// Setters and Getters
	void SetMesh(Mesh* mesh);
	Mesh* GetMesh();

	void SetRender(bool render);
	bool GetRender();

	void SetActive(bool active);
	bool GetActive();
};

#endif