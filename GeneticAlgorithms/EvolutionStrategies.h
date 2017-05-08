#ifndef _EVO_H_
#define _EVO_H_

#include <functional>
#include "XORShift.h"
#include <algorithm>
#include <vector>
#include <cassert>

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

			if (population.bestGenome == nullptr || population.bestGenome->fitness < population.genoms[genCnt].fitness)
			{
				population.bestGenome = &population.genoms[genCnt];
			}
		}
	}


	template<typename GT>
	inline GT MergeGenoms(GT** parents, size_t parentCnt)
	{
		assert(parentCnt > 0 && "At least one parent has to be used");

		GT newChild;

		// Accumulate the gens of all parents together
		for (size_t i = 0; i < parentCnt; i++)
		{
			for (size_t gen = 0; gen < parents[0]->length; gen++)
			{
				newChild.data[gen] = parents[i]->data[gen];
			}
		}

		// Now divide every gen by the number of parents
		for (size_t gen = 0; gen < parents[0]->length; gen++)
		{
			newChild.data[gen] /= parentCnt;
		}

		return newChild;
	}


	template<typename T, typename GT, typename signature>
	inline GT* OnePlusOne(T& population, std::function<signature>& fitnessFunction)
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

	template<typename T, typename GT, typename signature>
	inline GT* MuPlusLambda(T& population, std::function<signature>& fitnessFunction, size_t m, size_t l)
	{
		// New population has to have enough space for parents + new children
		std::vector<GT> newPopulation(m + l);

		// Copy the parents as they are into the new population
		memcpy(&newPopulation[0], &population.genoms[0], sizeof(GT) * population.size);

		// Iterate over all parents, if there are less than req. child, select more than once
		for (size_t p = population.size; p < (l + m); p++)
		{
			// Randomly choose a parent within the old population and mutate it
			size_t index = random::xorshift::IntRange(0, population.size - 1);
			GT newChild = population.genoms[index];

			// Mutate the selected parent just like 1+1
			for (size_t genIndex = 0; genIndex < newChild.length; genIndex++)
			{
				newChild.data[genIndex] += random::xorshift::IntRange(-5, 5);
			}
			newChild.fitness = fitnessFunction(newChild);

			// Early exit, if we found a proper genom return it
			if (newChild.fitness == 0)
			{
				population.genoms[index] = newChild;
				return &population.genoms[index];
			}

			newPopulation[p] = newChild;
		}

		// Sort ascending considering the genoms fitness
		std::sort(newPopulation.begin(), newPopulation.end(), [](GT& lhs, GT& rhs)
		{
			return lhs.fitness < rhs.fitness;
		});

		// The best mu genoms become the new generation
		memcpy(&population.genoms[0], &newPopulation[0], sizeof(GT) * m);

		return nullptr;
	}

	template<typename T, typename GT, typename signature>
	inline GT* MuCommaLambda(T& population, std::function<signature>& fitnessFunction, size_t m, size_t l)
	{
		// New population has to have enough space for new children
		std::vector<GT> newPopulation(l);

		// Iterate over all parents, if there are less than req. child, select more than once
		for (size_t p = 0; p < l; p++)
		{
			// Randomly choose a parent within the old population and mutate it
			size_t index = random::xorshift::IntRange(0, population.size - 1);
			GT newChild = population.genoms[index];

			// Mutate the selected parent just like 1+1
			for (size_t genIndex = 0; genIndex < newChild.length; genIndex++)
			{
				newChild.data[genIndex] += random::xorshift::IntRange(-5, 5);
			}
			newChild.fitness = fitnessFunction(newChild);

			// Early exit, if we found a proper genom return it
			if (newChild.fitness == 0)
			{
				population.genoms[index] = newChild;
				return &population.genoms[index];
			}

			newPopulation[p] = newChild;
		}

		// Sort ascending considering the genoms fitness
		std::sort(newPopulation.begin(), newPopulation.end(), [](GT& lhs, GT& rhs)
		{
			return lhs.fitness < rhs.fitness;
		});

		// The best mu genoms become the new generation
		memcpy(&population.genoms[0], &newPopulation[0], sizeof(GT) * m);

		return nullptr;

	}

	template<typename T, typename GT, typename signature>
	inline GT* MuHashLambda(T& population, std::function<signature>& fitnessFunction, size_t m, size_t l, size_t q)
	{
		// New population has to have enough space for new children
		std::vector<GT> newPopulation(l);

		// Create lambda new children
		for (size_t i = 0; i < l; i++)
		{
			std::vector<GT*> parents(q);

			// Choose q number of genoms to serve as parents for the new child
			for (size_t j = 0; j < q; j++)
			{
				size_t pIndex = random::xorshift::IntRange(0, population.size - 1);
				parents[j] = &population.genoms[pIndex];
			}

			// Inheritance: Whether via Combination or Merge
			GT newChild = MergeGenoms<GT>(&parents[0], q);

			// Mutate the child
			for (size_t genIndex = 0; genIndex < newChild.length; genIndex++)
			{
				newChild.data[genIndex] += random::xorshift::IntRange(-5, 5);
			}
			newChild.fitness = fitnessFunction(newChild);

			// Early exit, if we found a proper genom return it
			if (newChild.fitness == 0)
			{
				population.genoms[0] = newChild;
				return &population.genoms[0];
			}

			newPopulation[i] = newChild;
		}

		// Sort ascending considering the genoms fitness
		std::sort(newPopulation.begin(), newPopulation.end(), [](GT& lhs, GT& rhs)
		{
			return lhs.fitness < rhs.fitness;
		});

		// The best mu genoms become the new generation
		memcpy(&population.genoms[0], &newPopulation[0], sizeof(GT) * m);

		return nullptr;
	}
}

#endif