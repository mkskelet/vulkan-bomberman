#include "GameTime.h"

GameTime* GameTime::Instance = nullptr;

GameTime::GameTime(std::chrono::steady_clock::time_point startTime)
{
	this->startTime = std::chrono::time_point_cast<std::chrono::milliseconds>(startTime);
	this->time = this->startTime;
	Instance = this;
}

void GameTime::UpdateTime(std::chrono::steady_clock::time_point currentTime)
{
	time = std::chrono::time_point_cast<std::chrono::milliseconds>(currentTime);
}

float GameTime::GetTime() const
{
	return std::chrono::duration<float, std::chrono::milliseconds::period>(time - startTime).count();
}