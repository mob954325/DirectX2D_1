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
	void SetScreenSize(int width, int height) { screenWidth = width; screenHeight = height; };
	Transform* GetTransform() { return m_Transform; }

protected:
	D2DRenderManager* m_RenderManager = nullptr;
	D2D1::Matrix3x2F GetRenderMatrix(Transform* transform);

	Transform* m_Transform = new Transform();
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;
	D2D1_RECT_F destRect = {};
	D2D1_MATRIX_3X2_F finalMatrix = {};

	int screenWidth = 0;
	int screenHeight = 0;
};

