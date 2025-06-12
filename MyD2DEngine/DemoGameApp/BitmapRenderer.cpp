#include "BitmapRenderer.h"
#include "../MyD2DEngine/D2DRenderManager.h"

void BitmapRenderer::Render()
{
	if (m_RenderManager == nullptr) return;
	//if (m_bitmap != nullptr)
	//{
	//	// 최종 변환 값 계산
	//	finalMatrix =
	//		//m_Transform->IsUnityCoords() ? unityRenderMatrix : normalRenderMatrix *	// Render Matrix
	//		m_Transform->ToWorldMatrix() *											// m_transform world matrix 
	//		m_RenderManager->GetCameraInvertMatrix(); //*								// MainCamera invert matrix
	//		//unityCoordMatrix;														// unity coord Matrix

	//	m_RenderManager->SetBitmapTransform(finalMatrix);
	//}

	if (m_bitmap != nullptr)
	{
		// Camera Matrix 추가
		Transform* cameraTransform = m_Transform->GetCamera();
		D2D1_MATRIX_3X2_F cameraInverseMatrix =
			cameraTransform ? cameraTransform->ToWorldInvertMatrix() : D2D1::Matrix3x2F::Identity();

		// Render Matrix 추가
		D2D1_MATRIX_3X2_F renderMatrix = GetRenderMatrix(m_Transform);

		// Unity 좌표 Matrix 추가
		D2D1_MATRIX_3X2_F unityMatrix = m_Transform->IsUnityCoords() ?
			D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation((FLOAT)(screenWidth / 2), (FLOAT)(screenHeight / 2)) :
			D2D1::Matrix3x2F::Identity();

		// 최종 변환 값 계산
		finalMatrix = renderMatrix * m_Transform->ToWorldMatrix() * cameraInverseMatrix * unityMatrix;

		// m_d2dDeviceContext->SetTransform(comp->GetFinalMatrix());
		m_RenderManager->SetBitmapTransform(finalMatrix);
	}

	// bitmap 이미지 중앙으로 옮기기
	auto size = m_bitmap->GetSize();
	destRect = D2D1::RectF(
		-size.width / 2.0f,
		-size.height / 2.0f,
		size.width / 2.0f,
		size.height / 2.0f
	);

	m_RenderManager->DrawBitmap(m_bitmap);
}

void BitmapRenderer::SetRenderManager(D2DRenderManager*& manager)
{
	m_RenderManager = manager;
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
