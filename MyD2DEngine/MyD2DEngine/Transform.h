#pragma once
class Transform
{
public:
	Transform* parent;
	D2D1_VECTOR_2F position;
	float rotation;
	D2D1_VECTOR_2F scale;


	// ������
	Transform()
		: parent(nullptr), position({ 0.0f, 0.0f }), rotation(0.0f), scale({1.0f, 1.0f}) { }

	// get / set
	D2D1_VECTOR_2F GetPosition() { return position; }
	void SetPosition(float x, float y) { position.x = x; position.y = y; }

	float GetRotation() { return rotation; }
	void SetRotation(float rotValue) { rotation = rotValue; }

	D2D1_VECTOR_2F GetScale() { return scale; }
	void SetScale(float scaleX, float scaleY) { scale.x = scaleX; scale.y = scaleY; }

	Transform* GetParent() { return parent; }
	void SetParent(Transform* t) { parent = t; }


	/// <summary>
	/// ���� ��Ʈ���� ��ȯ
	/// </summary>
	D2D1_MATRIX_3X2_F ToLocalMatrix()
	{
		return
			D2D1::Matrix3x2F::Scale(scale.x, scale.y) *
			D2D1::Matrix3x2F::Rotation(rotation) *
			D2D1::Matrix3x2F::Translation(position.x, position.y);
	}

	/// <summary>
	/// ���� ��Ʈ���� ��ȯ
	/// </summary>
	D2D1_MATRIX_3X2_F ToWorldMatrix()
	{
		if (parent == nullptr)
			return ToLocalMatrix();
		else
			return ToLocalMatrix() * parent->ToWorldMatrix();
	}

	/// <summary>
	/// ��� ���� �ʱ�ȭ ( scale�� (1,1) , ������ �� 0���� �ʱ�ȭ )
	/// </summary>
	void Reset()
	{
		SetPosition(0.0f, 0.0f);
		SetRotation(0.0f);
		SetScale(1.0f, 1.0f);
	}	
};

