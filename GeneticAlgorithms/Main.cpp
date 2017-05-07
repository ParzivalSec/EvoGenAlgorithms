#include <iostream>
#include "Genome.h"
#include "Population.h"
#include "EvolutionStrategies.h"

int main(int argc, char** argv)
{
	// TEST: BEGIN
	{
		Genome<int, 8> genome_chess;

		Population<Genome<int, 8>, 200> int_8_population;

		std::cout << "PopSize: " << int_8_population.size << std::endl;

		int_8_population.genoms[0].data[7] = 77;

		std::function<int(Genome<int, 8>)> fitness_2 = [](Genome<int, 8>& genome) { return genome.data[7]; };
	}
	// TEST: END

	Genome<int, 4> genome;
	Population<Genome<int, 4>, 10000> int_population;

	std::function<int(Genome<int, 4>)> fitness = [&](Genome<int, 4>& genom)
	{
		if (genom.data[2] < genom.data[3])
		{
			return genom.fitness = INT_MAX;
		}
		else
		{
			int left_side = (3 * (genom.data[0] * genom.data[0])) + (5 * (genom.data[1] * genom.data[1] * genom.data[1]));
			int right_side = (7 * genom.data[2]) + (3 * (genom.data[3] * genom.data[3]));
			genom.fitness = left_side - right_side;

			genom.fitness < 0 ? genom.fitness *= -1 : (void)0;

			return genom.fitness;
		}
	};

	evo::InitPopulation<Population<Genome<int, 4>, 10000>, int(Genome<int, 4>)>(int_population, -500, 500, fitness);

	const int max_iteration = 10000;

	for (size_t it = 0; it < max_iteration; it++)
	{
		Genome<int, 4>* solution = evo::OnePlusOne<Population<Genome<int, 4>, 10000>, Genome<int, 4>, int(Genome<int, 4>)>
									(int_population, fitness);

		if (solution != nullptr)
		{
			printf("Solution found after %llu iterations! \n", it);
			printf("Solution is <( %d , %d , %d , %d )>\n", solution->data[0], solution->data[1], solution->data[2], solution->data[3]);
			break;
		}
	}

	return 0;
}