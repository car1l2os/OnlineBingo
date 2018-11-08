#pragma once
#include <chrono>
#include <iostream>

struct Timer
{
	std::chrono::time_point<std::chrono::steady_clock> lastStep;
	std::chrono::time_point<std::chrono::steady_clock> now;
	float deltaTime;
	float m_step;
	float m_counter;
	float m_maxCounter;

	Timer(float timeLimit, float counter);
	~Timer();
	void Update();
	bool StepDone();
	bool FinishedCountDown();
	void Reset();
};

