#include "pch.h"
#include "Transform.h"

void Transform::SetTransformToMatrix(D2D1_MATRIX_3X2_F matrix)
{
}

/// <summary>
/// 로컬 매트릭스 반환
/// </summary>
D2D1_MATRIX_3X2_F Transform::ToLocalMatrix()
{
	return
		D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
		D2D1::Matrix3x2F::Rotation(rotation) *
		D2D1::Matrix3x2F::Translation(position.x, position.y);
}

/// <summary>
/// 현재 로컬 좌표의 역행렬
/// </summary>
D2D1_MATRIX_3X2_F Transform::ToLocalInvertMatrix()
{
	D2D1_MATRIX_3X2_F resultMatrix = ToLocalMatrix();
	BOOL isSuccess = D2D1InvertMatrix(&resultMatrix);

	if (isSuccess)
	{
		return resultMatrix;
	}
	else
	{
		return D2D1::IdentityMatrix();
	}
}

/// <summary>
/// 월드 매트릭스 반환
/// </summary>
D2D1_MATRIX_3X2_F Transform::ToWorldMatrix()
{
	if (parent == nullptr)
		return ToLocalMatrix();
	else
		return ToLocalMatrix() * parent->ToWorldMatrix();
}

/// <summary>
/// 현재 월드 좌표의 역행렬
/// </summary>
D2D1_MATRIX_3X2_F Transform::ToWorldInvertMatrix()
{
	D2D1_MATRIX_3X2_F resultMatrix = ToWorldMatrix();
	BOOL isSuccess = D2D1InvertMatrix(&resultMatrix);

	if (isSuccess)
	{
		return resultMatrix;
	}
	else
	{
		return D2D1::IdentityMatrix();
	}
}

/// <summary>
/// 모든 값을 초기화 ( scale은 (1,1) , 나머지 다 0으로 초기화 )
/// </summary>
void Transform::Reset()
{
	SetPosition(0.0f, 0.0f);
	SetRotation(0.0f);
	SetScale(1.0f, 1.0f);
}