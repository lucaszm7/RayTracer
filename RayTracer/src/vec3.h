#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "rtweekend.h"

using std::sqrt;


struct vec3
{
public:
	double e[3];

	vec3() : e{0, 0, 0} {}

	vec3(double e0, double e1, double e2) : e{e0, e1, e2} {}

	bool operator == (const vec3& other) 
	{
		return e[0] == other.e[0] && e[1] == other.e[1] && e[2] == other.e[2];
	}

	vec3(const vec3& v) = default;
	vec3& operator=(const vec3& v) = default;

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }

	vec3 operator - () const { return vec3(-e[0], -e[1], -e[2]); }

	double operator [] (int i) const { return e[i]; }
	double& operator [] (int i) { return e[i]; }

	vec3 operator + (const vec3& v) const
	{
		return vec3(e[0] + v.e[0], e[1] + v.e[1], e[2] + v.e[2]);
	}

	vec3& operator += (const vec3& v)
	{
		this->e[0] += v.e[0];
		this->e[1] += v.e[1];
		this->e[2] += v.e[2];
		return *this;
	}

	vec3& operator *= (const double& f)
	{
		e[0] *= f;
		e[1] *= f;
		e[2] *= f;
		return *this;
	}

	vec3& operator /= (const double t)
	{
		return *this *= 1 / t;
	}

	double lenght_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	double lenght() const
	{
		return sqrt(lenght_squared());
	}

	inline static vec3 random()
	{
		return vec3(random_double(), random_double(), random_double());
	}

	inline static vec3 random(double min, double max)
	{
		return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
	}

	bool near_zero() const
	{
		const auto s = 1e-18;
		return (fabs(e[0]) < s && fabs(e[1]) < s && fabs(e[2]) < s);
	}

};


// Type aliases for vec3

using point3 = vec3;
using color = vec3;


// Utility Functions

inline std::ostream& operator << (std::ostream& out, const vec3& v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline vec3 operator - (vec3 u, vec3 v)
{
	return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline vec3 operator * (const vec3& u, const vec3& v)
{
	return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline vec3 operator * (double t, const vec3& v)
{
	return vec3(v.e[0] * t, v.e[1] * t, v.e[2] * t);
}

inline vec3 operator * (const vec3& v, double t)
{
	return t * v;
}

inline vec3 operator / (vec3 v, double t)
{
	return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v)
{
	return u.e[0] * v.e[0] + 
		   u.e[1] * v.e[1] + 
		   u.e[2] * v.e[2];
}

inline vec3 cross(const vec3& u, const vec3& v)
{
	return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
				u.e[2] * v.e[0] - u.e[0] * v.e[2], 
				u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline vec3 unit_vector(vec3 v)
{
	return v / v.lenght();
}

vec3 random_in_unit_sphere()
{
	while (true)
	{
		auto p = vec3::random(-1, 1);
		if (p.lenght_squared() >= 1) continue;
		return p;
	}
}

vec3 random_unit_vector()
{
	return unit_vector(random_in_unit_sphere());
}

vec3 reflect(const vec3& v, const vec3& n)
{
	return v - 2 * dot(v, n) * n;
}

vec3 refract(const vec3& uv, const vec3& n, double etai_over_etat)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	vec3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
	vec3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.lenght_squared())) * n;
	return r_out_perp + r_out_parallel;
}

vec3 random_in_unit_disk()
{
	while (true)
	{
		auto p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
		if (p.lenght_squared() >= 1) continue;
		return p;
	}
}

#endif