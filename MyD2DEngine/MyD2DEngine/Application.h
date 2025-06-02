#pragma once
#include "framework.h"

using namespace Microsoft::WRL;

class D2DRenderManager;

// 윈도우 프로그램 생성 재활용을 위한 클래스
class Application
{
public:
	D2DRenderManager* m_D2DRenderManager;

	/// <summary>
	/// 윈도우 생성 및 D2DRenderManager 초기화
	/// </summary>
	virtual void Initialize();
	virtual void Uninitialize();

	virtual LRESULT CALLBACK WndProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	virtual void Render();
	virtual void Run();

protected:
	HWND m_hwnd = nullptr;
	ComPtr<ID3D11Device> m_d3dDevice;
	ComPtr<IDXGISwapChain1> m_dxgiSwapChain;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;
	ComPtr<ID2D1Bitmap1> m_d2dBitmapTarget;

	UINT m_width = 1024;
	UINT m_height = 768;
};
