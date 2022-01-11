#ifndef RTWEEKEND_H
#define RTWEEKEND_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>
#include <random>


// Using

using std::shared_ptr;
using std::make_shared;
using std::sqrt;


// Constants

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;


// Utility Functions

inline double degree_to_radians(double degree)
{
	return degree * pi / 180.0 ;
}

inline double random_double()
{
	// Returns a number between [0, 1).
	return rand() / (RAND_MAX + 1.0);
	
	/*static std::uniform_real_distribution<double> distribution(0.0, 1.0);
	static std::mt19937 generator;
	return distribution(generator);*/
}

inline double random_double(double min, double max)
{
	// Returns a number between [min, max).
	return min + (max - min) * random_double();
}

inline double clamp(double x, double min, double max)
{
	if (x < min) return min;
	if (x > max) return max;
	return x;
}


// Commom Headers

#include "ray.h"
#include "vec3.h"
#include "Timer.h"
#include "hittable_list.h"


#endif