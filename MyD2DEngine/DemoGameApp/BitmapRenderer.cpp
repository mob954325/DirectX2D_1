#include "BitmapRenderer.h"
#include "../MyD2DEngine/D2DRenderManager.h"

void BitmapRenderer::Render()
{
	if (m_RenderManager == nullptr) return;
	if (m_bitmap != nullptr)
	{
		// 최종 변환 값 계산
		if (m_Transform->IsUnityCoords())
		{
			finalMatrix =
				unityRenderMatrix *						// Render Matrix
				m_Transform->ToWorldMatrix() *				// m_transform world matrix 
				m_RenderManager->GetCameraInvertMatrix() *	// MainCamera invert matrix
				unityCoordMatrix;							// unity coord Matrix
		}
		else
		{
			finalMatrix =
				normalRenderMatrix *						// Render Matrix
				m_Transform->ToWorldMatrix() *				// m_transform world matrix 
				m_RenderManager->GetCameraInvertMatrix();   // MainCamera invert matrix	
		}


		m_RenderManager->SetBitmapTransform(finalMatrix);
	}

	m_RenderManager->DrawBitmap(m_bitmap);
}

void BitmapRenderer::SetRenderManager(D2DRenderManager*& manager)
{
	m_RenderManager = manager;
}

void BitmapRenderer::SetScreenSize(int width, int height)
{
	screenWidth = width; screenHeight = height;
	unityCoordMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation((FLOAT)(screenWidth / 2), (FLOAT)(screenHeight / 2));
}

void BitmapRenderer::SetOffSet(float x, float y)
{
	offsetX = x; offsetY = y;
	normalRenderMatrix = D2D1::Matrix3x2F::Scale(1.0f, 1.0f) * D2D1::Matrix3x2F::Translation(offsetX, offsetY);
	unityRenderMatrix = D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation(offsetX, -offsetY);
}

D2D1::Matrix3x2F BitmapRenderer::GetRenderMatrix(Transform* transform)
{
	if (transform == nullptr)
		return D2D1::Matrix3x2F::Identity();

	float scaleX = 1.0f;
	float scaleY = transform->IsUnityCoords() ? -1.0f : 1.0f; // 유니티 좌표계면 y축 상하 반전

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	return D2D1::Matrix3x2F::Scale(scaleX, scaleY) * D2D1::Matrix3x2F::Translation(offsetX, offsetY);
}
