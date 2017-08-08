#ifndef POINT2D_H
#define POINT2D_H
#include <string>
#include "Object.h"

class Point2D : public Object{
 private:
 	int x;
 	int y;

 public:
 	Point2D(int x, int y);
 	Point2D(int x, int y, int id, std::string name);
 	int getX();
 	int getY();
 	Point2D* transformToViewport(int xwmin, int xwmax, int xvpmin, int xvpmax,
						   	     int ywmin, int ywmax, int yvpmin, int yvpmax);
};


#endif