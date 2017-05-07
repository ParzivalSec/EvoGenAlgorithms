#pragma once

/**
 * Generic genome POD
 * Provide genome data type and genome size as template parameter
 */

template<typename T, int size>
struct Genome
{
	T data[size];
	size_t length = size;
	int32_t fitness;
};