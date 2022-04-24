#pragma once

#include "hittable.h"

class xy_rect : public hittable {
public:
	std::shared_ptr<material> mat;
	double x0, x1, y0, y1;
	double k;

public:
	xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, std::shared_ptr<material> _mat)
		: x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mat(_mat) {}

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

class xz_rect : public hittable {
public:
	std::shared_ptr<material> mat;
	double x0, x1, z0, z1;
	double k;

public:
	xz_rect(double _x0, double _x1, double _z0, double _z1, double _k, std::shared_ptr<material> _mat)
		: x0(_x0), x1(_x1), z0(_z0), z1(_z1), k(_k), mat(_mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		auto t = (k - r.origin().y()) / r.direction().y();
		if (t < t_min || t > t_max)
			return false;

		auto x = r.origin().x() + t * r.direction().x();
		auto z = r.origin().z() + t * r.direction().z();
		if (x < x0 || x > x1 || z < z0 || z > z1)
			return false;

		rec.u = (x - x0) / (x1 - x0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.set_face_normal(r, vec3(0, 1, 0));
		rec.material_ptr = mat;
		rec.p = r.at(t);
		
		return true;
	};

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = aabb(point3(x0, k - 0.0001, z0), point3(x1, k + 0.0001, z1));
		return true;
	};

};

class yz_rect : public hittable {
public:
	std::shared_ptr<material> mat;
	double y0, y1, z0, z1;
	double k;

public:
	yz_rect(double _y0, double _y1, double _z0, double _z1, double _k, std::shared_ptr<material> _mat)
		: y0(_y0), y1(_y1), z0(_z0), z1(_z1), k(_k), mat(_mat) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		auto t = (k - r.origin().x()) / r.direction().x();
		if (t < t_min || t > t_max)
			return false;

		auto y = r.origin().y() + t * r.direction().y();
		auto z = r.origin().z() + t * r.direction().z();
		if (y < y0 || y > y1 || z < z0 || z > z1)
			return false;

		rec.u = (y - y0) / (y1 - y0);
		rec.v = (z - z0) / (z1 - z0);
		rec.t = t;
		rec.set_face_normal(r, vec3(1, 0, 0));
		rec.material_ptr = mat;
		rec.p = r.at(t);
		
		return true;
	};

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = aabb(point3(k - 0.0001, y0, z0), point3(k + 0.0001, y1, z1));
		return true;
	};

};
