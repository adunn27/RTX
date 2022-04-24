#pragma once
#include "vec3.h"
#include "rtw_stbh_image.h"

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

class image_texture : public texture {
public:
	static const int bytes_per_pixel = 3;

private:
	unsigned char* data; // The loaded file
	int width;
	int height;
	int bytes_per_scanline;


public:
	image_texture(const char* filename) {
		auto components_per_pixel = bytes_per_pixel;
		data = stbi_load(filename, &width, &height, &components_per_pixel, components_per_pixel);

		if (!data) {
			std::cerr << "ERROR: Could not load texture file '" << filename << "'\n";
			width = 0;
			height = 0;
		}

		bytes_per_scanline = bytes_per_pixel * width;
	}

	~image_texture() {
		delete data;
	}

	virtual color value(double u, double v, const point3& p) const override {
		if (data == nullptr)
			return color(0, 1, 1); // Cyan as default color in case of error

		// Convert to image coordinates
		u = clamp(u, 0.0, 1.0);
		v = 1.0 - clamp(v, 0.0, 1.0); // Need to flip v

		auto i = static_cast<int>(u * width);
		auto j = static_cast<int>(v * height);

		const auto color_scale = 1.0 / 255.0;
		auto pixel = data + j * bytes_per_scanline + i * bytes_per_pixel;

		return color(color_scale * pixel[0], color_scale * pixel[1], color_scale * pixel[2]);
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

