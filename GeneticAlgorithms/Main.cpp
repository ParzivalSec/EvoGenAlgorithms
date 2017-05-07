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
	Population<Genome<int, 4>, 100> int_population;

	std::function<int(Genome<int, 4>)> fitness = [](Genome<int, 4>& genom)
	{
		if (genom.data[2] < genom.data[3])
		{
			genom.fitness = INT_MIN;
		}
		else
		{
			
		}
	};

	evo::InitPopulation<Population<Genome<int, 4>, 100>, int(Genome<int, 4>)>(int_population, -500, 500, fitness);

	for(size_t i = 0; i < int_population.size; i++)
	{
		int_population.genoms[i].fitness = fitness(int_population.genoms[i]);
	}

	return 0;
}