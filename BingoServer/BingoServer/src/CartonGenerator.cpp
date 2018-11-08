#include "CartonGenerator.h"

CartonGenerator::CartonGenerator(int nMin, int nMax)
{
	nRange.reserve(nMax - nMin + 1);
	for (int i = nMin;i <= nMax;i++)
	{
		nRange.emplace_back(i);
	}
}

CartonGenerator::~CartonGenerator()
{
}

std::string CartonGenerator::GenerateCarton(int size)
{
	std::random_shuffle(nRange.begin(), nRange.end());
	std::string toReturn("");
	for (int i = 0;i < size;i++)
	{
		toReturn.append(" " + std::to_string(nRange[i]));
	}

	return toReturn;
}
