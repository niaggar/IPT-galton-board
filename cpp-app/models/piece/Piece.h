#ifndef PIECE_H
#define PIECE_H

#include <vector>
#include <utility>
#include <cmath>
#include <iostream>

using namespace std;

/// @brief Clase que representa el objete pieza que se va a dibujar en el tablero que cae
class Piece
{
    private:
        /// @brief Radio de la pieza
        double radius;
        /// @brief Posicion en x de la pieza
        double x;
        /// @brief Posicion en y de la pieza
        double y;
        /// @brief Velocidad en x de la pieza
        double vx = 0;
        /// @brief Velocidad en y de la pieza
        double vy = 0;
        /// @brief Camino recorrido por la pieza
        vector<pair<double, double>> path;
    public:
        Piece(double radius, double x, double y);

        void update(double dt);

        double getRadius();
        double getX();
        double getY();
        double getVx();
        double getVy();
        double getVelocity();
        vector<pair<double, double>> getPath();
        pair<double, double> getPathPoint(int i);

        void setVelocity(double vx, double vy);
        void setPos(double x, double y);

        bool collidesWithFloor(double floorY);
        void calculateCollisionWithFloor(double floorY, double alpha_r);

        bool collidesWithWall(double width);
        void calculateCollisionWithWall(double width, double alpha_r);

        void savePath(double x, double y);
};

#endif