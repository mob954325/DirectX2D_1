#pragma once
#include "framework.h"
#include "Transform.h"
#include "IRenderer.h"

using namespace Microsoft::WRL;

// Direct2D 코드 재활용 하기 위한 클래스
class D2DRenderManager
{
public:
	void Initialize();
	void Uninitialize();
	void Render();

	void SetD2D1DeviceContext7(ID2D1DeviceContext7* pD2D1DeviceContext7);
	void SetScreenSize(int width, int height);

	int GetScreenWidthSize() { return screenWidth; };
	int GetScreenHeightSize() { return screenHeight; };

	void SetBitmapTransform(D2D1_MATRIX_3X2_F& finalMatrix);
	void DrawBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap);
	void DrawBitmap(Microsoft::WRL::ComPtr<ID2D1Bitmap1> bitmap, D2D1_RECT_F& destRect);

	/// <summary>
	/// WIC를 통해 이미지를 ID2D1Bitmap1**로 반환하는 함수
	/// </summary>
	/// <param name="path">이미지 경로</param>
	/// <param name="outBitmap">반환 될 Bitmap</param>
	/// <returns>상태</returns>
	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1Bitmap1** outBitmap);
	void AddRenderObject(IRenderer* comp);
	void PrintText(const wchar_t* str, float left, float top);

protected:
	ComPtr<IWICImagingFactory> m_wicImagingFactory;
	ComPtr<ID2D1DeviceContext7> m_d2dDeviceContext;

	// DWrite
	ComPtr<IDWriteFactory> m_pDWriteFactory;
	ComPtr<IDWriteTextFormat> m_pDWriteTextFormat;
	ComPtr<ID2D1SolidColorBrush> m_pBrush; // 브러시 개체 인터페이스 포인터 변수

	std::vector<IRenderer*> renderList;

	int screenWidth = 0;
	int screenHeight = 0;
};