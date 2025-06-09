#include "DemoGameApp.h"
#include "../MyD2DEngine/D2DRenderManager.h"
#include "../MyD2DEngine/Singleton.h"
#include "../MyD2DEngine/Input.h"
#include "../MyD2DEngine/GameTime.h"

void DemoGameApp::Initialize()
{
	__super::Initialize(); // Application 초기화

	camera = new Transform();

	// Sun -> Earth -> Moon

	transform_Sun = new Transform();
	transform_Sun->SetScale(0.1f, 0.1f);
	HRESULT hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Sun.png", transform_Sun);
	assert(SUCCEEDED(hr));

	transform_Earth = new Transform();
	transform_Earth->SetPosition(0.0f, 1000.0f);
	transform_Earth->SetParent(transform_Sun);
	hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Earth.png", transform_Earth);
	assert(SUCCEEDED(hr));

	transform_Moon = new Transform();
	transform_Moon->SetPosition(0.0f, 1000.0f);
	transform_Moon->SetParent(transform_Earth);
	hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Moon.png", transform_Moon);
	assert(SUCCEEDED(hr));
}

void DemoGameApp::Render()
{
	__super::Render();
}

void DemoGameApp::Uninitialize()
{
	__super::Uninitialize();

	// delete transform
	transform_Sun = nullptr;
	transform_Earth = nullptr;
	transform_Moon = nullptr;

	delete transform_Sun;
	delete transform_Earth;
	delete transform_Moon;
}

void DemoGameApp::Update()
{
	__super::Update();

	if (Singleton<Input>::GetInstance().IsKeyDown('D'))
	{
		D2D1_VECTOR_2F position = transform_Sun->GetPosition();
		transform_Sun->SetPosition(position.x + moveSpeed, position.y);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('A'))
	{
		D2D1_VECTOR_2F position = transform_Sun->GetPosition();
		transform_Sun->SetPosition(position.x - moveSpeed, position.y);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('W'))
	{
		D2D1_VECTOR_2F position = transform_Sun->GetPosition();
		transform_Sun->SetPosition(position.x, position.y + moveSpeed);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('S'))
	{
		D2D1_VECTOR_2F position = transform_Sun->GetPosition();
		transform_Sun->SetPosition(position.x, position.y - moveSpeed);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('R'))
	{
		// rotation
		float rotation_sun = transform_Sun->GetRotation();
		transform_Sun->SetRotation(rotation_sun + rotSpeed * Singleton<GameTime>::GetInstance().GetDeltaTime());

		float rotation_earth = transform_Earth->GetRotation();
		transform_Earth->SetRotation(rotation_earth + rotSpeed * 0.8f * Singleton<GameTime>::GetInstance().GetDeltaTime());

		float rotation_moon = transform_Moon->GetRotation();
		transform_Moon->SetRotation(rotation_moon + rotSpeed * 0.6f * Singleton<GameTime>::GetInstance().GetDeltaTime());
	}
}
