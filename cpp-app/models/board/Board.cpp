#include "Board.h"

#include <vector>

Board::Board() {}

Board::Board(int width, int height, double aplpha_r)
{
    this->width = width;
    this->height = height;
    this->aplpha_r = aplpha_r;
    this->obstacles = vector<Obstacle*>();
}

void Board::addObstacle(Obstacle *obstacle)
{
    obstacles.push_back(obstacle);
}

double Board::getDamperConstant()
{
    return aplpha_r;
}

void Board::collidesWithObstacle(Piece *piece)
{
    for (int i = 0; i < static_cast<int>(obstacles.size()); i++)
    {
        if (obstacles[i]->collidesWith(piece))
        {
            obstacles[i]->calculateCollision(piece);
        }
    }
}

vector<Obstacle*> Board::getObstacles()
{
    return obstacles;
}

void Board::saveObstaclesPath()
{
    for (int i = 0; i < static_cast<int>(obstacles.size()); i++)
    {
        double x = obstacles[i]->getX();
        double y = obstacles[i]->getY();

        obstacles[i]->savePath(x, y);
    }
}
