#ifndef POINT2D_H
#define POINT2D_H
#include <string>
#include "Object.h"
#include "Coordinate.h"

class Point2D : public Object{
 private:
 	Coordinate coord;

 public:
 	Point2D(Coordinate coord);
 	Point2D(Coordinate coord, int id, std::string name);
 	Coordinate getCoordinate();
 	Point2D* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
};


#endif