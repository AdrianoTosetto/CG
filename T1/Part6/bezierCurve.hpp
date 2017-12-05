#ifndef CURVE_HPP
#define CURVE_HPP
#include "Coordinate.h"
#include "Object.h"
#include <vector>

/*
	gera matriz base da curva de bezier
*/


inline Matrix initMb() {
	Matrix Mb(4,4);

	Mb.setValue(0,0, -1.0);
	Mb.setValue(0,1, 3.0);
	Mb.setValue(0,2, -3.0);
	Mb.setValue(0,3, 1.0);

	Mb.setValue(1,0, 3.0);
	Mb.setValue(1,1, -6.0);
	Mb.setValue(1,2, 3.0);
	Mb.setValue(1,3, 0.0);

	Mb.setValue(2,0, -3.0);
	Mb.setValue(2,1, 3.0);
	Mb.setValue(2,2, 0.0);
	Mb.setValue(2,3, 0.0);

	Mb.setValue(3,0, 1.0);
	Mb.setValue(3,1, 0.0);
	Mb.setValue(3,2, 0.0);
	Mb.setValue(3,3, 0.0);

	return Mb;
}
class BezierCurve : public Object{
 public:
 	BezierCurve(std::string name, int id, Coordinate _p1, Coordinate _p2, Coordinate _p3, Coordinate _p4) : Object(name, id),
 		p1(_p1),p2(_p2),p3(_p3),p4(_p4) {
 		this->type = TCURVE_BEZIER;
 	}
 	Coordinate getP1() {
 		return this->p1;
 	}
 	Coordinate getP2() {
 		return this->p2;
 	}
 	Coordinate getP3() {
 		return this->p3;
 	}
 	Coordinate getP4() {
 		return this->p4;
 	}
 	void setP1(Coordinate _p1) {
 		this->p1 = _p1;
 	}
 	void setP2(Coordinate _p2) {
 		this->p2 = _p2;
 	}
 	void setP3(Coordinate _p3) {
 		this->p3 = _p3;
 	}
 	void setP4(Coordinate _p4) {
 		this->p4 = _p4;
 	}

 private:
 	Coordinate p1, p2, p3, p4;
};

#endif