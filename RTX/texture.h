#pragma once
#include "vec3.h"

class texture {
public:
	virtual color value(double u, double v, const point3& p) const = 0;
};

class solid_color : public texture {
private:
	color color_val;

public:
	solid_color(double r, double g, double b) : solid_color(color(r, g, b)) {}
	solid_color(color c) : color_val(c) {}

	virtual color value(double u, double v, const vec3& p) const override {
		return color_val;
	}
};

class checker_texture : public texture {
public:
	std::shared_ptr<texture> odd;
	std::shared_ptr<texture> even;

public:
	checker_texture(color c1, color c2) :
		odd(std::make_shared<solid_color>(c1)), even(std::make_shared<solid_color>(c2)) {}

	virtual color value(double u, double v, const point3& p) const override {
		auto sine = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
		if (sine < 0)
			return odd->value(u, v, p);
		else
			return even->value(u, v, p);
	}

};

