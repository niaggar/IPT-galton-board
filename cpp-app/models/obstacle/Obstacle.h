#ifndef OBSTACLE_H
#define OBSTACLE_H

#include "../piece/Piece.h"

using namespace std;

/// @brief Clase que representa el objete pieza que se va a dibujar en el tablero que cae
class Obstacle
{
    private:
        /// @brief Radio de la pieza
        double radius;
        /// @brief Posicion en x de la pieza
        double x;
        /// @brief Posicion en y de la pieza
        double y;
        /// @brief Constante de amortiguamiento
        double aplha_0;
        /// @brief Camino recorrido por el obstaculo
        vector<pair<double, double>> path;
    public:
        Obstacle(double radius, double x, double y, double alpha_0);

        bool collidesWith(Piece *piece);
        void calculateCollision(Piece *piece);

        void savePath(double x, double y);

        vector<pair<double, double>> getPath();
        pair<double, double> getPathPoint(int i);

        double getRadius();
        double getX();
        double getY();
};

#endif