#pragma once
#include "vec3.h"
#include "ray.h"
#include <memory>

// Axis-Aligned Bounding Box

class aabb {
public:
	point3 min;
	point3 max;

public:
	aabb() {}
	aabb(const point3& a, const point3& b) : min(a), max(b) {}

	// Credit to Andrew Kensler for improved hit
	inline bool hit(const ray& r, double t_min, double t_max) const {
		for (int a = 0; a < 3; a++) {
			auto invD = 1.0f / r.direction()[a];
			auto t0 = (min[a] - r.origin()[a]) * invD;
			auto t1 = (max[a] - r.origin()[a]) * invD;
			if (invD < 0.0f)
				std::swap(t0, t1);
			t_min = t0 > t_min ? t0 : t_min;
			t_max = t1 < t_max ? t1 : t_max;
			if (t_max <= t_min)
				return false;
		}
		return true;
	}
};

aabb surrounding_box(const aabb& box0, const aabb& box1) {
	point3 small(fmin(box0.min[0], box1.min[0]),
		fmin(box0.min[1], box1.min[1]),
		fmin(box0.min[2], box1.min[2])
	);
	
	point3 big(fmax(box0.max[0], box1.max[0]),
		fmax(box0.max[1], box1.max[1]),
		fmax(box0.max[2], box1.max[2])
	);

	return aabb(small, big);
}

