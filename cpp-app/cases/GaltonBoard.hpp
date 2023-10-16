#pragma once


#include "../utils/Math.hpp"
#include "../utils/Random.hpp"
#include "../utils/ExportFile.hpp"
#include "../models/Vector.hpp"
#include "../models/Solver.hpp"
#include "../models/RigidEsphere.hpp"
#include <vector>

using namespace std;


struct GaltonBoard
{
public:
	Solver solver;

	Vect2 dimensions;
	Vect2 gravity;
	Vect2 pegs_constraint;
	
	float peg_radio = 1.0f;
	float slope_radio = 0.25f;
	float peg_damping = 0.38f;
	float table_damping = 0.48f;

	string route_distribution_file;
	string route_paths_file;

	float esphere_radius = 1.0f;
	float esphere_damping = 0.999f;
	float esphere_mass = 1.0f;

	int n_tests = 10;
	int n_rows = 10;
	int n_cols = 12;
	int n_espheres = 500;

	float min_velocity_x = -5.0f;
	float max_velocity_x = 5.0f;
	float max_velocity_y = -5.0f;
	float range_position = 2;

	bool save_paths = false;

	vector<vector<int>> final_distributions;

	GaltonBoard() {}

	void execute() {
		initialice_constants();
		simulate();
		export_distribution();
	}

	void initialice_constants() {
		cout << "______________________________________________________________" << endl;
		cout << "_______________________PHYSICS ENGINE________________________" << endl;
		cout << "_______________________GALTONBOARD ƪ(˘⌣˘)ʃ________________________" << endl;
		cout << "______________________________________________________________" << endl << endl;

		cout << "Enter the height of the Galton Board: (57.0) ";
		cin >> dimensions.y;
		cout << "Enter the width of the Galton Board: (55.0) ";
		cin >> dimensions.x;

		cout << endl << endl;

		cout << "Enter the radius of the espheres: (1.0) ";
		cin >> esphere_radius;
		cout << "Enter the number of espheres: (500) ";
		cin >> n_espheres;

		cout << endl << endl;

		cout << "Enter the number of tests: (10) ";
		cin >> n_tests;
		cout << "Enter the number of rows: (10) ";
		cin >> n_rows;
		cout << "Enter the number of cols: (12) ";
		cin >> n_cols;

		cout << endl << endl;

		cout << "Enter the damping of the pegs: (0.38) ";
		cin >> peg_damping;
		cout << "Enter the damping of the table: (0.48) ";
		cin >> table_damping;

		cout << endl << endl;

		cout << "Enter the min velocity in x: (-5.0) ";
		cin >> min_velocity_x;
		cout << "Enter the max velocity in x: (5.0) ";
		cin >> max_velocity_x;
		cout << "Enter the max velocity in y: (-5.0) ";
		cin >> max_velocity_y;

		cout << endl << endl;

		cout << "Enter the range of the position of the espheres: (2.0) ";
		cin >> range_position;

		cout << endl << endl;

		cout << "Enter the route of the file: (distribution) ";
		cin >> route_distribution_file;
		cout << "Do you want to save the paths of the espheres? (0/1) ";
		cin >> save_paths;
		if (save_paths) {
			cout << "Enter the route of the file: (paths) ";
			cin >> route_paths_file;
		}

		cout << endl << endl;
		cout << "Running..." << endl;
	}

	Solver* simulate_test() {
		vector<int> final_count_col;
		for (int i = 0; i < n_cols + 1; i++) {
			final_count_col.push_back(0);
		}
		Solver* solver = new Solver(0.48f, 1.0f / 60.0f, final_count_col, false);

		vector<RigidEsphere> pegs_points = get_pegs_points(0.0f, 0.38f);
		solver->set_constraint(Vect2(dimensions.x, dimensions.y + 10.0f));
		solver->set_gravity(Vect2(0.0f, -9.81f));

		for (auto& peg : pegs_points) {
			solver->add_obstacles(peg);
		}

		for (int i = 0; i < n_espheres; i++) {
			float x = Random::get_random_on_point(dimensions.x / 2.0f, range_position);
			float y = dimensions.y + 10.0f;

			float vx = Random::get_random(min_velocity_x, max_velocity_x);
			float vy = Random::get_random(max_velocity_y, 0.0f);

			solver->add_esphere(esphere_radius, esphere_damping, esphere_mass, false, Vect2(x, y), Vect2(vx, vy));
		}

		for (int i_frame = 0; i_frame < 100000; i_frame++) {
			bool simulate_continue = solver->update();
			if (!simulate_continue) {
				break;
			}
		}

		return solver;
	}

	void simulate() {
		for (int i_test = 0; i_test < n_tests; i_test++) {
			cout << "Test (〜￣▽￣)〜 " << i_test << endl;

			auto* solver = simulate_test();
			auto distribution = solver->get_final_count();

			cout << "Distribution: " << endl;
			for (int i = 0; i < n_cols + 1; i++) {
				cout << distribution[i] << " ";
			}
			cout << endl;

			final_distributions.push_back(distribution);
			if (save_paths) {
				export_paths(i_test, solver);
			}

			delete solver;
		}
	}

private:
	void export_distribution() {
		ExportFile::export_final_distribution(final_distributions, route_distribution_file + ".dat");
	}

	void export_paths(int test, Solver* solver) {
		vector<RigidEsphere> bodies = solver->get_espheres();
		vector<RigidEsphere> obstacles = solver->get_obstacles();
		Vect2 constraint = solver->get_constraint();
		
        auto route = route_paths_file + "-" + std::to_string(test) + ".dat";
		ExportFile::export_paths(bodies, obstacles, constraint, route);
	}

	vector<RigidEsphere> get_pegs_points(float y_offset_general, float damping) {
		vector<RigidEsphere> pegs_points;
		float x_offset = dimensions.x / (n_cols + 1);

		for (int i = 0; i < n_rows; i++) {
			float y_offset = (dimensions.y - y_offset_general) / (n_rows + 1);

			for (int j = 0; j < n_cols + 2; j++) {
				if (i % 2 == 0) {
					auto peg = RigidEsphere(peg_radio, damping, 1.0f, true);
					peg.set_position(Vect2(x_offset * j, y_offset * (i + 1) + y_offset_general));
					pegs_points.push_back(peg);
				}
				else {
					if (j >= n_cols - 1) {
						continue;
					}
					else {
						auto peg = RigidEsphere(peg_radio, damping, 1.0f, true);
						peg.set_position(Vect2(x_offset * (j + 1) + x_offset / 2, y_offset * (i + 1) + y_offset_general));
						pegs_points.push_back(peg);
					}					
				}
			}
		}

		return pegs_points;
	}

	vector<RigidEsphere> get_obstacle_canal(float min_width, float slope, float damping) {
		vector<RigidEsphere> obstacle_canal;

		float min_height = dimensions.y;
		float width_distance = dimensions.x / 2 - min_width / 2;

		float right_initial_x = width_distance + min_width;
		float left_initial_x = width_distance;

		int number_divisions = (int) (width_distance / (slope_radio * 2));
		float division_width = slope_radio * 2;

		for (int i = 0; i < number_divisions; i++) {
			float right_x = right_initial_x + division_width * i;
			float left_x = left_initial_x - division_width * i;

			float height = min_height + slope * (right_x - right_initial_x);

			auto obstacleright = RigidEsphere(slope_radio, damping, 1.0f, true);
			obstacleright.set_position(Vect2(right_x, height));

			auto obstacleleft = RigidEsphere(slope_radio, damping, 1.0f, true);
			obstacleleft.set_position(Vect2(left_x, height));

			obstacle_canal.push_back(obstacleright);
			obstacle_canal.push_back(obstacleleft);
		}

		return obstacle_canal;
	}
};