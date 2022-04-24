#pragma once

#include "hittable.h"
#include "texture.h"
#include "material.h"

class constant_medium : public hittable {
public:
	std::shared_ptr<hittable> shape;
	std::shared_ptr<material> phase_fn;
	double neg_inv_density;

public:
	constant_medium(std::shared_ptr<hittable> obj, double density, std::shared_ptr<texture> texture)
		: shape(obj), neg_inv_density(-1.0 / density), phase_fn(std::make_shared<isotropic>(texture)) {}
	
	constant_medium(std::shared_ptr<hittable> obj, double density, color c)
		: shape(obj), neg_inv_density(-1.0 / density), phase_fn(std::make_shared<isotropic>(c)) {}


	// TODO assumes once a ray is out of the fog, it will not return. Will not work with shapes that contain voids
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		hit_record rec1, rec2;

		if (!shape->hit(r, -inf, inf, rec1))
			return false;

		if (!shape->hit(r, rec1.t + 0.001, inf, rec2))
			return false;

		if (rec1.t < t_min)
			rec1.t = t_min;
		if (rec2.t > t_max)
			rec2.t = t_max;

		if (rec1.t >= rec2.t)
			return false;

		if (rec1.t < 0)
			rec1.t = 0;

		const auto ray_length = r.direction().length();
		const auto distance_inside_boundary = (rec2.t - rec1.t) * ray_length;
		const auto hit_distance = neg_inv_density * log(random_double());

		if (hit_distance > distance_inside_boundary)
			return false;

		rec.t = rec1.t + hit_distance / ray_length;
		rec.p = r.at(rec.t);
		rec.material_ptr = phase_fn;

		// Next two are arbitary
		rec.normal = vec3(1, 0, 0);
		rec.front_face = true;

		return true;
	}


	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		return shape->bounding_box(t0, t1, output_box);
	}
};


