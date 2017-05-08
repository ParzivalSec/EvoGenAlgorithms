#include "QueensSolverDynamic.h"
#include "XORShift.h"


QueensSolverDynamic::QueensSolverDynamic(std::function<int(DynamicGenom)>& fitnessFunc, size_t genomSize, size_t populationSize)
	: m_population(populationSize)
	, m_fitnessFunction(fitnessFunc)
	, m_parents()
	, m_solution(nullptr)
	, m_elitist(false)
{
	for (size_t i = 0; i < m_population.genoms.size(); i++)
	{
		m_population.genoms[i].gens.resize(genomSize);

		for (size_t genIndex = 0; genIndex < genomSize; genIndex++)
		{
			m_population.genoms[i].gens[genIndex] = random::xorshift::IntRange(0, genomSize - 1);
		}

		m_population.genoms[i].fitness = m_fitnessFunction(m_population.genoms[i]);

		if (m_population.bestGenom == nullptr || m_population.bestGenom->fitness < m_population.genoms[i].fitness)
		{
			m_population.bestGenom = &m_population.genoms[i];
		}
	}
}

DynamicGenom* QueensSolverDynamic::CalculateSolution(size_t maxIterations, bool elitist) {}
void QueensSolverDynamic::SelectParents(size_t count) {}
void QueensSolverDynamic::Crossover() {}
void QueensSolverDynamic::Mutate(size_t amount) {}
void QueensSolverDynamic::PointCross(DynamicGenom* l, DynamicGenom* r) {}
