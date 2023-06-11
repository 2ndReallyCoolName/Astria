#pragma once
#include <chrono>

class AstriaTimer
{
public:
	AstriaTimer() noexcept;
	float Mark() noexcept;
	float Peek() const noexcept;
private:
	std::chrono::steady_clock::time_point last;
};