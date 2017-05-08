#pragma once

#include "Population.h"
#include <functional>
#include "EvolutionStrategies.h"
#include <cassert>

template<typename G, size_t size>
class Solver
{
public:
	Solver(std::function<int(G)>& fitnessFunc);

	G* OnePlusOne(size_t maxIterations);
	G* MuPlusLamda(size_t maxIterations, size_t mu, size_t lambda);
	G* MuCommaLamda(size_t maxIterations, size_t mu, size_t lambda);
	G* MuHashLamda(size_t maxIterations, size_t mu, size_t lambda, size_t rho);

private:
	Population<G, size> m_population;
	std::function<int(G)>& m_fitnessFunction;
	G* m_solution;
};

template <typename G, size_t size>
Solver<G, size>::Solver(std::function<int(G)>& fitnessFunc)
	: m_population()
	, m_fitnessFunction(fitnessFunc)
	, m_solution(nullptr)
{
	evo::InitPopulation<Population<G, size>, int(G)>(m_population, -500, 500, m_fitnessFunction);
}

template <typename G, size_t size>
inline G* Solver<G, size>::OnePlusOne(size_t maxIterations)
{
	for (size_t it = 0; it < maxIterations; it++)
	{
		m_solution = evo::OnePlusOne<Population<G, size>, G, int(G)>(m_population, m_fitnessFunction);

		if (m_solution != nullptr)
		{
			printf("Solution found after %llu iterations! \n", it);
			printf("Solution is <( %d , %d , %d , %d )>\n", m_solution->data[0], m_solution->data[1], m_solution->data[2], m_solution->data[3]);
			
			return m_solution;
		}
	}

	return m_solution;
}

template <typename G, size_t size>
inline G* Solver<G, size>::MuPlusLamda(size_t maxIterations, size_t mu, size_t lambda)
{
	assert(mu <= m_population.size && "Mu has to be less or equal the iteration size");

	for (size_t it = 0; it < maxIterations; it++)
	{
		m_solution = evo::MuPlusLambda<Population<G, size>, G, int(G)>(m_population, m_fitnessFunction, mu, lambda);

		if (m_solution != nullptr)
		{
			printf("Solution found after %llu iterations! \n", it);
			printf("Solution is <( %d , %d , %d , %d )>\n", m_solution->data[0], m_solution->data[1], m_solution->data[2], m_solution->data[3]);

			return m_solution;
		}
	}

	return m_solution;
}

template <typename G, size_t size>
inline G* Solver<G, size>::MuCommaLamda(size_t maxIterations, size_t mu, size_t lambda)
{
	assert(mu <= m_population.size && "Mu has to be less or equal the iteration size");

	for (size_t it = 0; it < maxIterations; it++)
	{
		m_solution = evo::MuCommaLambda<Population<G, size>, G, int(G)>(m_population, m_fitnessFunction, mu, lambda);

		if (m_solution != nullptr)
		{
			printf("Solution found after %llu iterations! \n", it);
			printf("Solution is <( %d , %d , %d , %d )>\n", m_solution->data[0], m_solution->data[1], m_solution->data[2], m_solution->data[3]);

			return m_solution;
		}
	}

	return m_solution;
}

template <typename G, size_t size>
inline G* Solver<G, size>::MuHashLamda(size_t maxIterations, size_t mu, size_t lambda, size_t rho)
{
	assert(mu <= m_population.size && "Mu has to be less or equal the iteration size");

	for (size_t it = 0; it < maxIterations; it++)
	{
		m_solution = evo::MuHashLambda<Population<G, size>, G, int(G)>(m_population, m_fitnessFunction, mu, lambda, rho);

		if (m_solution != nullptr)
		{
			printf("Solution found after %llu iterations! \n", it);
			printf("Solution is <( %d , %d , %d , %d )>\n", m_solution->data[0], m_solution->data[1], m_solution->data[2], m_solution->data[3]);

			return m_solution;
		}
	}

	return m_solution;
}
