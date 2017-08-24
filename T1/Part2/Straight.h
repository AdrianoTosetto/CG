#ifndef STRAIGHT_H
#define STRAIGHT_H
#include "Straight.h"
#include "Coordinate.h"
#include "Object.h"

class Straight : public Object{
private:
	Coordinate a;
	Coordinate b;
public:
	Straight(Coordinate a, Coordinate b);
	Straight(Coordinate a, Coordinate b, int id, std::string name);
	~Straight();
	Coordinate getA();
	Coordinate getB();
	void setA(Coordinate newA);
	void setB(Coordinate newB);
	Straight* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
};

#endif