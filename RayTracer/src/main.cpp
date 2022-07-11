#include "rtweekend.h"

#include "color.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"

#include "Timer.h"
#include "image.h"

#include <iostream>
#include <fstream>
#include <windows.h>
#include <chrono>
#include <thread>
#include <string>
#include <omp.h>


color ray_color(const ray& r, const hittable& world, int depth)
{
	hit_record rec;

	// If we've exceeded the ray bounce limit, no more light is gathered
	if (depth <= 0)
		return color(0, 0, 0);

	if (world.hit(r, 0.001, infinity, rec))
	{
		ray scattered;
		color attenuation;
		if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
			return attenuation * ray_color(scattered, world, depth - 1);
		return color(0, 0, 0);
	}

	// Sky gradient
	vec3 unit_direction = unit_vector(r.direction());
	auto t = 0.5 * (unit_direction.y() + 1.0);

	return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
	// return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.9, 0.6, 0.2);
}

hittable_list random_scene()
{
	hittable_list world;

	auto ground_material = make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; ++a)
	{
		for (int b = -11; b < 11; ++b)
		{
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).lenght() > 0.9)
			{
				shared_ptr<material> sphere_material;

				if (choose_mat < 0.6)
				{
					// Diffuse
					auto albedo = color::random() * color::random();
					sphere_material = make_shared<lambertian>(albedo);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				
				else if (choose_mat < 0.9)
				{
					// Metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = make_shared<metal>(albedo, fuzz);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
				
				else
				{
					// Glass
					sphere_material = make_shared<dielectric>(1.5);
					world.add(make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = make_shared<dielectric>(1.5);
	world.add(make_shared<sphere>(point3(0, 1, 0), 1.0, material1));
	
	auto material2 = make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));
	
	auto material3 = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}


void sum(color* out, color* in)
{
	*out += *in;
}

//#pragma omp declare reduction( + : vec3 : omp_out + omp_in) \
//								initializer( omp_priv = {0, 0, 0} )


int main()
{
	// Output
	std::ofstream image("image.ppm");


	// Image
	constexpr double aspect_ratio = 16.0 / 9.0;
	constexpr int image_width = 200;
	constexpr int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 20;
	const int max_depth = 10;


	// World
	hittable_list world = random_scene();


	// Camera
	point3 lookfrom(12, 3, 10);
	point3 lookat(0, 0, 0);
	vec3 vup(0, 1, 0);
	double aperture = 0.1;
	double dist_to_focus = 10.0;
	camera cam(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus);
	
	// Image Buffer
	auto pixels = new color[image_width][image_height];

	// Render
	image << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	{
		Timer timer;
		#pragma omp parallel for \
			schedule(dynamic, 1) \
			shared(std::cerr, timer, pixels, image_width, image_height, samples_per_pixel, world, cam) \
			num_threads(omp_get_num_procs())
		for (int j = image_height - 1; j >= 0; --j)
		{
			if(omp_get_thread_num() == 0)
			{
				printf("\rTime taken: %.4fs", timer.now());
			}
			for (int i = 0; i < image_width; ++i)
			{
				for (int s = 0; s < samples_per_pixel; ++s)
				{
					auto u = (i + random_double()) / (image_width - 1);
					auto v = (j + random_double()) / (image_height - 1);

					ray r = cam.get_ray(u, v);

					pixels[i][j] += (ray_color(r, world, max_depth));
				}
			}
		}
	}

	std::cout << "Writing to file..." << std::endl;
	write_color<image_height>(image, pixels, image_width, image_height, samples_per_pixel);
	image.close();
	std::cout << "Finished!" << std::endl;

	// ShellExecute(0, 0, L"image.ppm", 0, 0, SW_SHOW);

	delete[] pixels;

	std::cin.get();
}