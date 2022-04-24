#pragma once

// Bounding Volume Hierarchy

#include "hittable.h"
#include "hittable_list.h"
#include "utility.h"
#include <iostream>

inline bool box_compare(
	const std::shared_ptr<hittable> a,
	const std::shared_ptr<hittable> b,
	int axis
) {
	aabb box_a;
	aabb box_b;

	if (!a->bounding_box(0, 0, box_a) || !b->bounding_box(0, 0, box_b))
		std::cerr << "No bounding box in bvh_node constructor.\n";

	return box_a.min[axis] < box_b.min[axis];
}

bool box_x_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 0);
}

bool box_y_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 1);
}

bool box_z_compare(const std::shared_ptr<hittable> a, const std::shared_ptr<hittable> b) {
	return box_compare(a, b, 2);
}

class bvh_node : public hittable {
public:
	std::shared_ptr<hittable> left;
	std::shared_ptr<hittable> right;
	aabb box;

public:
	bvh_node(const hittable_list& list, double t0, double t1) 
		: bvh_node(list.objects, 0, list.objects.size(), t0, t1) {}

	bvh_node(const std::vector<std::shared_ptr<hittable>>& objs,
		size_t start, size_t end,
		double t0, double t1
	) {
		auto objs_modifiable = objs;
		int axis = random_int(0, 2);
		auto comparator = (axis == 0) ? box_x_compare :
			(axis == 1) ? box_y_compare : box_z_compare;

		size_t obj_span = end - start;

		if (obj_span == 1) {
			left = right = objs_modifiable[start];
		} else if (obj_span == 2) {
			if (comparator(objs_modifiable[start], objs_modifiable[start + 1])) {
				left = objs_modifiable[start];
				right = objs_modifiable[start + 1];
			} else {
				left = objs_modifiable[start + 1];
				right = objs_modifiable[start];
			}
		} else {
			std::sort(objs_modifiable.begin() + start, objs_modifiable.begin() + end, comparator);

			auto mid = start + obj_span;
			left = std::make_shared<bvh_node>(objs_modifiable, start, mid, t0, t1);
			right = std::make_shared<bvh_node>(objs_modifiable, mid, end, t0, t1);
		}

		aabb box_left, box_right;

		if (!left->bounding_box(t0, t1, box_left) || !right->bounding_box(t0, t1, box_right))
			std::cerr << "No bounding box in bvh_node constructor.\n";

		box = surrounding_box(box_left, box_right);
	}

	virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override {
		if (!box.hit(r, t_min, t_max))
			return false;

		bool hit_left = left->hit(r, t_min, t_max, rec);
		bool hit_right = right->hit(r, t_min, hit_left ? rec.t : t_max, rec);

		return hit_left || hit_right;
	}

	virtual bool bounding_box(double t0, double t1, aabb& output_box) const override {
		output_box = box;
		return true;
	}
};

