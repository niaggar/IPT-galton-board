#pragma once

#include <random>
#include <sstream>

using namespace std;


struct Random
{
	static int get_random(int min, int max) {
		random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<int> dist_x(min, max);

		return dist_x(gen);
	}

	static float get_random(float min, float max) {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> dist_x(min, max);

		return dist_x(gen);
	}

	static float get_random_on_point(float middle, float delta) {
		random_device rd;
		mt19937 gen(rd());
		uniform_real_distribution<float> dist_x(middle - (delta / 2.0f), middle + (delta / 2.0f));

		return dist_x(gen);
	}	
};