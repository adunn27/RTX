// RTX.cpp : This file contains the 'main' function. Program execution begins and ends there.
// Based on https://raytracing.github.io/books/RayTracingInOneWeekend.html#overview

#include "utility.h"
#include "vec3.h"
#include "color.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "camera.h"
#include "material.h"
#include "scenes.h"

color ray_color(const ray& r, const color& background_color, const hittable& world, int depth) {
	if (depth < 1)
		return color(0, 0, 0);

	hit_record rec;

	// If ray hits nothing return the void's color
	if (!world.hit(r, 0.001, inf, rec))
		return background_color;


	ray scattered;
	color attenuation;
	color emitted = rec.material_ptr->emitted(rec.u, rec.v, rec.p);
	
	if (!rec.material_ptr->scatter(r, rec, attenuation, scattered))
		return emitted;

	return emitted + attenuation * ray_color(scattered, background_color, world, depth - 1);
}

int main() {

	// Image
	
	const auto aspect_ratio = 3.0 / 2.0;
	const int image_width = 500;
	const int image_height = static_cast<int>(image_width / aspect_ratio);
	const int samples_per_pixel = 500;
	const int max_depth = 50;

	// World

	hittable_list world;
	color background(0, 0, 0);

	// Camera

	point3 lookfrom;
	point3 lookat;
	vec3 vup(0, 1, 0);
	auto dist_to_focus = 10.0;
	auto aperture = 0.1;
	auto vfov = 40.0;

	// Scene Picker
	switch (0) {
	case 1:
		world = random_scene();
		background = color(0.70, 0.80, 1.00);
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		aperture = 0.1;
		break;
	case 2:
		world = earth();
		lookfrom = point3(13, 2, 3);
		lookat = point3(0, 0, 0);
		vfov = 20.0;
		break;
	default:
	case 3:
		world = simple_light();
		lookfrom = point3(26, 3, 6);
		lookat = point3(0, 2, 0);
		vfov = 20.0;
		break;
	}

	camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus);

	// Render

	std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

	for (int j = image_height - 1; j >= 0; j--) {
		std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

		for (int i = 0; i < image_width; i++) {
			color pix(0, 0, 0);
			for (int s = 0; s < samples_per_pixel; s++) {
				auto u = (i + random_double()) / (image_width - 1);
				auto v = (j + random_double()) / (image_height - 1);
				pix += ray_color(cam.get_ray(u, v), background, world, max_depth);
			}
			write_color(std::cout, pix, samples_per_pixel);
		}
	}

	std::cerr << "\nDone.\n";
}

