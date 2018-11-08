#pragma once
#include "Carton.h"
#include <time.h>

template <size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
class Partida
{
private:
	std::array<Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>, MAX_CARTONES> m_cartones;
	int m_currentCartones;
	int m_isGoingToPlayNextGame;
	bool ready;

public:
	Partida();
	int InsertNumber(int number);
	void StartNewGame(int cartones);
	int GetLineas() { return LINEAS; };
	int GetNumbersPerLinea() { return NUMBER_PER_LINEA; };
	Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>& GetCarton(int index);
	void InjectCasillas(std::array<std::array<std::array<std::shared_ptr<Casilla>, NUMBER_PER_LINEA>, LINEAS>, MAX_CARTONES> toInject);
	void UpdateValues();
	bool Ready();
	void SetReady();

	void SetGoingToPlayNextGame(const bool value) { m_isGoingToPlayNextGame = value; }
	bool GetGoingToPlayNextGame() const { return m_isGoingToPlayNextGame; }

	int points;
	int lastPrize;
};




template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::Partida()
{
	srand(time(0));
	points = 0;
	ready = false;
}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline int Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::InsertNumber(int number)
{
	int result;
	int toReturn = NADA;
	for (int i = 0;i < m_currentCartones;i++)
	{
		result = m_cartones[i].InsertNumber(number);

		if (result > toReturn)
		{
			toReturn = result; //no podemos saltar de nada a bingo así que ++
		}
		if (result == LINEA && lastPrize < LINEA)
			points += 3;
		else if (result == BINGO)
			points += 10;

	}

	if (lastPrize < toReturn)
	{
		lastPrize = toReturn;
		return lastPrize;
	}
	else
		return NADA;

}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::StartNewGame(int cartones)
{
	m_currentCartones = cartones;
	lastPrize = NADA;
	//points = 0;
	//std::fill(m_cartones.begin(), m_cartones.end(), -1);
	for (int i = 0;i < MAX_CARTONES;i++)
	{
		if (i < cartones)
			m_cartones[i].Reset(true); //this carton va a jugar
		else
			m_cartones[i].Reset(false); //este no
	}
}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>& Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::GetCarton(int index)
{
	return m_cartones[index];
}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::
InjectCasillas(std::array<
	std::array<
	std::array<std::shared_ptr<Casilla>,
	NUMBER_PER_LINEA>, LINEAS>, MAX_CARTONES> toInject)
{
	for (int i = 0;i < MAX_CARTONES;i++)
	{
		auto t = toInject[i];

		m_cartones[i].InjectCasillas(t);
	}
}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::UpdateValues()
{
	for (int i = 0;i < MAX_CARTONES;i++)
		m_cartones.UpdateValues();
}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline bool Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::Ready()
{
	return ready;
}

template<size_t MAX_CARTONES, size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Partida<MAX_CARTONES, MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::SetReady()
{
	ready = true;
}

