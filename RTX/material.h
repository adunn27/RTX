#pragma once

#include "utility.h"
#include "ray.h"
#include "texture.h"

struct hit_record;

class material {
public:
	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const = 0;
};

class lambertian : public material {
public:
	std::shared_ptr<texture> albedo;

public:
	lambertian(const color& a) : albedo(std::make_shared<solid_color>(a)) {}
	lambertian(std::shared_ptr<texture> a) : albedo(a) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		auto scatter_dir = rec.normal + random_unit_vector();

		// Degenerate scatter directions
		if (scatter_dir.near_zero())
			scatter_dir = rec.normal;

		scattered = ray(rec.p, scatter_dir);
		attenuation = albedo->value(rec.u, rec.v, rec.p);
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

class dielectric : public material {
public:
	double refraction_idx;

private:
	static double reflectance(double cos, double refr_idx) {
		// Schlick's apprx
		auto r0 = pow((1 - refr_idx) / (1 + refr_idx), 2);
		return r0 + (1 - r0) * pow(1 - cos, 5);
	}

public:
	dielectric(double refraction_idx) : refraction_idx(refraction_idx) {}

	virtual bool scatter(const ray& r_in, const hit_record& rec, color& attenuation, ray& scattered) const override {
		attenuation = color(1.0, 1.0, 1.0);
		double refr_ratio = rec.front_face ? (1.0 / refraction_idx) : refraction_idx;

		vec3 unit_dir = r_in.direction().unit_vector();

		double cos_theta = fmin(vector_dot(-unit_dir, rec.normal), 1.0);
		double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

		vec3 dir;
		if (refr_ratio * sin_theta > 1.0 || reflectance(cos_theta, refr_ratio) > random_double())
			dir = reflect(unit_dir, rec.normal);
		else
			dir = refract(unit_dir, rec.normal, refr_ratio);

		scattered = ray(rec.p, dir);
		return true;
	}
};

