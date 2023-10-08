#include "Program.h"

#include <cstdlib> 
#include <iostream>
#include <iostream>
#include <fstream>
#include <random>

using namespace std;

MainProgram::MainProgram()
{
    cout << "Program created" << endl;
}


void MainProgram::run(double n_piece, double dt, double alpha_r, double aplpha_0, string file_route) {
    cout << "Running program..." << endl;
    cout << "Number of pieces: " << n_piece << endl;
    cout << "Time step: " << dt << endl;

    int height = 60;
    int width = 40;

    Board *board = new Board(height, width, alpha_r);

    // Add obstacles
    int nx_obstacles = 8;
    int ny_obstacles = 8;
    int offset = 10;
    double radius = 1;
    int par = 0;
    for (int i = 0; i < nx_obstacles; i++) {
        for (int j = 0; j < ny_obstacles; j++) {
            if (par % 2 == 0) {
                if (i == nx_obstacles - 1) {
                    par++;
                    continue;
                }
                double x = (i + 1) * ((width+radius) / (nx_obstacles)) - radius ;
                double y = (j + 1) * ((height-offset) / (ny_obstacles + 1)) + offset;

                Obstacle *obstacle = new Obstacle(radius, x, y, aplpha_0);
                board->addObstacle(obstacle);
            }
            else {
                double x = (i + 1) * ((width+radius) / (nx_obstacles)) - (width / (nx_obstacles * 2)) - radius;
                double y = (j + 1) * ((height-offset) / (ny_obstacles + 1)) + offset;

                Obstacle *obstacle = new Obstacle(radius, x, y, aplpha_0);
                board->addObstacle(obstacle);
            }
            par++;
        }
    }

// Add channels
    double radius_walls = 1;
    int nx_walls = nx_obstacles - 1;
    int ny_walls = offset/radius_walls + 1;
    for (int i = 0; i < nx_walls; i++) {
        double x =  (i + 1) * ((width+radius) / (nx_obstacles)) + radius*0.15;

        for (int j = 0; j < ny_walls; j++) {
                double y =  ((height-offset) / (ny_obstacles + 1)) + offset - (j+4)*radius;

                Obstacle *obstacle = new Obstacle(radius_walls, x, y, aplpha_0);
                board->addObstacle(obstacle);

        }
    }




    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist((width / 2) - 2, (width / 2) + 2);

    // Add pieces
    vector<Piece*> pieces;
    for (int i = 0; i < n_piece; i++) {
        double x = dist(gen);
        double y = height;
        double radius = 0.8;

        Piece *piece = new Piece(radius, x, y);
        pieces.push_back(piece);
    }

    // Set initial velocity
    for (int i = 0; i < n_piece; i++) {
        pieces[i]->setVelocity(0, 0);
    }

    // Run simulation
    bool running = true;
    double max_time = 60 * dt * 30;
    double time = 0;
    while (running) {
        // Save position of pieces
        for (int i = 0; i < n_piece; i++) {
            double x = pieces[i]->getX();
            double y = pieces[i]->getY();

            pieces[i]->savePath(x, y);
        }

        // Save position of obstacles
        board->saveObstaclesPath();

        // Update pieces with gravity
        for (int i = 0; i < n_piece; i++) {
            pieces[i]->update(dt);
        }

        // Check for collision with the floor
        for (int i = 0; i < n_piece; i++) {
            if (pieces[i]->collidesWithFloor(0)) {
                pieces[i]->calculateCollisionWithFloor(0, board->getDamperConstant());
            }
        }

        // Check for collisions with walls
        for (int i = 0; i < n_piece; i++) {
            if (pieces[i]->collidesWithWall(width)) {
                pieces[i]->calculateCollisionWithWall(width, board->getDamperConstant());
            }
        }

        // Check for collisions with obstacles
        for (int i = 0; i < n_piece; i++) {
            board->collidesWithObstacle(pieces[i]);
        }

        time += dt;
        cout << "Time: " << time << endl;
        if (time >= max_time) {
            running = false;
        }
    }

    vector<pair<double, double>> borders = vector<pair<double, double>>();
    for (double border_division = 0; border_division < height; border_division += (0.5 * 2)) {
        borders.push_back(pair<double, double>(0, border_division));
        borders.push_back(pair<double, double>(width, border_division));
    }
    for (double border_division = 0; border_division < width; border_division += (0.5 * 2)) {
        borders.push_back(pair<double, double>(border_division, 0));
    }


    // Print results
    ofstream file;
    file.open(file_route);

    int path_length = static_cast<int>(pieces[0]->getPath().size());
    vector<Obstacle*> obstacles = board->getObstacles();
    for (int t = 0; t < path_length; t++) {

        file << n_piece + static_cast<int>(obstacles.size()) + static_cast<int>(borders.size()) << endl;
        file << "aver" << endl;
                
        // Particulas
        for (int i = 0; i < n_piece; i++) {
            pair<double, double> point = pieces[i]->getPathPoint(t);
            file << i + 1 << "\t" << 1 << "\t" << point.first << "\t" << point.second << "\t" << pieces[i]->getRadius() << endl;
        }

        // Obstaculo
        for (int i = 0; i < static_cast<int>(obstacles.size()); i++) {
            pair<double, double> point = obstacles[i]->getPathPoint(t);
            file << n_piece + i + 1 << "\t" << 2 << "\t" << point.first << "\t" << point.second << "\t" << obstacles[i]->getRadius() << endl;
        }

        // Paredes
        for (int i = 0; i < static_cast<int>(borders.size()); i++) {
            file << n_piece + static_cast<int>(obstacles.size()) + i + 1 << "\t" << 3 << "\t" << borders[i].first << "\t" << borders[i].second << "\t" << 0.5 << endl;
        }
    }

    file.close();    

// Histogram
//Crear una lista de numero de particulas y otra con las posiciones finales en x
    vector<double> x_final;
    vector<int> n_particles;
    for (int i = 0; i < n_piece; i++) {
        x_final.push_back(pieces[i]->getX());
    }
    //Contar cuantas particulas hay en cada posicion final
    for (int i = 0; i < static_cast<int>(x_final.size()); i++) {
        int n = 0;
        for (int j = 0; j < static_cast<int>(x_final.size()); j++) {
            if (abs(x_final[i] - x_final[j]) <= radius) {
                n++;
            }
        }
        n_particles.push_back(n);
    }

    //Crear un archivo con los datos de las listas
    ofstream file2;
    file2.open("./data/histogram.dat");
    for (int i = 0; i < static_cast<int>(n_particles.size()); i++) {
        file2 << x_final[i] << "\t" << n_particles[i] << endl;
    }
    file2.close();



}

