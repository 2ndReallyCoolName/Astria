#include "AstriaTimer.h"

using namespace std::chrono;

AstriaTimer::AstriaTimer() noexcept
{
	last = steady_clock::now();
}

float AstriaTimer::Mark() noexcept
{
	const auto old = last;
	last = steady_clock::now();
	const duration<float> frameTime = last - old;
	return frameTime.count();
}

float AstriaTimer::Peek() const noexcept
{
	return duration<float>(steady_clock::now() - last).count();
}