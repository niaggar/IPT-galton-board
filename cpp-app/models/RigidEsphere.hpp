#pragma once

#include "Vector.hpp"
#include "../utils/Math.hpp"

using namespace std;

class RigidEsphere
{
	public:
		RigidEsphere(float radius, float damping, float mass, bool is_obstacle) {
			this->radius = radius;
			this->is_obstacle = is_obstacle;
			this->damping = damping;
			this->mass = mass;

			position = Vect2(0.0f, 0.0f);
			velocity = Vect2(0.0f, 0.0f);
			historic_path = vector<Vect2>();
		}

		~RigidEsphere() {}

		void update_position(float dt, Vect2 a) {
			velocity.x += a.x * dt;
			velocity.y += a.y * dt;

			float new_x = position.x + velocity.x * dt;
			float new_y = position.y + velocity.y * dt;

			position.x = new_x;
			position.y = new_y;
		}

		void set_position(Vect2 new_p) {
			position = new_p;
		}

		void set_velocity(Vect2 new_v) {
			velocity = new_v;
		}

		void set_is_active(bool is_active) {
			this->is_active = is_active;
		}

		bool get_is_active() {
			return is_active;
		}

		float get_radius() {
			return radius;
		}

		float get_damping() {
			return damping;
		}

		float get_mass() {
			return mass;
		}

		bool get_is_obstacle() {
			return is_obstacle;
		}

		Vect2 get_position() {
			return position;
		}

		Vect2 get_velocity() {
			return velocity;
		}

		void add_to_historic_path() {
			Vect2 v = Vect2(position.x, position.y);
			historic_path.push_back(v);
		}

		vector<Vect2> get_historic_path() {
			return historic_path;
		}

		void clear_historic_path() {
			historic_path.clear();
		}

	private:
		bool is_obstacle = false;
		bool is_active = true;
		float radius;
		float damping = 1.0f;
		float mass = 1.0f;
		Vect2 position;
		Vect2 velocity;

		vector<Vect2> historic_path;
};