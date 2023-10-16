#pragma once 

#include <vector>
#include <utility>
#include <fstream>

#include "Math.hpp"
#include "../models/Vector.hpp"
#include "../models/RigidEsphere.hpp"


using namespace std;


struct ExportFile
{
	public:
		static void export_paths(vector<RigidEsphere> bodies, vector<RigidEsphere> obstacles, Vect2 constraint, string filename) {
			ofstream file;
			file.open(filename);

			vector<Vect2> borders = ExportFile::build_constraint_wall(constraint);
			
			int n_bodies = static_cast<int>(bodies.size());
			int n_obstacles = static_cast<int>(obstacles.size());
			int n_borders = static_cast<int>(borders.size());

			vector<RigidEsphere> all_bodies = vector<RigidEsphere>();
			all_bodies.insert(all_bodies.end(), obstacles.begin(), obstacles.end());
			all_bodies.insert(all_bodies.end(), bodies.begin(), bodies.end());

			int n_max_path = ExportFile::get_max_path_lenght(all_bodies);

			for (int t = 0; t < n_max_path; t++) {
				// calculet the number of bodies in the current frame
				int n_bodies_in_frame = 0;
				for (int i_body = 0; i_body < n_bodies; i_body++) {
					int path_lenght = ExportFile::get_path_lenght(bodies[i_body]);
					int diff = n_max_path - path_lenght;
					int index = t - diff;

					if (index < 0 || index > path_lenght) {
						continue;
					}

					n_bodies_in_frame++;
				}

				// Write number of bodies
				file << n_borders + n_obstacles + n_bodies_in_frame << endl;
				file << "aver" << endl;

				// Write borders
				for (int i_border = 0; i_border < n_borders; i_border++) {
					Vect2 position1 = borders[i_border];
					file << i_border << "\t" << 1 << "\t" << position1.x << "\t" << position1.y << "\t" << 0.5f << endl;
				}

				// Write obstacles
				for (int i_obstacle = 0; i_obstacle < n_obstacles; i_obstacle++) {
					int path_lenght = ExportFile::get_path_lenght(obstacles[i_obstacle]);
					int diff = n_max_path - path_lenght;
					int index = t - diff;

					if (index < 0 || index > path_lenght) {
						continue;
					}

					Vect2 position = obstacles[i_obstacle].get_historic_path()[index];
					float radius = obstacles[i_obstacle].get_radius();

					int number = n_borders + i_obstacle + 1;
					file << number << "\t" << 2 << "\t" << position.x << "\t" << position.y << "\t" << radius << endl;
				}

				// Write bodies
				for (int i_body = 0; i_body < n_bodies; i_body++) {
					int path_lenght = ExportFile::get_path_lenght(bodies[i_body]);
					int diff = n_max_path - path_lenght;
					int index = t - diff;

					if (index < 0 || index > path_lenght) {
						continue;
					}

					Vect2 position = bodies[i_body].get_historic_path()[index];
					float radius = bodies[i_body].get_radius();

					int number = n_borders + n_obstacles + i_body + 1;
					file << number << "\t" << 3 << "\t" << position.x << "\t" << position.y << "\t" << radius << endl;
				}
			}

			file.close();
		}

		static void export_final_distribution(vector<vector<int>> distributions, string filename) {
			ofstream file;
			file.open(filename);

			int n_distributions = static_cast<int>(distributions.size());

			file << "column" << "\t" << "count" << endl;
			
			for (int i_distribution = 0; i_distribution < n_distributions; i_distribution++) {
				vector<int> distribution = distributions[i_distribution];
				int n_data = static_cast<int>(distribution.size());

				for (int i_data = 1; i_data <= n_data; i_data++) {
					file << i_data << "\t" << distribution[i_data - 1] << endl;
				}

				file << "endl" << endl;
			}

			file.close();
		}

		static vector<Vect2> build_constraint_wall(Vect2 constraint) {
			vector<Vect2> borders = vector<Vect2>();

			float width = constraint.x;
			float height = constraint.y;
			for (double border_division = 0; border_division < height; border_division += (0.5 * 2)) {
				borders.push_back(Vect2(0, border_division));
				borders.push_back(Vect2(width, border_division));
			}
			for (double border_division = 0; border_division < width; border_division += (0.5 * 2)) {
				borders.push_back(Vect2(border_division, 0));
			}

			return borders;
		}

	private:
		static int get_max_path_lenght(vector<RigidEsphere> bodies) {
			int max_path_lenght = 0;

			for (auto& bodie : bodies) {
				int path_lenght = ExportFile::get_path_lenght(bodie);

				if (path_lenght > max_path_lenght) {
					max_path_lenght = path_lenght;
				}
			}

			return max_path_lenght;
		}

		static int get_path_lenght(RigidEsphere bodie) {
			return static_cast<int>(bodie.get_historic_path().size());
		}
};