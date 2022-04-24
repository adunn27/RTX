#pragma once

// Functions to generate the scenes from the RayTracingInAWeekend books

#include "hittable_list.h"
#include "texture.h"
#include "material.h"
#include "sphere.h"
#include "rect.h"
#include "box.h"
#include "constant_medium.h"
#include "bvh.h"

hittable_list earth() {
	auto earth_texture = std::make_shared<image_texture>("texture\\earthmap.jpg");
	auto earth_surface = std::make_shared<lambertian>(earth_texture);
	auto globe = std::make_shared<sphere>(point3(0, 0, 0), 2, earth_surface);

	return hittable_list(globe);
}

hittable_list random_scene() {
	hittable_list world;

	auto ground_material = std::make_shared<lambertian>(color(0.5, 0.5, 0.5));
	world.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, ground_material));

	for (int a = -11; a < 11; a++) {
		for (int b = -11; b < 11; b++) {
			auto choose_mat = random_double();
			point3 center(a + 0.9 * random_double(), 0.2, b + 0.9 * random_double());

			if ((center - point3(4, 0.2, 0)).length() > 0.9) {
				std::shared_ptr<material> sphere_material;

				if (choose_mat < 0.8) {
					// diffuse
					auto albedo = color::random() * color::random();
					sphere_material = std::make_shared<lambertian>(albedo);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else if (choose_mat < 0.95) {
					// metal
					auto albedo = color::random(0.5, 1);
					auto fuzz = random_double(0, 0.5);
					sphere_material = std::make_shared<metal>(albedo, fuzz);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
				else {
					// glass
					sphere_material = std::make_shared<dielectric>(1.5);
					world.add(std::make_shared<sphere>(center, 0.2, sphere_material));
				}
			}
		}
	}

	auto material1 = std::make_shared<dielectric>(1.5);
	world.add(std::make_shared<sphere>(point3(0, 1, 0), 1.0, material1));

	auto material2 = std::make_shared<lambertian>(color(0.4, 0.2, 0.1));
	world.add(std::make_shared<sphere>(point3(-4, 1, 0), 1.0, material2));

	auto material3 = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	world.add(std::make_shared<sphere>(point3(4, 1, 0), 1.0, material3));

	return world;
}

hittable_list simple_light() {
	hittable_list objects;

	auto pertext = std::make_shared<solid_color>(color(0.8, 0.5, 0.3));
	objects.add(std::make_shared<sphere>(point3(0, -1000, 0), 1000, std::make_shared<lambertian>(pertext)));
	objects.add(std::make_shared<sphere>(point3(-2, 2, 3), 2, std::make_shared<lambertian>(pertext)));

	auto difflight = std::make_shared<diffuse_light>(color(4, 4, 4));
	objects.add(std::make_shared<xy_rect>(3, 5, 1, 3, -2, difflight));

	return objects;
}

hittable_list cornell_box() {
	hittable_list objects;

	auto red = std::make_shared<lambertian>(color(.65, .05, .05));
	auto white = std::make_shared<lambertian>(color(.73, .73, .73));
	auto green = std::make_shared<lambertian>(color(.12, .45, .15));
	auto light = std::make_shared<diffuse_light>(color(15, 15, 15));

	// Create walls and light
	objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 555, green));
	objects.add(std::make_shared<yz_rect>(0, 555, 0, 555, 0, red));
	objects.add(std::make_shared<xz_rect>(213, 343, 227, 332, 554, light));
	objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 0, white));
	objects.add(std::make_shared<xz_rect>(0, 555, 0, 555, 555, white));
	objects.add(std::make_shared<xy_rect>(0, 555, 0, 555, 555, white));

	// Add boxes
	std::shared_ptr<hittable> box0 = std::make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), white);
	box0 = std::make_shared<rotate_y>(box0, 15);
	box0 = std::make_shared<translate>(box0, vec3(265, 0, 295));
	objects.add(box0);

	std::shared_ptr<hittable> box1 = std::make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
	box1 = std::make_shared<rotate_y>(box1, -18);
	box1 = std::make_shared<translate>(box1, vec3(130, 0, 65));
	objects.add(std::make_shared<constant_medium>(box1, 0.01, color(1, 1, 1)));

	return objects;
}

hittable_list final_scene() {
	hittable_list boxes1;
	auto ground = std::make_shared<lambertian>(color(0.48, 0.83, 0.53));

	const int boxes_per_side = 20;
	for (int i = 0; i < boxes_per_side; i++) {
		for (int j = 0; j < boxes_per_side; j++) {
			auto w = 100.0;
			auto x0 = -1000.0 + i * w;
			auto z0 = -1000.0 + j * w;
			auto y0 = 0.0;
			auto x1 = x0 + w;
			auto y1 = random_double(1, 101);
			auto z1 = z0 + w;

			boxes1.add(std::make_shared<box>(point3(x0, y0, z0), point3(x1, y1, z1), ground));
		}
	}

	hittable_list objects;

	objects.add(std::make_shared<bvh_node>(boxes1, 0, 1));

	auto light = std::make_shared<diffuse_light>(color(7, 7, 7));
	objects.add(std::make_shared<xz_rect>(123, 423, 147, 412, 554, light));

	auto shiny_metal = std::make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
	objects.add(std::make_shared<sphere>(point3(400, 400, 200), 50, shiny_metal));

	objects.add(std::make_shared<sphere>(point3(260, 150, 45), 50, std::make_shared<dielectric>(1.5)));
	objects.add(std::make_shared<sphere>(
		point3(0, 150, 145), 50, std::make_shared<metal>(color(0.8, 0.8, 0.9), 1.0)
		));

	auto boundary = std::make_shared<sphere>(point3(360, 150, 145), 70, std::make_shared<dielectric>(1.5));
	objects.add(boundary);
	objects.add(std::make_shared<constant_medium>(boundary, 0.2, color(0.2, 0.4, 0.9)));
	boundary = std::make_shared<sphere>(point3(0, 0, 0), 5000, std::make_shared<dielectric>(1.5));
	objects.add(std::make_shared<constant_medium>(boundary, .0001, color(1, 1, 1)));

	objects.add(std::make_shared<sphere>(point3(400, 200, 400), 100, std::make_shared<lambertian>(color(1, 0, 0))));
	auto emat = std::make_shared<lambertian>(std::make_shared<image_texture>("texture\\earthmap.jpg"));
	objects.add(std::make_shared<sphere>(point3(220, 280, 300), 80, emat));

	hittable_list boxes2;
	auto white = std::make_shared<lambertian>(color(.73, .73, .73));
	int ns = 1000;
	for (int j = 0; j < ns; j++) {
		boxes2.add(std::make_shared<sphere>(point3::random(0, 165), 10, white));
	}

	objects.add(std::make_shared<translate>(
		std::make_shared<rotate_y>(
			std::make_shared<bvh_node>(boxes2, 0.0, 1.0), 15),
		vec3(-100, 270, 395)
		)
	);

	return objects;
}

