#pragma once

#include "vec3.h"
#include "utility.h"
#include <iostream>

void write_color(std::ostream& out, color pixel, int samples_per_pix) {
	auto r = pixel.x();
	auto g = pixel.y();
	auto b = pixel.z();

	auto scale = 1.0 / samples_per_pix;
	r *= scale;
	g *= scale;
	b *= scale;

	out << static_cast<int>(256 * clamp(r, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(g, 0.0, 0.999)) << ' '
		<< static_cast<int>(256 * clamp(b, 0.0, 0.999)) << '\n';
}
