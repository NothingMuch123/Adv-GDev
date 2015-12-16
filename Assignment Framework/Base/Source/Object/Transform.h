#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "Vector3.h"
#include "Mtx44.h"

class CTransform
{
public:
	Vector3 m_translate, m_rotate, m_scale;
	Mtx44 m_transformation;

public:
	CTransform(Vector3 translate = Vector3::ZERO_VECTOR, Vector3 rotate = Vector3::ZERO_VECTOR, Vector3 scale = Vector3(1.f, 1.f, 1.f));
	~CTransform();

	void Init(Vector3 translate, Vector3 rotate, Vector3 scale);
	void Reset(); // Reset obj

	// Setters and Getters
	void SetTranslate(Vector3 translate);
	void SetTranslateX(float x);
	void SetTranslateY(float y);
	void SetTranslateZ(float z);
	Vector3& GetTranslate();

	void SetRotate(Vector3 rotate);
	void SetRotateX(float x);
	void SetRotateY(float y);
	void SetRotateZ(float z);
	Vector3& GetRotate();

	void SetScale(Vector3 scale);
	void SetScaleX(float x);
	void SetScaleY(float y);
	void SetScaleZ(float z);
	Vector3& GetScale();

	Mtx44& GetMtx();
};

#endif