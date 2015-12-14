#ifndef _FIRSTPESON_H_
#define _FIRSTPESON_H_

#include "..\Object\GameObject.h"
#include "..\Camera3.h"

class CFirstPerson : public CGameObject
{
public:
	CFirstPerson();
	virtual ~CFirstPerson();

	virtual void Init(Camera3* view, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);

	void SetView(Camera3* view);
	Camera3* GetView();

protected:
	void updateViewPos();		// Update view pos based on position
	void updateTransformPos();	// Update transform pos based on view

protected:
	Camera3* m_view;
};

#endif