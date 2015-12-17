#include "ThirdPerson.h"
#include "MyMath.h"

const float CThirdPerson::S_MIN_OFFSET_TARGET = 50.f;
const float CThirdPerson::S_MAX_OFFSET_TARGET = S_MIN_OFFSET_TARGET * 3.f;
float CThirdPerson::S_OFFSET_TARGET = S_MIN_OFFSET_TARGET;

const float CThirdPerson::S_OFFSET_BACK = 50.f;
const float CThirdPerson::S_OFFSET_LEFT = 30.f;
const float CThirdPerson::S_OFFSET_UP = 45.f;

const float CThirdPerson::S_NORMAL_SPEED = 100.f;
const float CThirdPerson::S_SPRINT_SPEED = S_NORMAL_SPEED * 2.f;

CThirdPerson::CThirdPerson()
	: m_speed(S_NORMAL_SPEED)
	, m_targetSpeed(m_speed * 2.f)
{
}


CThirdPerson::~CThirdPerson()
{
	if (m_TPview)
	{
		delete m_TPview;
		m_TPview = NULL;
	}
}

void CThirdPerson::Init(E_NODE_TYPE type, Camera3 * view, Mesh * mesh, CTransform * transform, bool active, bool render)
{
	CFirstPerson::Init(type, view, mesh, transform, active, render);
	
	// Generate third person cam
	m_TPview = new Camera3();
	generateTPview(view);

	// Update transform with first person view
	updateTransformPos();
}

void CThirdPerson::Look(double dt, float yaw, float pitch)
{
	m_view->Yaw(dt, yaw);
	m_view->Pitch(dt, pitch);

	m_transform.m_rotate.y -= yaw * Camera3::CAMERA_SPEED * dt;

	generateTPview(m_view);
}

void CThirdPerson::MoveForward(double dt)
{
	changeTargetOffset(dt, true);
	m_TPview->MoveForward_Backward(dt, false);
	Vector3 tempTarget = m_TPview->target;
	tempTarget.y = m_TPview->position.y;
	Vector3 view = (tempTarget - m_TPview->position).Normalized();
	float movement = m_speed * dt;
	Vector3 V3Movement = view * movement;
	m_view->position += V3Movement;
	m_view->target += V3Movement;
	Look(dt, 0.0001f, 0.f);
	updateTransformPos();
}

void CThirdPerson::MoveBackward(double dt)
{
	changeTargetOffset(dt, true);
	m_TPview->MoveForward_Backward(dt, true);
	Vector3 tempTarget = m_TPview->target;
	tempTarget.y = m_TPview->position.y;
	Vector3 view = (m_TPview->position - tempTarget).Normalized();
	float movement = m_speed * dt;
	Vector3 V3Movement = view * movement;
	m_view->position += V3Movement;
	m_view->target += V3Movement;
	Look(dt, 0.0001f, 0.f);
	updateTransformPos();
}

void CThirdPerson::MoveLeft(double dt)
{
	changeTargetOffset(dt, true);
	m_TPview->MoveLeft_Right(dt, true);
	Vector3 tempTarget = m_TPview->target;
	tempTarget.y = m_TPview->position.y;
	Vector3 view = (tempTarget - m_TPview->position).Normalized();
	Vector3 right = view.Cross(m_TPview->up).Normalized();
	float movement = m_speed * dt;
	Vector3 V3Movement = -right * movement;
	m_view->position += V3Movement;
	m_view->target += V3Movement;
	Look(dt, 0.0001f, 0.f);
	updateTransformPos();
}

void CThirdPerson::MoveRight(double dt)
{
	changeTargetOffset(dt, true);
	m_TPview->MoveLeft_Right(dt, false);
	Vector3 tempTarget = m_TPview->target;
	tempTarget.y = m_TPview->position.y;
	Vector3 view = (tempTarget - m_TPview->position).Normalized();
	Vector3 right = view.Cross(m_TPview->up).Normalized();
	float movement = m_speed * dt;
	Vector3 V3Movement = right * movement;
	m_view->position += V3Movement;
	m_view->target += V3Movement;
	Look(dt, 0.0001f, 0.f);
	updateTransformPos();
}

void CThirdPerson::MoveUp(double dt)
{
}

void CThirdPerson::MoveDown(double dt)
{
}

void CThirdPerson::Sprint(bool mode)
{
	if (mode)
	{
		if (m_speed != S_SPRINT_SPEED)
		{
			m_speed = S_SPRINT_SPEED;
			m_targetSpeed *= 2.f;
		}
	}
	else
	{
		if (m_speed != S_NORMAL_SPEED)
		{
			m_speed = S_NORMAL_SPEED;
			m_targetSpeed *= 0.5f;
		}
	}
}

Vector3 CThirdPerson::calcTPviewPos()
{
	// All dir are based off first person view
	Vector3 backDir = (m_view->position - m_view->target).Normalized(); // This is essentially negative of view (-view)
	Vector3 leftDir = m_view->up.Cross(-backDir);
	Vector3 upDir = m_view->up;
	Vector3 TPview_pos = m_view->position;
	TPview_pos += leftDir * S_OFFSET_LEFT;
	TPview_pos += backDir * S_OFFSET_BACK;
	TPview_pos += upDir * S_OFFSET_UP;

	return TPview_pos;
}

void CThirdPerson::generateTPview(Camera3* view)
{
	/*Vector3 pos = calcTPviewPos();
	float angle = Math::RadianToDegree(acosf(S_OFFSET_UP / (pos - view->target).Length()));
	m_TPview->total_pitch = -angle;
	Vector3 right = ((m_view->target - m_view->position).Normalized()).Cross(view->up);
	right.y = 0;
	right.Normalize();
	Mtx44 rotation;
	rotation.SetToRotation(-angle, right.x, right.y, right.z);
	Vector3 up = right.Cross((view->target - pos).Normalized()).Normalized();
	up = rotation * up;
	right.Cross((view->target - pos).Normalized());
	m_TPview->Init(pos, view->target, m_view->up);*/

	/*Vector3 pos = calcTPviewPos();
	float angle = Math::RadianToDegree(acosf(S_OFFSET_UP / (pos - view->target).Length()));
	m_TPview->total_pitch = -angle;
	Vector3 right = (m_view->target - m_view->position).Normalized().Cross(view->up);
	m_TPview->Init(pos, view->target, right.Cross((view->target - pos).Normalized()));*/

	Vector3 backDir = (m_view->position - m_view->target).Normalized();
	Vector3 pos = m_view->position + backDir * S_OFFSET_BACK;
	Mtx44 rotation;
	Vector3 right = (-backDir).Cross(m_view->up);
	right.y = 0.f;
	right.Normalize();
	rotation.SetToRotation(-S_OFFSET_UP, right.x, right.y, right.z);
	Vector3 fpTOtp_dir = (pos - m_view->position).Normalized();
	fpTOtp_dir = rotation * fpTOtp_dir;
	pos = m_view->position + (fpTOtp_dir * S_OFFSET_BACK);
	Vector3 up = right.Cross((m_view->target - pos).Normalized());
	rotation.SetToRotation(-S_OFFSET_LEFT, 0, 1, 0);
	fpTOtp_dir = rotation * fpTOtp_dir;
	up = rotation * up;
	pos = m_view->position + (fpTOtp_dir * S_OFFSET_BACK);
	m_TPview->Init(pos, m_view->target, up);
}

void CThirdPerson::changeTargetOffset(double dt, bool mode)
{
	if (mode)
	{
		// Moving
		if (S_OFFSET_TARGET < S_MAX_OFFSET_TARGET)
		{
			S_OFFSET_TARGET += m_targetSpeed * dt;
			if (S_OFFSET_TARGET > S_MAX_OFFSET_TARGET)
			{
				S_OFFSET_TARGET = S_MAX_OFFSET_TARGET;
			}
		}
	}
	else
	{
		// Not moving
		if (S_OFFSET_TARGET > S_MIN_OFFSET_TARGET)
		{
			S_OFFSET_TARGET -= m_targetSpeed * dt;
			if (S_OFFSET_TARGET < S_MIN_OFFSET_TARGET)
			{
				S_OFFSET_TARGET = S_MIN_OFFSET_TARGET;
			}
			Look(dt, 0.0001f, 0.f);
		}
	}
}

void CThirdPerson::updateTarget()
{
	Vector3 tpDir = (m_TPview->target - m_TPview->position).Normalized();
	m_TPview->target = m_TPview->position + (tpDir * S_OFFSET_TARGET);
	m_view->target = m_TPview->target;
}

Camera3 * CThirdPerson::GetTPView()
{
	return m_TPview;
}
