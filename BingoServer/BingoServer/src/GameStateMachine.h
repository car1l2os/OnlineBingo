#pragma once
#include "Timer.h"
#include <string>
#include <algorithm>
#include <array>
#include <time.h>


enum class GameState { Idle, Buying, Playing };

template<size_t MIN_RANGE, size_t MAX_RANGE>
class GameStateMachine
{
private:
	GameState m_state;
	Timer m_timer;
	int m_indexForReturn;
	void GenerateNumbers();
	std::array<int, MAX_RANGE - MIN_RANGE + 1> m_numbersPool;
	int  m_numberPlayers = false;

public:
	GameStateMachine();
	std::string Update();
	std::string ChangeState(GameState newState);
	void PlayerInGame();
	void PlayerLeavesGame();
	bool HasPlayers();
};

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline void GameStateMachine<MIN_RANGE, MAX_RANGE>::GenerateNumbers()
{
	srand(time(0));
	int toInsert = MIN_RANGE;
	for (int i = 0;i < m_numbersPool.size();i++, toInsert++)
	{
		m_numbersPool[i] = toInsert;
	}
	std::random_shuffle(m_numbersPool.begin(), m_numbersPool.end());
}

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline GameStateMachine<MIN_RANGE, MAX_RANGE>::GameStateMachine()
	: m_state(GameState::Idle), m_timer(Timer(1, 60)), m_indexForReturn(-1)
{
	srand(time(0));
}

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline std::string GameStateMachine<MIN_RANGE, MAX_RANGE>::Update()
{
	bool flag = false;
	m_timer.Update();
	if (m_timer.StepDone() && (m_state == GameState::Playing || !m_timer.FinishedCountDown()))
	{
		switch (m_state)
		{
		case GameState::Idle:
			break;
		case GameState::Buying:
			return std::string("#TIM").append(std::to_string((int)m_timer.m_counter / 1000));
			m_indexForReturn = -1;
			break;
		case GameState::Playing:
			m_indexForReturn++;
			if (m_indexForReturn == m_numbersPool.size())
				return ChangeState(GameState::Buying);
			return std::string("#NUM").append(std::to_string(m_numbersPool[m_indexForReturn]));
			break;
		default:
			break;
		}
	}
	else if (m_timer.FinishedCountDown() && m_state != GameState::Playing && m_numberPlayers)
		return ChangeState(GameState::Playing);
	else if(!m_numberPlayers && m_timer.FinishedCountDown() && m_state == GameState::Buying)
		return ChangeState(GameState::Buying);
	return std::string("");
}

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline std::string GameStateMachine<MIN_RANGE, MAX_RANGE>::ChangeState(GameState newState)
{
	m_state = newState;
	switch (newState)
	{
	case GameState::Idle:
		break;
	case GameState::Buying:
		m_indexForReturn = -1;
		m_numberPlayers = 0;
		m_timer.m_step = 1000;
		m_timer.Reset();
		return std::string("!FIN");
		break;
	case GameState::Playing:
		m_timer.m_step = 2000; //mejor game feel
		GenerateNumbers();
		return std::string("!STR");
		break;
	default:
		break;
	}
}

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline void GameStateMachine<MIN_RANGE, MAX_RANGE>::PlayerInGame()
{
	m_numberPlayers++;;
}

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline void GameStateMachine<MIN_RANGE, MAX_RANGE>::PlayerLeavesGame()
{
	m_numberPlayers--;
}

template<size_t MIN_RANGE, size_t MAX_RANGE>
inline bool GameStateMachine<MIN_RANGE, MAX_RANGE>::HasPlayers()
{
	return m_numberPlayers != 0;
}
