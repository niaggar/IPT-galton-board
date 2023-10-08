#include "Piece.h"

#include <iostream>
#include <cmath>

using namespace std;

Piece::Piece(double radius, double x, double y)
{
    this->radius = radius;
    this->x = x;
    this->y = y;
}

void Piece::setVelocity(double vx, double vy)
{
    this->vx = vx;
    this->vy = vy;    
}

void Piece::setPos(double x, double y)
{
    this->x = x;
    this->y = y;
}

void Piece::update(double dt)
{
    vx = vx;
    vy += -dt * 9.81;

    x += dt * vx;
    y += dt * vy;
}

bool Piece::collidesWithFloor(double floorY)
{
    return y - radius <= floorY;
}

void Piece::calculateCollisionWithFloor(double floorY, double alpha_r)
{
    y = floorY + radius;
    vy = -vy * alpha_r;
    vx = vx * alpha_r;
}


bool Piece::collidesWithWall(double width)
{
    return x - radius <= 0 || x + radius >= width;
}

void Piece::calculateCollisionWithWall(double width, double alpha_r)
{
    if (x - radius <= 0)
    {
        x = radius;
        vx = abs(vx) * alpha_r;
    }
    else
    {
        x = width - radius;
        vx = -abs(vx) * alpha_r;
    }

    vy = vy * alpha_r;
}

double Piece::getRadius()
{
    return radius;
}

double Piece::getX()
{
    return x;
}

double Piece::getY()
{
    return y;
}

double Piece::getVx()
{
    return vx;
}

double Piece::getVy()
{
    return vy;
}

double Piece::getVelocity()
{
    return sqrt(vx * vx + vy * vy);
}

vector<pair<double, double>> Piece::getPath()
{
    return path;
}

pair<double, double> Piece::getPathPoint(int i)
{
    return path[i];
}

void Piece::savePath(double x, double y)
{
    pair<double, double> point;
    point.first = x;
    point.second = y;

    path.push_back(point);
}

