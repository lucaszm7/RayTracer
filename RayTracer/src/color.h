#ifndef COLOR_H
#define COLOR_H

#include <iostream>
#include <fstream>

#include "rtweekend.h"

void write_color(std::ofstream& out, color& pixel_color, int samples_per_pixel)
{
	auto r = pixel_color.x();
	auto g = pixel_color.y();
	auto b = pixel_color.z();

	// Divide the color by the number of samples ans gamma correct for gamma=2.0
	auto scale = 1.0 / samples_per_pixel;
	r = sqrt(scale * r);
	b = sqrt(scale * b);
	g = sqrt(scale * g);

	// Write the translated [0, 255] value of each color component
	out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
}

template <size_t cols>
void write_color(std::ofstream& out, color(*pixels)[cols], int image_width, int image_height, int samples_per_pixel)
{
	for (int j = image_height - 1; j >= 0; --j)
	{
		for (int i = 0; i < image_width; ++i)
		{
			auto r = pixels[i][j].x();
			auto g = pixels[i][j].y();
			auto b = pixels[i][j].z();

			// pixels[i].e[0] = pixels[i].e[1] = pixels[i].e[2] = 0;

			// Divide the color by the number of samples ans gamma correct for gamma=2.0
			auto scale = 1.0 / samples_per_pixel;
			r = sqrt(scale * r);
			b = sqrt(scale * b);
			g = sqrt(scale * g);

			// Write the translated [0, 255] value of each color component
			out << static_cast<int>(256 * clamp(r, 0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(g, 0, 0.999)) << ' '
				<< static_cast<int>(256 * clamp(b, 0, 0.999)) << '\n';
		}
	}
}

#endif