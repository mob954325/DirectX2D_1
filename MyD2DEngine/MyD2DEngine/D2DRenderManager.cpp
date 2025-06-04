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
}

void D2DRenderManager::Uninitialize()
{
	m_wicImagingFactory = nullptr;
}

void D2DRenderManager::Render()
{
	assert(m_d2dDeviceContext);

	m_d2dDeviceContext->BeginDraw();

	// Clear
	m_d2dDeviceContext->Clear(D2D1::ColorF(D2D1::ColorF::DarkSlateBlue));

	// bitmap 랜더링
	for (auto bitmap : bitmaps)
	{	
		m_d2dDeviceContext->DrawBitmap(bitmap.Get());
	}

	m_d2dDeviceContext->EndDraw();
}

void D2DRenderManager::GetD2D1DeviceContext7(ID2D1DeviceContext7* pD2D1DeviceContext7)
{
	if (pD2D1DeviceContext7)
	{
		m_d2dDeviceContext = pD2D1DeviceContext7;
	}
}

HRESULT D2DRenderManager::CreateBitmapFromFile(const wchar_t* path)
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
	ComPtr<ID2D1Bitmap1> outBitmap;
	hr = m_d2dDeviceContext->CreateBitmapFromWicBitmap(converter.Get(), &bmpProps, outBitmap.GetAddressOf());

	// bitmap 저장
	bitmaps.push_back(outBitmap);

	return hr;
}
