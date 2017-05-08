#pragma once
#include <functional>
#include <vector>


struct DynamicGenom
{
	DynamicGenom(size_t genLength)
		: gens(genLength)
		, fitness(INT_MAX)
	{}

	std::vector<size_t> gens;
	int64_t fitness;
};

struct DynamicPopulation
{
	DynamicPopulation(size_t popSize)
		: genoms(popSize)
		, bestGenom(nullptr)
	{}

	std::vector<DynamicGenom> genoms;
	DynamicGenom* bestGenom;
};


class QueensSolverDynamic
{
	QueensSolverDynamic(std::function<int(DynamicGenom)>& fitnessFunc, size_t genomSize, size_t populationSize);
	DynamicGenom* CalculateSolution(size_t maxIterations, bool elitist);

private:

	void SelectParents(size_t count);
	void Crossover();
	void Mutate(size_t amount);

	void PointCross(DynamicGenom* l, DynamicGenom* r);


	DynamicPopulation m_population;
	std::function<int(DynamicGenom)>& m_fitnessFunction;
	std::vector<DynamicGenom*> m_parents;
	DynamicGenom* m_solution;
	bool m_elitist;
};
