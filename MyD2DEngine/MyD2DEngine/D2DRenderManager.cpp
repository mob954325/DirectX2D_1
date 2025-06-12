#include "pch.h"
#include "D2DRenderManager.h"

void D2DRenderManager::Initialize()
{
	HRESULT hr;

	// Create WIC factory
	hr = CoCreateInstance(CLSID_WICImagingFactory,
		NULL, CLSCTX_INPROC_SERVER,
		__uuidof(m_wicImagingFactory),
		(void**)m_wicImagingFactory.GetAddressOf());

	assert(SUCCEEDED(hr));

	// DirectWrite 팩터리를 만듭니다.
	hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(m_pDWriteFactory), reinterpret_cast<IUnknown**>(m_pDWriteFactory.GetAddressOf()));
	assert(SUCCEEDED(hr));

	// DirectWrite 텍스트 형식 개체를 만듭니다.
	hr = m_pDWriteFactory->CreateTextFormat(
		L"", // FontName    제어판-모든제어판-항목-글꼴-클릭 으로 글꼴이름 확인가능
		NULL,
		DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH_NORMAL,
		15.0f,   // Font Size
		L"", //locale
		&m_pDWriteTextFormat
	);
	assert(SUCCEEDED(hr));
}

void D2DRenderManager::Uninitialize()
{
	renderList.clear();
	m_wicImagingFactory = nullptr;
}

void D2DRenderManager::Render()
{
	assert(m_d2dDeviceContext);

	m_d2dDeviceContext->BeginDraw();

	// Clear
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	// bitmap 랜더링
	for (auto* comp : renderList)
	{
		comp->Render();
	}

	PrintText(L"회전 여부 : R",0,0);
	PrintText(L"회전 방향 변경 : T",0,20);
	PrintText(L"유니티 좌표계 전환 : Y",0,40);
	PrintText(L"태양 위치 (0,0)으로 초기화 : U",0, 80);
	PrintText(L"카메라 이동 : WASD",0, 60);

	m_d2dDeviceContext->EndDraw();
}

void D2DRenderManager::SetD2D1DeviceContext7(ID2D1DeviceContext7* pD2D1DeviceContext7)
{
	if (pD2D1DeviceContext7)
	{
		m_d2dDeviceContext = pD2D1DeviceContext7;

		// Brush 생성
		HRESULT hr = m_d2dDeviceContext->CreateSolidColorBrush(D2D1::ColorF(D2D1::ColorF::GreenYellow), &m_pBrush);
		assert(SUCCEEDED(hr));
	}
}

void D2DRenderManager::SetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

void D2DRenderManager::SetBitmapTransform(D2D1_MATRIX_3X2_F& finalMatrix)
{
	m_d2dDeviceContext->SetTransform(finalMatrix);
}

void D2DRenderManager::DrawBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap)
{
	m_d2dDeviceContext->DrawBitmap(bitmap.Get());
}

void D2DRenderManager::DrawBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap, D2D1_RECT_F& destRect)
{
	m_d2dDeviceContext->DrawBitmap(bitmap.Get(), destRect);
}

void D2DRenderManager::AddRenderObject(IRenderer* comp)
{
	renderList.push_back(comp);
}

void D2DRenderManager::PrintText(const wchar_t* str, float left, float top)
{
	if (!m_d2dDeviceContext || !m_pBrush) return;

	m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());
	m_pBrush->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	m_d2dDeviceContext->DrawTextW(str, (UINT32)wcslen(str), m_pDWriteTextFormat.Get(), D2D1::RectF(left, top, left + 300, top + 250), m_pBrush.Get());
}

D2D1_MATRIX_3X2_F D2DRenderManager::GetCameraInvertMatrix()
{
	if (m_camTransform != nullptr)
	{
		return m_camTransform->ToWorldInvertMatrix();
	}
	else
	{
		return D2D1::Matrix3x2F::Identity();
	}
}

HRESULT D2DRenderManager::CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap)
{
	ComPtr<IWICBitmapDecoder>     decoder;
	ComPtr<IWICBitmapFrameDecode> frame;
	ComPtr<IWICFormatConverter>   converter;

	// ① 디코더 생성
	HRESULT hr = m_wicImagingFactory->CreateDecoderFromFilename(
		path, nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &decoder);
	if (FAILED(hr)) return hr;

	// ② 첫 프레임 얻기
	hr = decoder->GetFrame(0, &frame);
	if (FAILED(hr)) return hr;

	// ③ 포맷 변환기 생성
	hr = m_wicImagingFactory->CreateFormatConverter(&converter);
	if (FAILED(hr)) return hr;

	// ④ GUID_WICPixelFormat32bppPBGRA로 변환
	hr = converter->Initialize(
		frame.Get(),
		GUID_WICPixelFormat32bppPBGRA,
		WICBitmapDitherTypeNone,
		nullptr,
		0.0f,
		WICBitmapPaletteTypeCustom
	);
	if (FAILED(hr)) return hr;

	// ⑤ Direct2D 비트맵 속성 (premultiplied alpha, B8G8R8A8_UNORM)
	D2D1_BITMAP_PROPERTIES1 bmpProps = D2D1::BitmapProperties1(
		D2D1_BITMAP_OPTIONS_NONE,
		D2D1::PixelFormat(DXGI_FORMAT_B8G8R8A8_UNORM, D2D1_ALPHA_MODE_PREMULTIPLIED)
	);

	// ⑥ DeviceContext에서 WIC 비트맵으로부터 D2D1Bitmap1 생성
	hr = m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap);

	return hr;
}
