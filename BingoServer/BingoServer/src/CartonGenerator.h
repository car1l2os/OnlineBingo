#pragma once

#include <string>
#include <vector>
#include <algorithm>    // std::random_shuffle

class CartonGenerator
{
private:
	std::vector<int> nRange;

public:
	CartonGenerator(int nMin, int nMax);
	~CartonGenerator();
	std::string GenerateCarton(int size);
};

