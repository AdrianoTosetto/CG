#ifndef STRAIGHT_H
#define STRAIGHT_H
#include "Straight.h"
#include "Point2D.h"
#include "Object.h"

class Straight : public Object{
private:
	Point2D a;
	Point2D b;
public:
	Straight(Point2D a, Point2D b);
	Straight(Point2D a, Point2D b, int id, std::string name);
	~Straight();
	Point2D getA();
	Point2D getB();
	Straight* transformToViewport(int xwmin, int xwmax, int xvpmin, int xvpmax,
			      	 	     	  int ywmin, int ywmax, int yvpmin, int yvpmax);
};

#endif