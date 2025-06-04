#pragma once
#include "../MyD2DEngine/Application.h"

class DemoGameApp : public Application
{
public:
	void Initialize() override;
	void Render() override;
	void Uninitialize() override;
	//void Run() override;

	ComPtr<ID2D1Bitmap1> g_d2dBitmapFromFile; // 그릴 이미지 비트맵
};