#pragma once

#include <array>
#include <algorithm>
#include "Casilla.h"

#define NADA 25
#define TACHADO NADA+1
#define LINEA TACHADO+1
#define BINGO LINEA+1

template <typename T, size_t SIZE>
class Linea
{
private:
	std::array<T, SIZE> m_lineaNumbers;
	int m_crossNumbersCounter;
	std::array<std::shared_ptr<Casilla>, SIZE> casillas;
	void UpdateValues();

public:
	Linea();
	void SetNumbers(std::array<T, SIZE> numbersInLinea);
	int InsertNumber(T number);
	bool Complete();
	void AssignCasilla(std::shared_ptr<Casilla> cas, int pos);
	std::shared_ptr<Casilla> GetCasilla(int index);
	void InjectCasillas(std::array<std::shared_ptr<Casilla>, SIZE> toInject);
	void Reset();
};


template<typename T, size_t SIZE>
inline Linea<T, SIZE>::Linea()
	: m_crossNumbersCounter(0)
{
}

template<typename T, size_t SIZE>
inline void Linea<T, SIZE>::SetNumbers(std::array<T, SIZE> numbersInLinea)
{
	m_lineaNumbers = numbersInLinea;
}

template<typename T, size_t SIZE>
inline int Linea<T, SIZE>::InsertNumber(T number)
{
	auto it = std::find(m_lineaNumbers.begin(), m_lineaNumbers.end(), number);
	if (it != m_lineaNumbers.end())
	{
		int i = std::distance(m_lineaNumbers.begin(), it);
		casillas[i]->Disable();
		m_crossNumbersCounter++;
		return TACHADO;
	}
	return NADA;
}

template<typename T, size_t SIZE>
inline bool Linea<T, SIZE>::Complete()
{
	return SIZE == m_crossNumbersCounter;
}

template<typename T, size_t SIZE>
inline void Linea<T, SIZE>::AssignCasilla(std::shared_ptr<Casilla> cas, int pos)
{
	casillas[pos] = cas;
	auto a = m_lineaNumbers[pos];
	auto l = std::to_string(a);
	cas->SetTextInCasilla(l);
}

template<typename T, size_t SIZE>
inline std::shared_ptr<Casilla> Linea<T, SIZE>::GetCasilla(int index)
{
	return casillas[index];
}

template<typename T, size_t SIZE>
inline void Linea<T, SIZE>::InjectCasillas(std::array<std::shared_ptr<Casilla>, SIZE> toInject)
{
	for (int i = 0;i < SIZE;i++)
	{
		AssignCasilla(toInject[i], i);
	}
}

template<typename T, size_t SIZE>
inline void Linea<T, SIZE>::UpdateValues()
{
	for (int i = 0;i < SIZE;i++)
	{
		casillas[i]->SetTextInCasilla(std::to_string(m_lineaNumbers[i]));
	}
}

template<typename T, size_t SIZE>
inline void Linea<T, SIZE>::Reset()
{
	m_crossNumbersCounter = 0;
	UpdateValues();
}
