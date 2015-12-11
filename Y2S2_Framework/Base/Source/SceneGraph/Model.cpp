#include "Model.h"

#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#include <windows.h> // Header File For Windows
#include <gl\gl.h> // Header File For The OpenGL32 Library
#include <gl\glu.h> // Header File For The GLu32 Library

#include <stdio.h>
#include "..\MeshBuilder.h"

CModel::CModel(void)
	: m_cModelMesh(NULL)
	, m_iCurrentResolution(RES_HIGH)
	, theArrayOfMeshes(NULL)
{
	theArrayOfMeshes = new Mesh*[NUM_RES];
	for (int i = 0; i < NUM_RES; ++i)
	{
		theArrayOfMeshes[i] = NULL;
	}

	vTopLeft = Vector3( 1.0f, 1.0f, 1.0f );
	vBottomRight = Vector3( -1.0f, -1.0f, -1.0f );
	red = 128;
	green = 255;
	blue = 0;
}

CModel::~CModel(void)
{
	if (m_cModelMesh != NULL)
	{
		delete m_cModelMesh;
		m_cModelMesh = NULL;
	}
	if (theArrayOfMeshes)
	{
		for (int i = 0; i < NUM_RES; ++i)
		{
			if (theArrayOfMeshes[i])
			{
				delete theArrayOfMeshes[i];
				theArrayOfMeshes[i] = NULL;
			}
		}
		delete[] theArrayOfMeshes;
		theArrayOfMeshes = NULL;
	}
}

void CModel::Init(void)
{
	m_cModelMesh = MeshBuilder::GenerateCube("cube", Color(1, 0, 0));
	//m_cModelMesh = MeshBuilder::GenerateCone("cone", Color(0.5f, 1, 0.3f), 36, 10.f, 10.f);

	// Create low res sphere
	theArrayOfMeshes[RES_LOW] = MeshBuilder::GenerateSphere("Sphere_LowRes", Color(0, 0, 1), 6, 12, 0.5f);
	// Create mid res sphere
	theArrayOfMeshes[RES_MID] = MeshBuilder::GenerateSphere("Sphere_MidRes", Color(0, 1, 0), 12, 24, 0.5f);
	// Create high res sphere
	theArrayOfMeshes[RES_HIGH] = MeshBuilder::GenerateSphere("Sphere_HighRes", Color(1, 1, 0), 18, 36, 0.5f);
}

void CModel::Draw(bool m_bLight)
{
	theArrayOfMeshes[m_iCurrentResolution]->Render();

	//if (m_cModelMesh)
		//m_cModelMesh->Render();
}

void CModel::SetColor(const float red, const float green, const float blue)
{
	this->red = red;
	this->green = green;
	this->blue = blue;
}

// Get the top left of the bounding box of the instance
Vector3 CModel::GetTopLeft(void)
{
	return Vector3(vTopLeft.x, vTopLeft.y, vTopLeft.z);
}

// Get the bottomright of the bounding box of the instance
Vector3 CModel::GetBottomRight(void)
{
	return Vector3(vBottomRight.x, vBottomRight.y, vBottomRight.z);
}

int CModel::GetResolution()
{
	return m_iCurrentResolution;
}

void CModel::SetResolution(const RESOLUTION_TYPE type)
{
	m_iCurrentResolution = type;
}
