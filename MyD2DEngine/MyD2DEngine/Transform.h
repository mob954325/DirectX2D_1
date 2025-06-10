#pragma once
class Transform
{
protected:
	Transform* camera = nullptr;
	Transform* parent = nullptr;

	D2D1_VECTOR_2F position;
	float rotation;
	D2D1_VECTOR_2F scale;

	bool isUnityCoords = true;

public:
	// 생성자
	Transform()
		: position({ 0.0f, 0.0f }), rotation(0.0f), scale({1.0f, 1.0f}) { }

	// get, set
	D2D1_VECTOR_2F GetPosition() { return position; }
	void SetPosition(float x, float y) { position.x = x; position.y = y; }

	float GetRotation() { return rotation; }
	void SetRotation(float rotValue) { rotation = rotValue; }

	D2D1_VECTOR_2F GetScale() { return scale; }
	void SetScale(float scaleX, float scaleY) { scale.x = scaleX; scale.y = scaleY; }

	Transform* GetCamera() { return camera; }
	void SetCamera(Transform* t) { camera = t; }

	Transform* GetParent() { return parent; }
	void SetParent(Transform* t) { parent = t; }

	bool IsUnityCoords() { return isUnityCoords; }
	void SetIsUnityCoords(bool value) { isUnityCoords = value; }

	/// <summary>
	/// 매트릭스로 transform 값 설정
	/// </summary>
	void SetTransformToMatrix(D2D1_MATRIX_3X2_F matrix);

	/// <summary>
	/// 로컬 매트릭스 반환
	/// </summary>
	D2D1_MATRIX_3X2_F ToLocalMatrix();

	/// <summary>
	/// 현재 로컬 좌표의 역행렬
	/// </summary>
	D2D1_MATRIX_3X2_F ToLocalInvertMatrix();

	/// <summary>
	/// 월드 매트릭스 반환
	/// </summary>
	D2D1_MATRIX_3X2_F ToWorldMatrix();

	/// <summary>
	/// 현재 월드 좌표의 역행렬
	/// </summary>
	D2D1_MATRIX_3X2_F ToWorldInvertMatrix();

	/// <summary>
	/// 모든 값을 초기화 ( scale은 (1,1) , 나머지 다 0으로 초기화 )
	/// </summary>
	void Reset();
};

