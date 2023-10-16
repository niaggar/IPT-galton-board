#pragma once 

#include <vector>
#include "Math.hpp"
#include "../models/Vector.hpp"
#include "../models/RigidEsphere.hpp"

using namespace std;


vector<Vect2> get_path(RigidEsphere esphere, float max_time, float time_step) {
	vector<Vect2> path = vector<Vect2>();

	float time = 0.0f;

	while (time < max_time) {
		path.push_back(esphere.get_position());

		esphere.update_position(time_step, Vect2(0.0f, -9.81f));

		time += time_step;
	}

	return path;
}
