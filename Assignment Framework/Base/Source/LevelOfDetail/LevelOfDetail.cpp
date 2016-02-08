#include "LevelOfDetail.h"



CLevelOfDetail::CLevelOfDetail()
{
	for (int i = 0; i < NUM_RES; ++i)
	{
		m_resList[i] = nullptr;
	}
}


CLevelOfDetail::~CLevelOfDetail()
{
}

void CLevelOfDetail::Init(Mesh * resList[NUM_RES])
{
}

Mesh * CLevelOfDetail::Update(double dt, CTransform * me, CTransform * target)
{
	return nullptr;
}
