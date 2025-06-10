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
	transform_Sun->SetCamera(camera);
	HRESULT hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Sun.png", transform_Sun);
	assert(SUCCEEDED(hr));

	transform_Earth = new Transform();
	transform_Earth->SetScale(0.5f, 0.5f);
	transform_Earth->SetPosition(0.0f, 1000.0f);
	transform_Earth->SetParent(transform_Sun);
	transform_Earth->SetCamera(camera);
	hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Earth.png", transform_Earth);
	assert(SUCCEEDED(hr));

	transform_Moon = new Transform();
	transform_Moon->SetScale(0.5f, 0.5f);
	transform_Moon->SetPosition(0.0f, 1000.0f);
	transform_Moon->SetParent(transform_Earth);
	transform_Moon->SetCamera(camera);
	hr = m_D2DRenderManager->CreateBitmapFromFile(L"../Resource/Moon.png", transform_Moon);
	assert(SUCCEEDED(hr));

	objects.push_back(transform_Sun);
	objects.push_back(transform_Earth);
	objects.push_back(transform_Moon);
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

	FlagInputUpdate();
	MoveCamera();
	SolarSystemUpdate();
}

void DemoGameApp::FlagInputUpdate()
{
	if (Singleton<Input>::GetInstance().IsKeyPressed('R')) // R : 그림 움직임 
	{		
		isSolarSystemMove = !isSolarSystemMove;
	}

	if (Singleton<Input>::GetInstance().IsKeyPressed('T')) // T : 회전 방향 반전
	{
		baseRotateSpeed *= -1;
	}

	if (Singleton<Input>::GetInstance().IsKeyPressed('Y')) // Y : 유니티 좌표계 사용
	{
		for (Transform* transform : objects)
		{
			transform->SetIsUnityCoords(!transform->IsUnityCoords());
		}
	}
}

void DemoGameApp::MoveCamera()
{
	if (Singleton<Input>::GetInstance().IsKeyDown('D'))
	{
		D2D1_VECTOR_2F position = camera->GetPosition();
		camera->SetPosition(position.x + cameraMoveSpeed, position.y);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('A'))
	{
		D2D1_VECTOR_2F position = camera->GetPosition();
		camera->SetPosition(position.x - cameraMoveSpeed, position.y);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('W'))
	{
		D2D1_VECTOR_2F position = camera->GetPosition();
		camera->SetPosition(position.x, position.y + cameraMoveSpeed);
	}
	if (Singleton<Input>::GetInstance().IsKeyDown('S'))
	{
		D2D1_VECTOR_2F position = camera->GetPosition();
		camera->SetPosition(position.x, position.y - cameraMoveSpeed);
	}
}

void DemoGameApp::SolarSystemUpdate()
{
	if (!isSolarSystemMove) return;

	float rotation_sun = transform_Sun->GetRotation();
	transform_Sun->SetRotation(rotation_sun + baseRotateSpeed * sunRotateRatio * Singleton<GameTime>::GetInstance().GetDeltaTime());

	float rotation_earth = transform_Earth->GetRotation();
	transform_Earth->SetRotation(rotation_earth + baseRotateSpeed * earthRotateRatio * Singleton<GameTime>::GetInstance().GetDeltaTime());

	//float rotation_moon = transform_Moon->GetRotation();
	//transform_Moon->SetRotation(rotation_moon + baseRotateSpeed * moonRotateRatio * Singleton<GameTime>::GetInstance().GetDeltaTime());
}
