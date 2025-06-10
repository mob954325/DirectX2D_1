#pragma once
#include "D2DRenderManager.h"
#include "IRenderer.h"
#include "Transform.h"

class BitmapRenderer : public IRenderer
{
public:
	void Render() override;

protected:
	D2D1::Matrix3x2F GetRenderMatrix(Transform* transform);

	Transform* m_Transform = new Transform();
	Microsoft::WRL::ComPtr<ID2D1Bitmap1> m_bitmap;
};

