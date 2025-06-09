#include "pch.h"
#include "GameTime.h"

void GameTime::InitTime()
{
    QueryPerformanceFrequency(&freq); // QPC 주파수(초당 Tick 수) 가져오기
    QueryPerformanceCounter(&start);
    QueryPerformanceCounter(&prev);

    deltaTime = 0;
    timeElapsed = 0;
}

void GameTime::UpdateTime()
{
    QueryPerformanceCounter(&curr);
    deltaTime = static_cast<float>(curr.QuadPart - prev.QuadPart) / freq.QuadPart;
    prev = curr;
}

float GameTime::GetDeltaTime()
{
    return deltaTime;
}

float GameTime::GetElapsedTime()
{
    QueryPerformanceCounter(&end);
    return static_cast<float>(end.QuadPart - start.QuadPart) / freq.QuadPart;
}