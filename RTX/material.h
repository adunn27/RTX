#pragma once

#include "utility.h"
#include "ray.h"

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	color albedo;

public:
	lambertian(const color& a) : albedo(a) {}
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		auto scatter_dir = rec.normal + random_unit_vector();

		// Degenerate scatter directions
		if (scatter_dir.near_zero())
			scatter_dir = rec.normal;

		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo;
		return true;
	}
};

class metal : public material {
public:
	color albedo;
	double fuzz;

public:
	metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		vec3 refl = reflect(r_in.direction().unit_vector(), rec.normal);
		scattered = ray(rec.p, refl + fuzz * random_in_unit_sphere());
		attenuation = albedo;
		return vector_dot(scattered.direction(), rec.normal) > 0;
	}
};

