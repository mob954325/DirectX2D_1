#include "DemoGameApp.h"
#include "../MyD2DEngine/D2DRenderManager.h"

void DemoGameApp::Initialize()
{
	__super::Initialize(); // Application 초기화

	// 이미지 로드
	// Create WIC factory
	HRESULT hr;
	hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/mushroom.png", m_d2dDeviceContext.GetAddressOf(), g_d2dBitmapFromFile.GetAddressOf());
	assert(SUCCEEDED(hr));
}

void DemoGameApp::Render()
{
	// 랜더 내용
	m_d2dDeviceContext->BeginDraw();
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	// mushroom.png 그리기
	D2D1_SIZE_F size;

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity()); // 변환 초기화

	//1. 0,0 위치에 비트맵 전체영역 그린다. (변환은 초기화)
	m_d2dDeviceContext->DrawBitmap(g_d2dBitmapFromFile.Get());

	m_d2dDeviceContext->EndDraw();
	m_dxgiSwapChain->Present(1, 0);
}