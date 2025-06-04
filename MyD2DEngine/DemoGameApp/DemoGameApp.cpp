#include "DemoGameApp.h"
#include "../MyD2DEngine/D2DRenderManager.h"

void DemoGameApp::Initialize()
{
	__super::Initialize(); // Application 초기화

	HRESULT hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Mushroom.png");
	assert(SUCCEEDED(hr));
}

void DemoGameApp::Render()
{
	__super::Render();
}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();

	g_d2dBitmapFromFile = nullptr;
}
