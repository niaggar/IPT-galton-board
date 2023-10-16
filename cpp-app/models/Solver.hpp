#pragma once


#include <vector>
#include "RigidEsphere.hpp"
#include "Vector.hpp"
#include "../utils/Math.hpp"

using namespace std;

class Solver
{
	public:
		Solver () {}
		Solver(float damping, float time_step, vector<int> final_count, bool have_collision) {
			this->damping = damping;
			this->time_step = time_step;
			this->time_substep = time_step / 8.0f;
			this->constraint = Vect2(0.0f, 0.0f);
			this->gravity = Vect2(0.0f, 0.0f);
			this->final_count = final_count;
			this->all_espheres_counted = false;
			this->n_espheres_counted = 0;
			this->have_collision = have_collision;
		}
		~Solver() {}

		void add_esphere(float radius, float damping, float mass, bool is_obstacle, Vect2 position, Vect2 velocity) {
			auto esphere = RigidEsphere(radius, damping, mass, is_obstacle);
			esphere.set_position(position);
			esphere.set_velocity(velocity);

			if (is_obstacle) {
				obstacles.push_back(esphere);
			}
			else {
				espheres.push_back(esphere);
			}
		}

		void add_obstacles(RigidEsphere obstacle) {
			obstacles.push_back(obstacle);
		}

		bool update() {
			update_objects();
			
			for (int i_current_substep = 0; i_current_substep < 8; i_current_substep++) {
				apply_gravity();
				check_collisions();
				apply_constraint();
			}

			int total_espheres = static_cast<int>(espheres.size());
			if (n_espheres_counted == total_espheres) {
				all_espheres_counted = true;
				return false;
			}

			return true;
		}

		void apply_gravity() {
			for (auto& esphere : espheres) {
				if (!esphere.get_is_active()) {
					continue;
				}

				esphere.update_position(time_substep, gravity);
			}
		}

		void check_collisions() {
			int n_espheres = static_cast<int>(espheres.size());
			int n_obstacles = static_cast<int>(obstacles.size());

			for (int i_esphere = 0; i_esphere < n_espheres; i_esphere++) {
				RigidEsphere& esphere1 = espheres[i_esphere];

				if (!esphere1.get_is_active()) {
					continue;
				}

				Vect2 position1 = esphere1.get_position();
				Vect2 velocity1 = esphere1.get_velocity();
				float radius1 = esphere1.get_radius();

				// Collision with obstacles
				for (int i_obstacle = 0; i_obstacle < n_obstacles; i_obstacle++)
				{
					RigidEsphere& obstacle = obstacles[i_obstacle];
					Vect2 position_o = obstacle.get_position();
					Vect2 velocity_o = obstacle.get_velocity();
					float radius_o = obstacle.get_radius();

					float distance = Math::distance(position1, position_o);
					float sum_radius = radius1 + radius_o;

					if (distance <= sum_radius) {
						float dx = position1.x - position_o.x;
						float dy = position1.y - position_o.y;
						float impact_angle = atan2(dy, dx);

						float vxa = velocity1.x;
						float vya = velocity1.y;
						float aplha_0 = obstacle.get_damping();

						float v_tangen = -vxa * sin(impact_angle) + vya * cos(impact_angle);
						float v_radial = -aplha_0 * (vxa * cos(impact_angle) + vya * sin(impact_angle));

						float vx_new = v_radial * cos(impact_angle) - v_tangen * sin(impact_angle);
						float vy_new = v_radial * sin(impact_angle) + v_tangen * cos(impact_angle);

						float new_x = (sum_radius) * cos(impact_angle) + position_o.x;
						float new_y = (sum_radius) * sin(impact_angle) + position_o.y;

						esphere1.set_position(Vect2(new_x, new_y));
						esphere1.set_velocity(Vect2(vx_new, vy_new));
					}
				}

				// Collision with other espheres
				if (have_collision) {
					for (int i_esphere2 = i_esphere + 1; i_esphere2 < n_espheres; i_esphere2++) {
						RigidEsphere& esphere2 = espheres[i_esphere2];
						Vect2 position2 = esphere2.get_position();
						Vect2 velocity2 = esphere2.get_velocity();
						float radius2 = esphere2.get_radius();

						float distance = Math::distance(position1, position2);
						float sum_radius = radius1 + radius2;

						if (distance <= sum_radius) {
							Vect2 normal = Math::normalize(position1 - position2);
							Vect2 tangent = Vect2(-normal.y, normal.x);

							float v1n = Math::dot(normal, velocity1);
							float v1t = Math::dot(tangent, velocity1);

							float v2n = Math::dot(normal, velocity2);
							float v2t = Math::dot(tangent, velocity2);

							float mass_1 = esphere1.get_mass();
							float mass_2 = esphere2.get_mass();
							float total_mass = mass_1 + mass_2;

							// Conservation of momentum
							float v1n_new = (v1n * (mass_1 - mass_2) + 2 * mass_2 * v2n) / total_mass;
							float v2n_new = (v2n * (mass_2 - mass_1) + 2 * mass_1 * v1n) / total_mass;

							Vect2 v2n_new_vect = normal * v2n_new;
							Vect2 v1n_new_vect = normal * v1n_new;

							Vect2 v1t_new_vect = tangent * v1t;
							Vect2 v2t_new_vect = tangent * v2t;

							Vect2 v1_new = (v1n_new_vect + v1t_new_vect) * esphere2.get_damping();
							Vect2 v2_new = (v2n_new_vect + v2t_new_vect) * esphere1.get_damping();

							esphere1.set_velocity(v1_new);
							esphere2.set_velocity(v2_new);

							// Position correction
							const float mass_ratio_1 = radius1 / sum_radius;
							const float mass_ratio_2 = radius2 / sum_radius;
							const float delta = 0.5f * (distance - sum_radius);

							Vect2 p1 = position1 - normal * (mass_ratio_2 * delta);
							Vect2 p2 = position2 + normal * (mass_ratio_1 * delta);

							esphere1.set_position(p1);
							esphere2.set_position(p2);
						}
					}
				}
			}
		}

		void apply_constraint() {
			for (auto& esphere : espheres) {
				if (!esphere.get_is_active()) {
					continue;
				}

				Vect2 position = esphere.get_position();
				Vect2 velocity = esphere.get_velocity();
				float radius = esphere.get_radius();

				// Constraint with the walls
				Vect2 temp_velocity = velocity;
				if (position.x < radius) {
					position.x = radius;
					temp_velocity.x = -velocity.x * this->damping;
					temp_velocity.y = velocity.y * this->damping;

				}
				else if (position.x > constraint.x - radius) {
					position.x = constraint.x - radius;
					temp_velocity.x = -velocity.x * this->damping;
					temp_velocity.y = velocity.y * this->damping;
				}

				// Constraint with the floor
				if (position.y < radius) {
					position.y = radius;
					temp_velocity.y = -velocity.y * this->damping;
					temp_velocity.x = velocity.x * this->damping;

					// Check if the esphere is in the final count zone
					int n_cols = static_cast<int>(final_count.size());
					float canal_width = constraint.x / (n_cols);
					int col = static_cast<int>(position.x / canal_width);

					if (col < n_cols) {
						bool is_active = esphere.get_is_active();
						if (is_active) {
							final_count[col]++;
							n_espheres_counted++;
							esphere.set_is_active(false);
						}
					}
				}
				else if (position.y > constraint.y - radius) {
					position.y = constraint.y - radius;
					temp_velocity.y = -velocity.y * this->damping;
					temp_velocity.x = velocity.x * this->damping;
				}

				esphere.set_position(position);
				esphere.set_velocity(temp_velocity);
			}
		}

		void update_objects() {
			for (auto& esphere : espheres) {
				esphere.add_to_historic_path();
			}

			for (auto& obstacle : obstacles) {
				obstacle.add_to_historic_path();
			}
		}

		void set_constraint(Vect2 constraint) {
			this->constraint = constraint;
		}

		void set_gravity(Vect2 gravity) {
			this->gravity = gravity;
		}

		vector<RigidEsphere> get_espheres() {
			return espheres;
		}

		vector<RigidEsphere> get_obstacles() {
			return obstacles;
		}

		Vect2 get_constraint() {
			return constraint;
		}

		vector<int> get_final_count() {
			return final_count;
		}

	private:
		vector<RigidEsphere> espheres;
		vector<RigidEsphere> obstacles;
		vector<int> final_count;
		Vect2 constraint;
		Vect2 gravity;
		float damping;
		float time_step;
		float time_substep;
		bool all_espheres_counted;
		bool have_collision;
		int n_espheres_counted;
};