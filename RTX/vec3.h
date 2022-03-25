#pragma once

class vec3 {
	double e[3];

public:
	vec3() : e{ 0, 0, 0 } {}
	vec3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

	double x() const { return e[0]; }
	double y() const { return e[1]; }
	double z() const { return e[2]; }
};

using color = vec3;
using point3 = vec3;
