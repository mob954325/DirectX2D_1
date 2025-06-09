#include "pch.h"
#include "Application.h"
#include "D2DRenderManager.h"

#include "Singleton.h"
#include "Input.h"
#include "GameTime.h"

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	Application* pThis = nullptr;

	// WM_NCCREATE: 윈도우 생성 아주 초기에, 프레임 생성 전에. WM_CREATE보다 이전에발생
	if (uMsg == WM_NCCREATE)
	{
		// lParam은 CREATESTRUCT* 이다
		CREATESTRUCT* cs = reinterpret_cast<CREATESTRUCT*>(lParam);
		pThis = reinterpret_cast<Application*>(cs->lpCreateParams);

		// HWND에 this 포인터 저장
		SetWindowLongPtr(hWnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pThis));
	}
	else {
		// WM_NCCREATE가 아닐 때는 HWND에서 this 포인터를 가져온다
		pThis = reinterpret_cast<Application*>(GetWindowLongPtr(hWnd, GWLP_USERDATA));
	}

	if (pThis)
		pThis->MessageProc(hWnd, uMsg, wParam, lParam); // 멤버 함수 호출

	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

void Application::Initialize()
{
	// Window 생성
	WNDCLASS wc = {};
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = m_hInstance;
	wc.lpszClassName = L"MyD2DWindowClass";
	RegisterClass(&wc);

	SIZE clientSize = { (LONG)m_Width,(LONG)m_Height };
	RECT clientRect = { 0, 0, clientSize.cx, clientSize.cy };
	AdjustWindowRect(&clientRect, WS_OVERLAPPEDWINDOW, FALSE);

	m_hwnd = CreateWindowEx(0, L"MyD2DWindowClass", L"D2D1 Clear Example",
		WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT,
		clientRect.right - clientRect.left, clientRect.bottom - clientRect.top,
		nullptr, nullptr, m_hInstance, this);
	ShowWindow(m_hwnd, SW_SHOW);

	// D3D11 디바이스 생성
	D3D_FEATURE_LEVEL featureLevel;
	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	D3D11CreateDevice(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr,
		D3D11_CREATE_DEVICE_BGRA_SUPPORT, levels, 1,
		D3D11_SDK_VERSION, m_d3dDevice.GetAddressOf(), &featureLevel, nullptr);

	// D2D 팩토리 및 디바이스 생성
	ComPtr<ID2D1Factory8> d2dFactory;
	D2D1_FACTORY_OPTIONS options = {
#ifdef _DEBUG
		D2D1_DEBUG_LEVEL_INFORMATION
#endif // _DEBUG

	};
	D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, options, d2dFactory.GetAddressOf());

	ComPtr<IDXGIDevice> dxgiDevice;
	m_d3dDevice.As(&dxgiDevice);
	ComPtr<ID2D1Device7> d2dDevice;
	d2dFactory->CreateDevice((dxgiDevice.Get()), d2dDevice.GetAddressOf());
	d2dDevice->CreateDeviceContext(D2D1_DEVICE_CONTEXT_OPTIONS_NONE, m_d2dDeviceContext.GetAddressOf());

	ComPtr<IDXGIFactory7> dxgiFactory;
	CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));

	// SwapChain 생성
	DXGI_SWAP_CHAIN_DESC1 scDesc = {};
	scDesc.Width = m_Width;
	scDesc.Height = m_Height;
	scDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM;
	scDesc.SampleDesc.Count = 1;
	scDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scDesc.BufferCount = 2;
	scDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	dxgiFactory->CreateSwapChainForHwnd(m_d3dDevice.Get(), m_hwnd, &scDesc, nullptr, nullptr, m_dxgiSwapChain.GetAddressOf());

	// 스왑체인의 백버퍼를 사용하는 D2D1Bitmap1 인터페이스 생성 
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(scDesc.Format, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_d2dBitmapTarget.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	// D2DManager 초기화
	m_D2DRenderManager = new D2DRenderManager;
	m_D2DRenderManager->Initialize();
	m_D2DRenderManager->GetD2D1DeviceContext7(m_d2dDeviceContext.Get());

	// GameTime 초기화
	Singleton<GameTime>::GetInstance().InitTime();
}

void Application::Uninitialize()
{
	m_D2DRenderManager->Uninitialize();
	delete m_D2DRenderManager;

	m_d3dDevice = nullptr;
	m_dxgiSwapChain = nullptr;
	m_d2dDeviceContext = nullptr;
	m_d2dBitmapTarget = nullptr;
}

void Application::MessageProc(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		m_isWindowOpen = false;
		break;
	case WM_KEYDOWN:
		if (wParam == VK_SPACE)
			//g_useScreenEffect = !g_useScreenEffect;
		break;
	case WM_SIZE:
	{
		if (wParam == SIZE_MINIMIZED)
			break; // 최소화는 무시

		UINT width = LOWORD(lParam); // 새 너비
		UINT height = HIWORD(lParam); // 새 높이			
		if (m_Width != width || m_Height != height)
		{
			m_Width = width;
			m_Height = height;
			m_resized = true;
		}
	}
	break;
	case WM_EXITSIZEMOVE:
		if (m_resized)
		{
			ResizeSwapChainBuffers();
			m_resized = false;
		}
		break;
	default:
		break;
	}
}

void Application::Render()
{
	m_D2DRenderManager->Render();
	m_dxgiSwapChain->Present(1, 0);
}

void Application::Update()
{
	// 업데이트 내용
}

void Application::Run()
{
	MSG msg = {};
	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else 
		{
			Singleton<Input>::GetInstance().Update();
			Singleton<GameTime>::GetInstance().UpdateTime();
			Update();
			Render();
		}
	}
}

void Application::ResizeSwapChainBuffers()
{
	if (!m_dxgiSwapChain || !m_d2dDeviceContext) return;

	m_d2dDeviceContext->SetTarget(nullptr);
	m_d2dBitmapTarget.Reset();

	// 1. 스왑체인 버퍼 리사이즈
	HRESULT hr = m_dxgiSwapChain->ResizeBuffers(0, m_Width, m_Height, DXGI_FORMAT_UNKNOWN, 0);
	if (FAILED(hr))
	{
		// 오류 로그 출력
		return;
	}

	// 2. 백버퍼 다시 얻고 D2D Bitmap 다시 생성
	ComPtr<IDXGISurface> backBuffer;
	m_dxgiSwapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));

	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_TARGET | D2D1_BITMAP_OPTIONS_CANNOT_DRAW,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);
	m_d2dDeviceContext->CreateBitmapFromDxgiSurface(backBuffer.Get(), &bmpProps, m_d2dBitmapTarget.GetAddressOf());
	m_d2dDeviceContext->SetTarget(m_d2dBitmapTarget.Get());

	// 렌더 매니저에도 다시 설정
	m_D2DRenderManager->GetD2D1DeviceContext7(m_d2dDeviceContext.Get());
}