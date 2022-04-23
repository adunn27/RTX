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
