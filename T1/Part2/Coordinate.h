#ifndef COORDINATE
#define  COORDINATE

class Coordinate {

private: 
	double x;
	double y;
	double z;

 public:
 	Coordinate(double _x, double _y);
 	double getX();
 	double getY();
 	void setX(double xToSet);
 	void setY(double yToSet);
 	Coordinate* transformCoordinate(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
};

#endif