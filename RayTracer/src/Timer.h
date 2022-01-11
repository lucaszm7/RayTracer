#pragma once

#include <iostream>
#include <chrono>

struct Timer
{
	std::chrono::steady_clock::time_point start;
	std::chrono::duration<double> duration;

	Timer()
	{
		start = std::chrono::high_resolution_clock::now();
	}

	~Timer()
	{
		duration = std::chrono::high_resolution_clock::now() - start;
		std::cout << "\nTook: " << duration.count() * 1000.0 << "ms\n";
	}

	double now()
	{
		duration = std::chrono::high_resolution_clock::now() - start;
		return duration.count() * 1000.0;
	}

};