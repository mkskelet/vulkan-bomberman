#pragma once

#include <chrono>

class GameTime
{
private:
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> startTime;
	std::chrono::time_point<std::chrono::high_resolution_clock, std::chrono::milliseconds> time;

public:
	static GameTime* Instance;
	GameTime(std::chrono::high_resolution_clock::time_point startTime);
	void UpdateTime(std::chrono::steady_clock::time_point currentTime);		///< Adds time.
	float GetTime() const;				///< Returns time since game started (in miliseconds).
};

