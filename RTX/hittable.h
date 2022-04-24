#pragma once

#include "ray.h"
#include "aabb.h"

class material;

struct hit_record {
	point3 p;
	vec3 normal;
	double t; // Time of hit
	double u, v; // Surface coord of hit
	bool front_face;
	std::shared_ptr<material> material_ptr;

	inline void set_face_normal(const ray& r, const vec3& outward_normal) {
		front_face = vector_dot(r.direction(), outward_normal) < 0;
		normal = front_face ? outward_normal : -outward_normal;
	}
};

class hittable {
public:
	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const = 0;
	virtual bool bounding_box(double t0, double t1, aabb& output_box) const = 0;
};

class translate : public hittable {
public:
	std::shared_ptr<hittable> ptr;
	vec3 offset;

public:
	translate(std::shared_ptr<hittable> obj, const vec3& _offset) : ptr(obj), offset(_offset) {}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		ray r2(r.origin() - offset, r.direction());
		if (!ptr->hit(r2, t_min, t_max, rec))
			return false;

		rec.p += offset;
		rec.set_face_normal(r2, rec.normal);

		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		if (!ptr->bounding_box(t0, t1, output_box))
			return false;

		output_box = aabb(output_box.min + offset, output_box.max + offset);

		return true;
	}

};

class rotate_y : public hittable {
public:
	std::shared_ptr<hittable> ptr;
	double sin_theta;
	double cos_theta;
	bool has_box;
	aabb bbox;

public:
	rotate_y(std::shared_ptr<hittable> p, double angle) : ptr(p) {
		auto theta = deg_to_rad(angle);
		sin_theta = sin(theta);
		cos_theta = cos(theta);

		has_box = ptr->bounding_box(0, 1, bbox);

		point3 min(inf, inf, inf);
		point3 max(-inf, -inf, -inf);

		for (int i = 0; i < 2; i++) {
			for (int j = 0; j < 2; j++) {
				for (int k = 0; k < 2; k++) {
					auto x = i * bbox.max.x() + (1 - i) * bbox.min.x();
					auto y = j * bbox.max.y() + (1 - j) * bbox.min.y();
					auto z = k * bbox.max.z() + (1 - k) * bbox.min.z();

					auto newx = cos_theta * x + sin_theta * z;
					auto newz = -sin_theta * x + cos_theta * z;

					vec3 test(newx, y, newz);
				
					for (int c = 0; c < 3; c++) {
						min[c] = fmin(min[c], test[c]);
						max[c] = fmax(max[c], test[c]);
					}
				}
			}
		}

		bbox = aabb(min, max);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		auto origin = r.origin();
		auto direction = r.direction();

		origin[0] = cos_theta * r.origin()[0] - sin_theta * r.origin()[2];
		origin[2] = sin_theta * r.origin()[0] + cos_theta * r.origin()[2];

		direction[0] = cos_theta * r.direction()[0] - sin_theta * r.direction()[2];
		direction[2] = sin_theta * r.direction()[0] + cos_theta * r.direction()[2];

		ray r_rotated(origin, direction);

		if (!ptr->hit(r_rotated, t_min, t_max, rec))
			return false;

		auto p = rec.p;
		auto normal = rec.normal;

		p[0] = cos_theta * rec.p[0] + sin_theta * rec.p[2];
		p[2] = -sin_theta * rec.p[0] + cos_theta * rec.p[2];

		normal[0] = cos_theta * rec.normal[0] + sin_theta * rec.normal[2];
		normal[2] = -sin_theta * rec.normal[0] + cos_theta * rec.normal[2];

		rec.p = p;
		rec.set_face_normal(r_rotated, normal);

		return true;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = bbox;
		return has_box;
	}
};
