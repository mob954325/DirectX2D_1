#pragma once
#include "../MyD2DEngine/Application.h"
#include "../MyD2DEngine/Transform.h"

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
	float rotSpeed = 360.0f;
	float moveSpeed = 10.0f;
};