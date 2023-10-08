#ifndef PROGRAM_H
#define PROGRAM_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "../models/board/Board.h"
#include "../models/piece/Piece.h"
#include "../models/obstacle/Obstacle.h"

using namespace std;

/// @brief Clase que representa el objete pieza que se va a dibujar en el tablero que cae
class MainProgram
{
    private:
        double n_piece;
        double dt;
        double t0;
        double t;
    public:
        MainProgram();

        void run(double n_piece, double dt, double alpha_r, double aplpha_0, string file_route);
};

#endif