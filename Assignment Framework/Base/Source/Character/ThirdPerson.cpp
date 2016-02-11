#include "ThirdPerson.h"
#include "MyMath.h"

string CThirdPerson::S_PROPERTIES[CThirdPerson::NUM_PLAYER_SAVE_PROPERTIES] =
{
	"PLAYER_POS_X",
	"PLAYER_POS_Y",
	"PLAYER_POS_Z",
	"PLAYER_ROTATE_X",
	"PLAYER_ROTATE_Y",
	"PLAYER_ROTATE_Z",
	"PLAYER_SCALE_X",
	"PLAYER_SCALE_Y",
	"PLAYER_SCALE_Z",
	"PLAYER_TARGET_X",
	"PLAYER_TARGET_Y",
	"PLAYER_TARGET_Z",
	"PLAYER_UP_X",
	"PLAYER_UP_Y",
	"PLAYER_UP_Z",
	"PLAYER_TP_POS_X",
	"PLAYER_TP_POS_Y",
	"PLAYER_TP_POS_Z",
	"PLAYER_TP_UP_X",
	"PLAYER_TP_UP_Y",
	"PLAYER_TP_UP_Z",
	"PLAYER_TOTAL_YAW",
	"PLAYER_TOTAL_PITCH",
};

const float CThirdPerson::S_MIN_OFFSET_TARGET = 50.f;
const float CThirdPerson::S_MAX_OFFSET_TARGET = S_MIN_OFFSET_TARGET * 3.f;
float CThirdPerson::S_OFFSET_TARGET = S_MIN_OFFSET_TARGET;

const float CThirdPerson::S_OFFSET_BACK = 50.f;
const float CThirdPerson::S_OFFSET_LEFT = 0.f;
const float CThirdPerson::S_OFFSET_UP = 45.f;

const float CThirdPerson::S_NORMAL_SPEED = 100.f;
const float CThirdPerson::S_SPRINT_SPEED = S_NORMAL_SPEED * 2.f;

CThirdPerson::CThirdPerson()
	: m_speed(S_NORMAL_SPEED)
	, m_targetSpeed(m_speed * 2.f)
	, m_health(5)
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

void CThirdPerson::Injure(int damage)
{
	m_health -= damage;
	if (m_health < 0)
	{
		m_health = 0;
	}
}

bool CThirdPerson::IsAlive()
{
	return m_health > 0;
}

bool CThirdPerson::SaveState(fstream * file, int id)
{
	// Transform
	write(file, S_PROPERTIES[PLAYER_POS_X], id, to_string((long double)m_transform.m_translate.x)); // Pos X
	write(file, S_PROPERTIES[PLAYER_POS_Y], id, to_string((long double)m_transform.m_translate.y)); // Pos Y
	write(file, S_PROPERTIES[PLAYER_POS_Z], id, to_string((long double)m_transform.m_translate.z)); // Pos Z

	write(file, S_PROPERTIES[PLAYER_ROTATE_X], id, to_string((long double)m_transform.m_rotate.x)); // Rotate X
	write(file, S_PROPERTIES[PLAYER_ROTATE_Y], id, to_string((long double)m_transform.m_rotate.y)); // Rotate Y
	write(file, S_PROPERTIES[PLAYER_ROTATE_Z], id, to_string((long double)m_transform.m_rotate.z)); // Rotate Z

	write(file, S_PROPERTIES[PLAYER_SCALE_X], id, to_string((long double)m_transform.m_scale.x)); // Scale X
	write(file, S_PROPERTIES[PLAYER_SCALE_Y], id, to_string((long double)m_transform.m_scale.y)); // Scale Y
	write(file, S_PROPERTIES[PLAYER_SCALE_Z], id, to_string((long double)m_transform.m_scale.z)); // Scale Z

	write(file, S_PROPERTIES[PLAYER_TARGET_X], id, to_string((long double)m_view->target.x)); // Target X
	write(file, S_PROPERTIES[PLAYER_TARGET_Y], id, to_string((long double)m_view->target.y)); // Target Y
	write(file, S_PROPERTIES[PLAYER_TARGET_Z], id, to_string((long double)m_view->target.z)); // Target Z

	write(file, S_PROPERTIES[PLAYER_UP_X], id, to_string((long double)m_view->up.x)); // Up X
	write(file, S_PROPERTIES[PLAYER_UP_Y], id, to_string((long double)m_view->up.y)); // Up Y
	write(file, S_PROPERTIES[PLAYER_UP_Z], id, to_string((long double)m_view->up.z)); // Up Z

	write(file, S_PROPERTIES[PLAYER_TP_POS_X], id, to_string((long double)m_TPview->position.x)); // TP pos X
	write(file, S_PROPERTIES[PLAYER_TP_POS_Y], id, to_string((long double)m_TPview->position.y)); // TP pos Y
	write(file, S_PROPERTIES[PLAYER_TP_POS_Z], id, to_string((long double)m_TPview->position.z)); // TP pos Z

	write(file, S_PROPERTIES[PLAYER_TP_UP_X], id, to_string((long double)m_TPview->up.x)); // TP up X
	write(file, S_PROPERTIES[PLAYER_TP_UP_Y], id, to_string((long double)m_TPview->up.y)); // TP up Y
	write(file, S_PROPERTIES[PLAYER_TP_UP_Z], id, to_string((long double)m_TPview->up.z)); // TP up Z

	/*write(file, S_PROPERTIES[PLAYER_TOTAL_YAW], id, to_string((long double)m_view->total_yaw)); // Total yaw
	write(file, S_PROPERTIES[PLAYER_TOTAL_PITCH], id, to_string((long double)m_view->total_pitch)); // Total pitch*/

	(*file) << "\n";

	return true;
}

bool CThirdPerson::LoadState(CLua_Wrapper * lua, int id)
{
	// Transform
	m_transform.m_translate.x = m_view->position.x = read(lua, S_PROPERTIES[PLAYER_POS_X], id); // Pos X
	m_transform.m_translate.y = m_view->position.y = read(lua, S_PROPERTIES[PLAYER_POS_Y], id); // Pos Y
	m_transform.m_translate.z = m_view->position.z = read(lua, S_PROPERTIES[PLAYER_POS_Z], id); // Pos Z

	m_transform.m_rotate.x = read(lua, S_PROPERTIES[PLAYER_ROTATE_X], id); // Rotate X
	m_transform.m_rotate.y = read(lua, S_PROPERTIES[PLAYER_ROTATE_Y], id); // Rotate Y
	m_transform.m_rotate.z = read(lua, S_PROPERTIES[PLAYER_ROTATE_Z], id); // Rotate Z

	m_transform.m_scale.x = read(lua, S_PROPERTIES[PLAYER_SCALE_X], id); // Scale X
	m_transform.m_scale.y = read(lua, S_PROPERTIES[PLAYER_SCALE_Y], id); // Scale Y
	m_transform.m_scale.z = read(lua, S_PROPERTIES[PLAYER_SCALE_Z], id); // Scale Z

	m_view->target.x = m_TPview->target.x = read(lua, S_PROPERTIES[PLAYER_TARGET_X], id); // Target X
	m_view->target.y = m_TPview->target.y = read(lua, S_PROPERTIES[PLAYER_TARGET_Y], id); // Target Y
	m_view->target.z = m_TPview->target.z = read(lua, S_PROPERTIES[PLAYER_TARGET_Z], id); // Target Z

	m_view->up.x = read(lua, S_PROPERTIES[PLAYER_UP_X], id); // Up X
	m_view->up.y = read(lua, S_PROPERTIES[PLAYER_UP_Y], id); // Up Y
	m_view->up.z = read(lua, S_PROPERTIES[PLAYER_UP_Z], id); // Up Z

	m_TPview->position.x = read(lua, S_PROPERTIES[PLAYER_TP_POS_X], id); // TP pos X
	m_TPview->position.y = read(lua, S_PROPERTIES[PLAYER_TP_POS_Y], id); // TP pos Y
	m_TPview->position.z = read(lua, S_PROPERTIES[PLAYER_TP_POS_Z], id); // TP pos Z

	m_TPview->up.x = read(lua, S_PROPERTIES[PLAYER_TP_UP_X], id); // TP up X
	m_TPview->up.y = read(lua, S_PROPERTIES[PLAYER_TP_UP_Y], id); // TP up Y
	m_TPview->up.z = read(lua, S_PROPERTIES[PLAYER_TP_UP_Z], id); // TP up Z

	return true;
}

void CThirdPerson::SetHealth(int health)
{
	m_health = health;
}

int CThirdPerson::GetHealth()
{
	return m_health;
}
