#ifndef BOARD_H
#define BOARD_H

#include <vector>
#include "../obstacle/Obstacle.h"
#include "../piece/Piece.h"

using namespace std;

/// @brief Clase que representa el metodo de Runge-Kutta de orden 4
class Board
{
    private:
        /// @brief Longitud del tablero
        int height;
        /// @brief Ancho del tablero
        int width;
        /// @brief Lista de obstaculos
        vector<Obstacle*> obstacles;
        /// @brief Constante de amortiguamiento
        double aplpha_r;
    public:
        Board();
        Board(int height, int width, double aplpha_r);

        void addObstacle(Obstacle *obstacle);
        vector<Obstacle*> getObstacles();

        double getDamperConstant();

        void collidesWithObstacle(Piece *piece);

        void saveObstaclesPath();
};

#endif