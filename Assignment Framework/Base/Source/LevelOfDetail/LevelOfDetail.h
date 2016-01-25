#ifndef _LEVEL_OF_DETAIL_
#define _LEVEL_OF_DETAIL_

#include "Mesh.h"

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

private:
	Mesh* m_resList[NUM_RES];
};

#endif