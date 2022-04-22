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

color ray_color(const ray& r, const hittable& world) {
    hit_record rec;
    if (world.hit(r, 0, inf, rec)) {
        return 0.5 * (rec.normal + color(1, 1, 1));
    }
    vec3 unit_direction = r.direction().unit_vector();
    auto t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
}

int main() {

    // Image
    
    const double aspect_ratio = 16.0 / 9.0;
    const int image_height = 256;
    const int image_width = static_cast<int>(aspect_ratio * image_height);
    const int samples_per_pixel = 100;

    // World
    hittable_list world;
    world.add(std::make_shared<sphere>(point3(0, 0, -1), 0.5));
    world.add(std::make_shared<sphere>(point3(0, -100.5, -1), 100));

    // Camera
    camera cam;

    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; j--) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;

        for (int i = 0; i < image_width; i++) {
            color pix(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; s++) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                pix += ray_color(cam.get_ray(u, v), world);
            }
            write_color(std::cout, pix, samples_per_pixel);
        }
    }

    std::cerr << "\nDone.\n";
}

