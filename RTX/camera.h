#pragma once

#include "vec3.h"
#include "ray.h"

class camera {
private:
	point3 origin;
	point3 bottom_left_corner;
	vec3 horizontal;
	vec3 vertical;
	vec3 u, v, w;
	double lens_radius;

public:
	camera(
		point3 from,
		point3 to,
		vec3 vup,
		double vfov,
		double aspect_ratio,
		double aperature,
		double focus_dist
	) {
		auto theta = deg_to_rad(vfov);
		double viewport_height = 2.0 * tan(theta/2);
		double viewport_width = aspect_ratio * viewport_height;
		double focal_length = 1.0;

		w = (from - to).unit_vector();
		u = vector_cross(vup, w).unit_vector();
		v = vector_cross(w, u);

		origin = from;
		horizontal = focus_dist * viewport_width * u;
		vertical = focus_dist * viewport_height * v;
		bottom_left_corner = origin - horizontal / 2 - vertical / 2 - focus_dist * w;

		lens_radius = aperature / 2;
	}

	ray get_ray(double s, double t) const {
		vec3 rd = lens_radius * random_in_unit_disk();
		vec3 offset = u * rd.x() + v * rd.y();
		return ray(origin + offset, bottom_left_corner + s * horizontal + t * vertical - origin - offset);
	}	

};
