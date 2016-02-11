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

void CLevelOfDetail::Init(Mesh * resList[NUM_RES], float dist[NUM_RES])
{
	for (int i = 0; i < NUM_RES; ++i)
	{
		m_resList[i] = resList[i];
		m_dist[i] = dist[i];
	}
}

Mesh * CLevelOfDetail::Update(double dt, CTransform * me, CTransform * target)
{
	float distSquared = (me->m_translate - target->m_translate).LengthSquared();
	if (distSquared <= m_dist[RES_ULTRA] * m_dist[RES_ULTRA])
	{
		return m_resList[RES_ULTRA];
	}
	else if (distSquared <= m_dist[RES_HIGH] * m_dist[RES_HIGH] && distSquared > m_dist[RES_ULTRA] * m_dist[RES_ULTRA])
	{
		return m_resList[RES_HIGH];
	}
	else if (distSquared <= m_dist[RES_MID] * m_dist[RES_MID] && distSquared > m_dist[RES_HIGH] * m_dist[RES_HIGH])
	{
		return m_resList[RES_MID];
	}
	else if (distSquared <= m_dist[RES_LOW] * m_dist[RES_LOW] && distSquared > m_dist[RES_MID] * m_dist[RES_MID])
	{
		return m_resList[RES_LOW];
	}
	else
	{
		return nullptr;
	}
}
