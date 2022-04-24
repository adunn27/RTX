#pragma once

#include "hittable.h"

class rect : public hittable {
public:
	std::shared_ptr<material> mat;
	double x0, x1, y0, y1;
	double k;

public:
	rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> _mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), mat(_mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		auto t = (k - r.origin().z()) / r.direction().z();
		if (t < t_min || t > t_max)
			return false;

		auto x = r.origin().x() + t * r.direction().x();
		auto y = r.origin().y() + t * r.direction().y();
		if (x < x0 || x > x1 || y < y0 || y > y1)
			return false;

		rec.u = (x - x0) / (x1 - x0);
		rec.v = (y - y0) / (y1 - y0);
		rec.t = t;
		rec.set_face_normal(r, vec3(0, 0, 1));
		rec.material_ptr = mat;
		rec.p = r.at(t);
		
		return true;
	};

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = aabb(point3(x0, y0, k - 0.0001), point3(x1, y1, k + 0.0001));
		return true;
	};

};
