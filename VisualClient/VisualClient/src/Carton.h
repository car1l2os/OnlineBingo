#pragma once

#include <array>
#include <algorithm>
#include "Linea.h"

template <size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
class Carton
{
private:
	std::array<Linea<int, NUMBER_PER_LINEA>, LINEAS> m_lineas;
	int m_lineasComplete;
	void GenerateValues(bool isGoingToBeInGame);

public:
	Carton();
	int InsertNumber(int number);
	bool Complete();
	void Reset(bool isGoingToPlay);
	std::array<int, MAX_RANGE - MIN_RANGE + 1> m_numbersPool; //I'm going to work with ints instead of typename T for code clarity //TODO-make this static
	Linea<int, NUMBER_PER_LINEA>& GetLinea(int index);
	void InjectCasillas(std::array<std::array<std::shared_ptr<Casilla>, NUMBER_PER_LINEA>, LINEAS> toInject);
};

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::GenerateValues(bool isGoingToBeInGame)
{
	if (isGoingToBeInGame)
	{
		std::random_shuffle(m_numbersPool.begin(), m_numbersPool.end());

		for (int i = 0;i < LINEAS;i++)
		{
			std::array<int, NUMBER_PER_LINEA> tmp;
			for (int j = 0;j < NUMBER_PER_LINEA;j++)
			{
				tmp[j] = m_numbersPool[i*NUMBER_PER_LINEA + j];
			}
			m_lineas[i].SetNumbers(tmp);
		}
	}
	else
	{
		for (int i = 0;i < LINEAS;i++)
		{
			std::array<int, NUMBER_PER_LINEA> tmp;
			for (int j = 0;j < NUMBER_PER_LINEA;j++)
			{
				tmp[j] = NO_NUMBER;
			}
			m_lineas[i].SetNumbers(tmp);
		}
	}
}

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::Carton()
{
	m_lineasComplete = 0;
	//create el pool
	int toInsert = MIN_RANGE;
	for (int i = 0;i < m_numbersPool.size();i++, toInsert++)
	{
		m_numbersPool[i] = toInsert;
	}

	GenerateValues(false);
}

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline int Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::InsertNumber(int number)
{
	for (int i = 0;i < LINEAS;i++)
	{
		if (m_lineas[i].InsertNumber(number) == TACHADO)
		{
			if (m_lineas[i].Complete())
			{
				m_lineasComplete++;
				if (!Complete())
				{
					return LINEA;
				}
				else
					return BINGO;
			}
		}
	}
	return NADA;
}

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline bool Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::Complete()
{
	return m_lineasComplete == LINEAS;
}

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::Reset(bool isGoingToPlay)
{
	m_lineasComplete = 0;
	GenerateValues(isGoingToPlay);

	for (int i = 0;i < LINEAS;i++)
		m_lineas[i].Reset();

}

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline Linea<int, NUMBER_PER_LINEA>& Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::GetLinea(int index)
{
	return m_lineas[index];
}

template<size_t MIN_RANGE, size_t MAX_RANGE, size_t LINEAS, size_t NUMBER_PER_LINEA>
inline void Carton<MIN_RANGE, MAX_RANGE, LINEAS, NUMBER_PER_LINEA>::
InjectCasillas(
	std::array<
	std::array<
	std::shared_ptr<Casilla>,
	NUMBER_PER_LINEA>, LINEAS> toInject)
{
	for (int i = 0;i < LINEAS;i++)
	{
		m_lineas[i].InjectCasillas(toInject[i]);
	}
}






