#pragma once

#include <vector>
#include <utility>
#include <cmath>

using namespace std;

struct Vect2 {
	float x;
	float y;

	Vect2() {
		x = 0.0f;
		y = 0.0f;
	}

	Vect2(float x, float y) {
		this->x = x;
		this->y = y;
	}

	Vect2 operator+(const Vect2& v) const {
		return Vect2(x + v.x, y + v.y);
	}

	Vect2 operator-(const Vect2& v) const {
		return Vect2(x - v.x, y - v.y);
	}

	Vect2 operator*(const float& f) const {
		return Vect2(x * f, y * f);
	}

	Vect2 operator/(const float& f) const {
		return Vect2(x / f, y / f);
	}
};
