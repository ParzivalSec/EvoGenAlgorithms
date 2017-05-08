#pragma once
#include "Population.h"
#include <functional>
#include "EvolutionStrategies.h"


template <typename G, size_t N, size_t populationSize>
class QueensSolverTemplated
{
public:
	QueensSolverTemplated(std::function<int(G)>& fitnessFunc);
	G* CalculateSolution(size_t maxIterations, bool elitist);

private:

	void SelectParents(size_t count);
	void Crossover();
	void Mutate(size_t amount);

	void PointCross(G* l, G* r);


	Population<G, populationSize> m_population;
	std::function<int(G)>& m_fitnessFunction;
	std::vector<G*> m_parents;
	G* m_solution;
	bool m_elitist;
};

template <typename G, size_t N, size_t populationSize>
QueensSolverTemplated<G, N, populationSize>::QueensSolverTemplated(std::function<int(G)>& fitnessFunc)
	: m_population()
	, m_fitnessFunction(fitnessFunc)
	, m_parents()
	, m_solution(nullptr)
	, m_elitist(false)
{
	evo::InitPopulation<Population<G, populationSize>, int(G)>(m_population, 0, N - 1, m_fitnessFunction);
}

template <typename G, size_t N, size_t populationSize>
G* QueensSolverTemplated<G, N, populationSize>::CalculateSolution(size_t maxIterations, bool elitist)
{
	size_t selectionCount = 200;
	size_t mutationAmount = 1;
	m_elitist = elitist;

	// For maIterations, try to find a solution where n*n queens can be placed
	for (size_t it = 0; it < maxIterations; it++)
	{
		SelectParents(selectionCount);
		Crossover();
		Mutate(mutationAmount);

		for (size_t i = 0; i < m_parents.size(); ++i)
		{
			m_parents[i]->fitness = m_fitnessFunction(*m_parents[i]);
			if (m_parents[i]->fitness == 0)
			{
				m_solution = m_parents[i];
			}
		}

		if (m_solution != nullptr)
		{
			printf("Solution found after %llu iterations\n", it);
			return m_solution;
		}
	}

	return nullptr;
}

template <typename G, size_t N, size_t populationSize>
void QueensSolverTemplated<G, N, populationSize>::SelectParents(size_t count)
{
	m_parents.resize(count);

	size_t index = 0;
	if (m_elitist)
	{
		m_parents[0] = m_population.bestGenome;
		++index;
	}

	// Select randomly count amount of parents for reproduction
	for (index; index < count; index++)
	{
		size_t parentIndex = random::xorshift::IntRange(0, m_population.size - 1);
		m_parents[index] = &m_population.genoms[parentIndex];
	}
}

template <typename G, size_t N, size_t populationSize>
void QueensSolverTemplated<G, N, populationSize>::Crossover(void)
{
	// Do crossover with parents, only keep fittest of both
	for (size_t i = 0; i < m_parents.size(); i += 2)
	{
		PointCross(m_parents[i], m_parents[i + 1]);
	}
}

template <typename G, size_t N, size_t populationSize>
void QueensSolverTemplated<G, N, populationSize>::Mutate(size_t amount)
{
	size_t mutation = 0;

	// For every crossed parent check whether there shall be a mutation
	for (size_t i = 0; i < m_parents.size(); i++)
	{
		mutation = random::xorshift::IntRange(0, 10);

		if (mutation == 1)
		{
			mutation = random::xorshift::IntRange(0, N - 1);
			m_parents[i]->data[mutation] = random::xorshift::IntRange(-amount, amount);
			if (m_parents[i]->data[mutation] < 0)
			{
				m_parents[i]->data[mutation] = 0;
			}
			else if (m_parents[i]->data[mutation] > 7)
			{
				m_parents[i]->data[mutation] = 7;
			}
		}
	}
}

template <typename G, size_t N, size_t populationSize>
void QueensSolverTemplated<G, N, populationSize>::PointCross(G* l, G* r)
{
	size_t cutPt = random::xorshift::IntRange(1, N - 2);

	for (size_t i = cutPt; i < N; ++i)
	{
		// INFO: Nice swap without tmp var for evey allele to swap
		r->data[i] += l->data[i];
		l->data[i] = r->data[i] - l->data[i];
		r->data[i] -= l->data[i];
	}
}
