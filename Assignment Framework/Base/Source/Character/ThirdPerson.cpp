#include "ThirdPerson.h"
#include "MyMath.h"

const float CThirdPerson::S_OFFSET_BACK = 20.f;
const float CThirdPerson::S_OFFSET_LEFT = 10.f;
const float CThirdPerson::S_OFFSET_UP = 20.f;

CThirdPerson::CThirdPerson()
{
}


CThirdPerson::~CThirdPerson()
{
}

void CThirdPerson::Init(Camera3 * view, Mesh * mesh, CTransform * transform, bool active, bool render)
{
	CFirstPerson::Init(view, mesh, transform, active, render);
	
	// Generate third person cam
	m_TPview = new Camera3();
	Vector3 pos = calcTPviewPos();
	float angle = Math::RadianToDegree(acosf(S_OFFSET_UP / (pos - view->target).Length()));
	m_TPview->total_pitch = -angle;
	Vector3 right = (m_view->target - m_view->position).Normalized().Cross(view->up);
	m_TPview->Init(pos, view->target, right.Cross((view->target - pos).Normalized()));

	// Update transform with first person view
	updateTransformPos();
}

void CThirdPerson::UpdateTPView()
{
}

Vector3 CThirdPerson::calcTPviewPos()
{
	// All dir are based off first person view
	Vector3 backDir = (m_view->position - m_view->target).Normalized(); // This is essentially negative of view (-view)
	Vector3 leftDir = m_view->up.Cross(-backDir);
	Vector3 upDir = m_view->up;
	Vector3 TPview_pos;
	TPview_pos += leftDir * S_OFFSET_LEFT;
	TPview_pos += backDir * S_OFFSET_BACK;
	TPview_pos += upDir * S_OFFSET_UP;

	return TPview_pos;
}

Camera3 * CThirdPerson::GetTPView()
{
	return m_TPview;
}
