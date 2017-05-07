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


	template<typename T, typename GT, typename signature>
	GT* OnePlusOne(T& population, std::function<signature>& fitnessFunction)
	{
		for (size_t i = 0; i < population.size; i++)
		{
			GT parent = population.genoms[i];

			for (size_t genIndex = 0; genIndex < population.genoms[i].length; genIndex++)
			{
				population.genoms[i].data[genIndex] += random::xorshift::IntRange(-5, 5);
			}
			population.genoms[i].fitness = fitnessFunction(population.genoms[i]);
			
			if (population.genoms[i].fitness == 0)
			{
				return &population.genoms[i];
			}
			
			if (parent.fitness < population.genoms[i].fitness)
			{
				population.genoms[i] = parent;
			}
		}

		return nullptr;
	}

	void MuPlusLambda() {}
	void MuCommaLambda() {}
	void MuHashLambda() {}
}

