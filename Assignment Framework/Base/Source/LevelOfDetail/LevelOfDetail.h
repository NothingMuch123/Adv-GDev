#ifndef _LEVEL_OF_DETAIL_
#define _LEVEL_OF_DETAIL_

#include "Mesh.h"
#include "..\Object\GameObject.h"

class CLevelOfDetail
{
public:
	enum RESOLUTION_TYPE
	{
		RES_LOW,
		RES_MID,
		RES_HIGH,
		RES_ULTRA,
		NUM_RES,
	};

	CLevelOfDetail();
	~CLevelOfDetail();

	void Init(Mesh* resList[NUM_RES]);
	Mesh* Update(double dt, CTransform* me, CTransform* target); // Update and return the proper mesh

private:
	Mesh* m_resList[NUM_RES];
	float m_dist[NUM_RES];
};

#endif