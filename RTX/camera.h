#pragma once

#include "vec3.h"
#include "ray.h"

class camera {
private:
	point3 origin;
	point3 bottom_left_corner;
	vec3 horizontal;
	vec3 vertical;

public:
	camera(
		point3 from,
		point3 to,
		vec3 vup,
		double vfov,
		double aspect_ratio
	) {
		auto theta = deg_to_rad(vfov);
		double viewport_height = 2.0 * tan(theta/2);
		double viewport_width = aspect_ratio * viewport_height;
		double focal_length = 1.0;

		auto w = (from - to).unit_vector();
		auto u = vector_cross(vup, w).unit_vector();
		auto v = vector_cross(w, u);

		origin = from;
		horizontal = viewport_width * u;
		vertical = viewport_height * v;
		bottom_left_corner = origin - horizontal / 2 - vertical / 2 - w;
	}

	ray get_ray(double u, double v) const {
		return ray(origin, bottom_left_corner + u * horizontal + v * vertical - origin);
	}	

};
