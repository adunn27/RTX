#pragma once

#include "hittable.h"
#include "vec3.h"

class sphere : public hittable {
public:
	point3 center;
	double radius;
	std::shared_ptr<material> material_ptr;

public:
	sphere(point3 cen, double r, std::shared_ptr<material> m) : center(cen), radius(r), material_ptr(m) {};

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		vec3 oc = r.origin() - center;
		auto a = r.direction().length_squared();
		auto half_b = vector_dot(oc, r.direction());
		auto c = oc.length_squared() - radius * radius;

		auto discriminant = half_b * half_b - a * c;
		if (discriminant < 0)
			return false;
	
		// Find nearest root that is within range
		auto sqrtd = sqrt(discriminant);
		auto root = (-half_b - sqrtd) / a;
		if (root < t_min || root > t_max) {
			root = (-half_b + sqrtd) / a;
			if (root < t_min || root > t_max)
				return false;
		}

		rec.t = root;
		rec.p = r.at(rec.t);
		vec3 outward_normal = (rec.p - center) / radius;
		rec.set_face_normal(r, outward_normal);
		rec.material_ptr = material_ptr;

		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = aabb(
			center - vec3(radius, radius, radius),
			center + vec3(radius, radius, radius)
		);
		return true;
	}
};
