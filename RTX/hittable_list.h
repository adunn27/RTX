#pragma once

#include "hittable.h"

#include <memory>
#include <vector>

class hittable_list : public hittable {
public:
	std::vector<std::shared_ptr<hittable>> objects;

public:
	hittable_list() {};
	hittable_list(std::shared_ptr<hittable> object) { add(object); }

	void clear() { objects.clear(); }
	void add(std::shared_ptr<hittable> object) { objects.push_back(object); }

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		hit_record temp_rec;
		bool hit_anything = false;
		auto closest_so_far = t_max;

		for (const auto& object : objects) {
			if (object->hit(r, t_min, closest_so_far, temp_rec)) {
				hit_anything = true;
				closest_so_far = temp_rec.t;
				rec = temp_rec;
			}
		}

		return hit_anything;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const {
		if (objects.empty())
			return false;

		aabb temp_box;
		bool first_box = true;

		for (const auto& obj : objects) {
			if (!obj->bounding_box(t0, t1, temp_box))
				return false;
			output_box = first_box ? temp_box : surrounding_box(output_box, temp_box);
			first_box = false;
		}

		return true;
	}
};
