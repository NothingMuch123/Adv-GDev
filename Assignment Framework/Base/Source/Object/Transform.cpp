#include "Transform.h"



CTransform::CTransform(Vector3 translate, Vector3 rotate, Vector3 scale) : m_translate(translate), m_rotate(rotate), m_scale(scale)
{
	m_transformation.SetToIdentity();
}

CTransform::~CTransform()
{
}

void CTransform::Init(Vector3 translate, Vector3 rotate, Vector3 scale)
{
	this->m_translate = translate;
	this->m_rotate = rotate;
	this->m_scale = scale;
}

void CTransform::Reset()
{
	m_translate = m_rotate = Vector3::ZERO_VECTOR;
	m_scale = Vector3(1.f, 1.f, 1.f);
	m_transformation.SetToIdentity();
}

void CTransform::SetTranslate(Vector3 translate)
{
	this->m_translate = translate;
}

void CTransform::SetTranslateX(float x)
{
	this->m_translate.x = x;
}

void CTransform::SetTranslateY(float y)
{
	this->m_translate.y = y;
}

void CTransform::SetTranslateZ(float z)
{
	this->m_translate.z = z;
}

Vector3 & CTransform::GetTranslate()
{
	return m_translate;
}

void CTransform::SetRotate(Vector3 rotate)
{
	this->m_rotate = rotate;
}

void CTransform::SetRotateX(float x)
{
	this->m_rotate.x = x;
}

void CTransform::SetRotateY(float y)
{
	this->m_rotate.y = y;
}

void CTransform::SetRotateZ(float z)
{
	this->m_rotate.z = z;
}

Vector3 & CTransform::GetRotate()
{
	return m_rotate;
}

void CTransform::SetScale(Vector3 scale)
{
	this->m_scale = scale;
}

void CTransform::SetScaleX(float x)
{
	this->m_scale.x = x;
}

void CTransform::SetScaleY(float y)
{
	this->m_scale.y = y;
}

void CTransform::SetScaleZ(float z)
{
	this->m_scale.z = z;
}

Vector3 & CTransform::GetScale()
{
	return m_scale;
}

Mtx44 & CTransform::GetMtx()
{
	m_transformation.SetToIdentity();

	Mtx44 tempMtx;

	// Translate
	tempMtx.SetToIdentity();
	tempMtx.SetToTranslation(m_translate.x, m_translate.y, m_translate.z);
	m_transformation = m_transformation * tempMtx;

	// Scale
	static const float S_OFFSET = 0.5f;
	tempMtx.SetToScale(m_scale.x * S_OFFSET, m_scale.y * S_OFFSET, m_scale.z * S_OFFSET);
	m_transformation = m_transformation * tempMtx;

	return m_transformation;
}
