#include "Obstacle.h"

#include <cmath>

using namespace std;

Obstacle::Obstacle(double radius, double x, double y, double alpha_0)
{
    this->radius = radius;
    this->x = x;
    this->y = y;
    this->aplha_0 = alpha_0;
}

bool Obstacle::collidesWith(Piece *piece)
{
    double x = piece->getX();
    double y = piece->getY();
    double radius = piece->getRadius();

    double distance = sqrt(pow(this->x - x, 2) + pow(this->y - y, 2));

    return distance <= this->radius + radius;
}

void Obstacle::calculateCollision(Piece *piece)
{
    double x_piece = piece->getX();
    double y_piece = piece->getY();
    double radius = piece->getRadius();

    double dx = x_piece - this->x;
    double dy = y_piece - this->y;
    double impact_angle = atan2(dy, dx);

    double vxa = piece->getVx();
    double vya = piece->getVy(); 

    double v_tangen = - vxa * sin(impact_angle) + vya * cos(impact_angle);
    double v_radial = - aplha_0 * (vxa * cos(impact_angle) + vya * sin(impact_angle));

    double vx_new = v_radial * cos(impact_angle) - v_tangen * sin(impact_angle);
    double vy_new = v_radial * sin(impact_angle) + v_tangen * cos(impact_angle);

    double new_x = (radius + this->radius) * cos(impact_angle) + this->x;
    double new_y = (radius + this->radius) * sin(impact_angle) + this->y;

    piece->setVelocity(vx_new, vy_new);
    piece->setPos(new_x, new_y);
}

vector<pair<double, double>> Obstacle::getPath()
{
    return path;
}

pair<double, double> Obstacle::getPathPoint(int i)
{
    return path[i];
}

void Obstacle::savePath(double x, double y)
{
    pair<double, double> point;
    point.first = x;
    point.second = y;

    path.push_back(point);
}

double Obstacle::getRadius()
{
    return radius;
}

double Obstacle::getX()
{
    return x;
}

double Obstacle::getY()
{
    return y;
}
