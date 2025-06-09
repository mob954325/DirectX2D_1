#pragma once
#include <iostream>
#include <windows.h>
#include "Singleton.h"

class GameTime : Singleton<GameTime>
{
public:
	GameTime() = default;
	~GameTime() = default;

	void InitTime();
	void UpdateTime();
	float GetDeltaTime();
	float GetElapsedTime();

private:
	LARGE_INTEGER freq, curr, prev, start, end;
	float deltaTime;
	float timeElapsed;
};
