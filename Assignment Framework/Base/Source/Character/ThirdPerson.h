#ifndef _THIRDPERSON_H_
#define _THIRDPERSON_H_

#include "FirstPerson.h"

class CThirdPerson : public CFirstPerson
{
public:
	CThirdPerson();
	virtual ~CThirdPerson();

	virtual void Init(Camera3* view, Mesh* mesh, CTransform* transform, bool active = true, bool render = true);
	
	void UpdateTPView();

	Camera3* GetTPView();

protected:
	Vector3 calcTPviewPos();

protected:
	// Offset between first person view position and third person view position
	static const float S_OFFSET_BACK;	// Offset for z axis
	static const float S_OFFSET_LEFT;	// Offset for x axis
	static const float S_OFFSET_UP;		// Offset for y axis

	Camera3* m_TPview; // Third person view camera
};

#endif