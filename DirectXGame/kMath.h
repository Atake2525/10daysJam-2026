#pragma once

#include "AABB.h"
#include "Matrix4x4.h"
#include "Vector3.h"
#define _USE_MATH_DEFINES
#include <cassert>
#include <cmath>

// ������Z�q�I�[�o�[���[�h
// Vector3�̑����Z
Vector3& operator+=(Vector3& lhs, const Vector3& rhv);

Vector3& operator-=(Vector3& lhs, const Vector3& rhv);

Vector3 operator+(const Vector3& v1, const Vector3& v2);

bool IsCollision(const AABB& aabb1, const AABB& aabb2);

// ������Z�q�I�[�o�[���[�h
// Vector3�̊|���Z
Vector3& operator*=(Vector3& v, float s);

// 2�����Z�q�I�[�o�[���[�h
// Vector3�̊|���Z
const Vector3 operator*(const Vector3& v, float s);

// ease In-Out
float easeInOut(float t, float x1, float x2);

// Lerp
float Lerp(float x1, float x2, float t);

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t);

// 1, x����]�s��
Matrix4x4 MakeRotateXMatrix(float radian);

// 2, y����]�s��
Matrix4x4 MakeRotateYMatrix(float radian);

// 3, z����]�s��
Matrix4x4 MakeRotateZMatrix(float radian);

Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);

// �R�����A�t�B���ϊ��s��
Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

// 3, ���W�ϊ�
Vector3 MathTransform(const Vector3& vector, const Matrix4x4& matrix);

float Dot(const Vector3& v1, const Vector3& v2);

float Length(const Vector3& v);
