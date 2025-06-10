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
	RenderBitmaps();

	m_d2dDeviceContext->EndDraw();
}

void D2DRenderManager::GetD2D1DeviceContext7(ID2D1DeviceContext7* pD2D1DeviceContext7)
{
	if (pD2D1DeviceContext7)
	{
		m_d2dDeviceContext = pD2D1DeviceContext7;
	}
}

void D2DRenderManager::GetScreenSize(int width, int height)
{
	screenWidth = width;
	screenHeight = height;
}

HRESULT D2DRenderManager::CreateBitmapFromFile(const wchar_t* path)
{
	return CreateBitmapFromFile(path, {});
}

HRESULT D2DRenderManager::CreateBitmapFromFile(const wchar_t* path, Transform* transform)
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

	bitmaps.push_back(std::make_pair(outBitmap, transform));

	return hr;
}

void D2DRenderManager::RenderBitmaps()
{
	int count = (int)bitmaps.size();
	for (int i = 0; i < count; i++)
	{
		if (bitmaps[i].second != nullptr)
		{
			// Camera Matrix 추가
			Transform* cameraTransform = bitmaps[i].second->GetCamera();
			D2D1_MATRIX_3X2_F cameraInverseMatrix =
				cameraTransform ? cameraTransform->ToWorldInvertMatrix() : D2D1::Matrix3x2F::Identity();

			// Render Matrix 추가
			D2D1_MATRIX_3X2_F renderMatrix = GetRenderMatrix(bitmaps[i].second);

			// Unity 좌표 Matrix 추가
			D2D1_MATRIX_3X2_F unityMatrix = bitmaps[i].second->IsUnityCoords() ?
				D2D1::Matrix3x2F::Scale(1.0f, -1.0f) * D2D1::Matrix3x2F::Translation((FLOAT)(screenWidth / 2), (FLOAT)(screenHeight / 2)) : 
				D2D1::Matrix3x2F::Identity();

			// 최종 변환 값 계산
			D2D1_MATRIX_3X2_F finalMatrix = renderMatrix * bitmaps[i].second->ToWorldMatrix() * cameraInverseMatrix * unityMatrix;

			m_d2dDeviceContext->SetTransform(finalMatrix);
		}
		else // Transform이 없으면 0,0 위치 설정
		{
			m_d2dDeviceContext->SetTransform(D2D1::Matrix3x2F::Identity());

			//wchar_t buffer[128];
			//swprintf_s(buffer, 128, L"&d번 인덱스 Transform 없음\n", i);
			//OutputDebugStringW(buffer);
		}

		// bitmap 이미지 중앙으로 옮기기
		auto size = bitmaps[i].first->GetSize();
		D2D1_RECT_F drawRect = D2D1::RectF(
			-size.width / 2.0f,
			-size.height / 2.0f,
			size.width / 2.0f,
			size.height / 2.0f
		);

		m_d2dDeviceContext->DrawBitmap(bitmaps[i].first.Get(), drawRect); // Bitmap 출력
	}
}

D2D1::Matrix3x2F D2DRenderManager::GetRenderMatrix(Transform* transform)
{
	if (transform == nullptr)
		return D2D1::Matrix3x2F::Identity();

	float scaleX = 1.0f;
	float scaleY = transform->IsUnityCoords() ? -1.0f : 1.0f; // 유니티 좌표계면 y축 상하 반전

	float offsetX = 0.0f;
	float offsetY = 0.0f;

	return D2D1::Matrix3x2F::Scale(scaleX, scaleY) * D2D1::Matrix3x2F::Translation(offsetX, offsetY);
}
