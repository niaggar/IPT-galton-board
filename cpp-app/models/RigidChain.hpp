#pragma	once

#include "RigidEsphere.hpp"
#include <vector>

using namespace std;

class RigidChain
{
	public:
		RigidChain(float distance) {
			this->distance = distance;
		}
		~RigidChain() {}

		RigidEsphere* add_esphere(float radius, float damping, float mass, float x, float y) {
			RigidEsphere esphere(radius, damping, mass, false);

			esphere.set_position(0.0f, 0.0f);
			esphere.set_velocity(0.0f, 0.0f);
			esphere.set_position(x, y);

			espheres.push_back(esphere);
			return &espheres.back();
		}

	private:
		vector<RigidEsphere> espheres;
		float distance;

		vector<pair<float, float>> initial_ends;
};
