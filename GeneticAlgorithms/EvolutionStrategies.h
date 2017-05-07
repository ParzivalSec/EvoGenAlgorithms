#pragma once
#include <functional>
#include "XORShift.h"

namespace evo
{
	template<typename T, typename signature>
	void InitPopulation(T& population, int minInitVal, int maxInitVal, std::function<signature>& fitnessFunction)
	{
		for (size_t genCnt = 0; genCnt < population.size; genCnt++)
		{
			for (size_t genIndex = 0; genIndex < population.genoms[genCnt].length; genIndex++)
			{
				population.genoms[genCnt].data[genIndex] = random::xorshift::IntRange(minInitVal, maxInitVal);
			}
			population.genoms[genCnt].fitness = fitnessFunction(population.genoms[genCnt]);
		}
	}


	template<typename T, typename signature>
	void OnePlusOne(T& population, std::function<signature>& fitnessFunction)
	{

	}

	void MuPlusLambda() {}
	void MuCommaLambda() {}
	void MuHashLambda() {}
}

