#ifndef COORDINATE
#define  COORDINATE

class Coordinate {

private: 
	double x;
	double y;

 public:
 	Coordinate(double _x, double _y);
 	double getX();
 	double getY();
 	Coordinate* transformCoordinate(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
};

#endif