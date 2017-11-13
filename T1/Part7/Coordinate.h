#ifndef COORDINATE
#define  COORDINATE



class Coordinate {

private: 
	double x;
	double y;
	double z;
	double w;

 public:
 	Coordinate(double _x, double _y);
 	Coordinate(double _x, double _y, double _z);
 	Coordinate();
 	double getX();
 	double getY();
 	double getZ();
 	void setX(double xToSet);
 	void setY(double yToSet);
 	void setZ(double zToSet);
 	Coordinate* transformCoordinate(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
};

#endif