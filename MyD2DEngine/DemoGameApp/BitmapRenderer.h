#pragma once
#include "../MyD2DEngine/Application.h"
#include "../MyD2DEngine/IRenderer.h"
#include "../MyD2DEngine/Transform.h"

class BitmapRenderer : public IRenderer
{
public:
	BitmapRenderer() {};
	~BitmapRenderer() 
	{ 
		delete m_Transform; 
		m_Transform = nullptr;
	};

	void Render() override;

	void SetRenderManager(D2DRenderManager*& manager);
	void SetBitmap(const ComPtr<ID2D1Bitmap1>& bitmap) { m_bitmap = bitmap; }
	void SetScreenSize(int width, int height);
	void SetOffSet(float x, float y);
	Transform* GetTransform() { return m_Transform; }

protected:
	D2DRenderManager* m_RenderManager = nullptr;
	D2D1::Matrix3x2F GetRenderMatrix(Transform* transform);

	D2D1_MATRIX_3X2_F unityCoordMatrix = D2D1::Matrix3x2F::Identity(); // 유니티 좌표 전환 메트릭스
	D2D1_MATRIX_3X2_F normalRenderMatrix = D2D1::Matrix3x2F::Scale(1.0f, 1.0f) * D2D1::Matrix3x2F::Translation(offsetX, offsetY);
	D2D1_MATRIX_3X2_F unityRenderMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation(offsetX, offsetY);

	Transform* m_Transform = new Transform();
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;
	D2D1_RECT_F destRect = {};
	D2D1_MATRIX_3X2_F finalMatrix = {};

	int screenWidth = 0;
	int screenHeight = 0;
	float offsetX = 0.0f;
	float offsetY = 0.0f;
};

