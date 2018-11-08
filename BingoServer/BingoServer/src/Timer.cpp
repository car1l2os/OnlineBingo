#include "Timer.h"

Timer::Timer(float step, float counter)
	: lastStep(std::chrono::high_resolution_clock::now()), now(std::chrono::high_resolution_clock::now()), deltaTime(0), m_step(step * 1000), m_counter(counter * 1000), m_maxCounter(counter*1000)
{
}

Timer::~Timer()
{

}

void Timer::Update()
{
	now = std::chrono::high_resolution_clock::now();
	auto timeStep = now - lastStep;
	lastStep = now;
	auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(timeStep);
	deltaTime += ms.count();
	m_counter -= ms.count();
}

bool Timer::StepDone()
{
	if (deltaTime >= m_step)
	{
		deltaTime -= m_step;
		return true;
	}
	return false;
}

bool Timer::FinishedCountDown()
{
	return m_counter <= 0;
}

void Timer::Reset()
{
	lastStep = std::chrono::high_resolution_clock::now();
	now = std::chrono::high_resolution_clock::now();
	deltaTime = 0;
	m_counter = m_maxCounter;
}
