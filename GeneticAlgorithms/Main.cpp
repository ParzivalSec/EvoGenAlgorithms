#include "Genome.h"
#include "Solver.h"
#include "QueensSolverTemplated.h"
#include "cmdline.h"

int main(int argc, char** argv)
{
	cmdline::parser cmd_parser;

	cmd_parser.add<std::string>("program", 'p', "Program to start", false, "solver", cmdline::oneof<std::string>("solver", "queens", "queens_templated"));
	cmd_parser.add<std::string>("solver_modes", 'm', "Evolution strategy of solver", false, "OplusO", cmdline::oneof<std::string>("OplusO", "MplusL", "McommaL", "MhashL"));
	cmd_parser.add<size_t>("queens", 'n', "If mode queen, n*n queens", false, 8);
	cmd_parser.add<size_t>("queens_template", 't', "Try templated versions of queens", false, 8);
	cmd_parser.parse(argc, argv);

	std::string programMode = cmd_parser.get<std::string>("program");
	if (programMode == "solver")
	{
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

		Solver<Genome<int, 4>, 1000> solver(fitness);
		Genome<int, 4>* sol = nullptr;

		std::string mode = cmd_parser.get<std::string>("solver_modes");
		if (mode == "OplusO")
		{
			sol = solver.OnePlusOne(10000);
		}
		else if (mode == "MplusL")
		{
			sol = solver.MuPlusLamda(10000, 1000, 2000);
		}
		else if (mode == "McommaL")
		{
			sol = solver.MuCommaLamda(10000, 1000, 2000);
		}
		else if (mode == "MhashL")
		{
			sol = solver.MuHashLamda(10000, 1000, 2000, 2);
		}

		if (sol == nullptr)
		{
			printf("No solution found for solver!\n");
		}
	}
	else if (programMode == "queens_templated")
	{
		size_t queensCount = cmd_parser.get<size_t>("queens_template");

		std::function<int(Genome<int, 8>)> queenFitness = [&](Genome<int, 8>& genom)
		{
			size_t collision = 0;

			for (size_t startGen = 0; startGen < 8; startGen++)
			{
				for (size_t gen = startGen + 1; gen < 8; gen++)
				{
					// Is there a queen at the same row in another column
					if (genom.data[startGen] == genom.data[gen])
					{
						++collision;
					}

					size_t offset = gen - startGen;

					// Check the diagonals too
					if (genom.data[startGen] == genom.data[gen] - offset || genom.data[startGen] == genom.data[gen] + offset)
					{
						++collision;
					}
				}
			}

			return collision;
		};

		QueensSolverTemplated<Genome<int, 8>, 8, 1000> queenSolver(queenFitness);

		Genome<int, 8>* quenSol = queenSolver.CalculateSolution(5000, false);

		if (quenSol != nullptr)
		{
			printf("Solution < ");
			for (size_t i = 0; i < queensCount; i++)
				printf("%d, ", quenSol->data[i]);
			printf(">\n");
		}
	}
	else if (programMode == "queens")
	{
		size_t queensCount = cmd_parser.get<size_t>("queens");
	}

	return 0;
}
