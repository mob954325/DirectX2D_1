#pragma once
#include "../MyD2DEngine/Application.h"
#include "../MyD2DEngine/Transform.h"
#include <vector>

class DemoGameApp : public Application
{
public:
	void Initialize() override;
	void Render() override;
	void Uninitialize() override;
	void Update() override;

	Transform* camera;
	Transform* transform_Sun;
	Transform* transform_Earth;
	Transform* transform_Moon;

private:
	void FlagInputUpdate();		// 인풋으로 플래그 전환 내용 업데이트
	void MoveCamera();			// 카메라 이동 관리
	void SolarSystemUpdate();	// 태양계 이동 관리

	std::vector<Transform*> objects;

	float baseRotateSpeed = 360.0f;
	float cameraMoveSpeed = 10.0f;

	float sunRotateRatio = 0.1f;
	float earthRotateRatio = 0.5f;
	float moonRotateRatio = 0.8f;

	bool isSolarSystemMove = true; // 태양계 움직임 여부
};