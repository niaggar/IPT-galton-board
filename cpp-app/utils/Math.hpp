#pragma once

#include <vector>
#include <utility>
#include <cmath>
#include "../models/Vector.hpp"

using namespace std;


struct Math
{
	static float distance(Vect2 v1, Vect2 v2) {
		return sqrt(pow(v1.x - v2.x, 2) + pow(v1.y - v2.y, 2));
	}

	static float norm(Vect2 v) {
		return sqrt(pow(v.x, 2) + pow(v.y, 2));
	}

	static float dot(Vect2 v1, Vect2 v2) {
		return v1.x * v2.x + v1.y * v2.y;
	}

	static float cross(Vect2 v1, Vect2 v2) {
		return v1.x * v2.y - v1.y * v2.x;
	}

	static Vect2 normalize(Vect2 v) {
		float norm = Math::norm(v);

		return Vect2(v.x / norm, v.y / norm);
	}
};