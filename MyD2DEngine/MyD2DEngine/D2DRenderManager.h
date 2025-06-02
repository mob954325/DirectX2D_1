#pragma once
#include "framework.h"

using namespace Microsoft::WRL;

// Direct2D 코드 재활용 하기 위한 클래스
class D2DRenderManager
{
public:
	void Initialize();
	void Uninitialize();

	/// <summary>
	/// WIC를 통해 이미지를 ID2D1Bitmap1**로 반환하는 함수
	/// </summary>
	/// <param name="path">이미지 경로</param>
	/// <param name="outBitmap">반환 될 Bitmap</param>
	/// <returns>상태</returns>
	HRESULT CreateBitmapFromFile(const wchar_t* path, ID2D1DeviceContext7* pDeviceContext, ID2D1Bitmap1** outBitmap);

protected:
	ComPtr<IWICImagingFactory> g_wicImagingFactory;
};