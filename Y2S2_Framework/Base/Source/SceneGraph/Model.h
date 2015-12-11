#pragma once

#include "Vector3.h"
#include "GL\glew.h"
#include "..\Mesh.h"
#include "Mtx44.h"

class CModel
{
public:
	enum RESOLUTION_TYPE
	{
		RES_LOW,
		RES_MID,
		RES_HIGH,
		NUM_RES,
	};

private:
	Vector3 vTopLeft, vBottomRight;
	float red, green, blue;
	
	// Handle to the Mesh
	Mesh* m_cModelMesh;
	Mesh** theArrayOfMeshes;

	// Current resolution
	RESOLUTION_TYPE m_iCurrentResolution;

public:
	CModel(void);
	~CModel(void);

	void Init(void);
	void Draw(bool m_bLight = false);
	void SetColor(const float red, const float green, const float blue);

	// Get the top left of the bounding box of the instance
	Vector3 GetTopLeft(void);
	// Get the bottomright of the bounding box of the instance
	Vector3 GetBottomRight(void);

	// Get current resolution
	int GetResolution();
	// Set current resolution
	void SetResolution(const RESOLUTION_TYPE type);


};
