#pragma once
#include "Genome.h"

/**
 * Population
 */

template<typename T, size_t populationSize>
struct Population
{
	size_t size = populationSize;
	T genoms[populationSize];
	T* bestGenome;
};