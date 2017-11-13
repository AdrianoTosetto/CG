namespace algebra {

#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <math.h>

class Vector {
 private:
 	double a;
 	double b;
 	double c;
 public:
 	Vector(double _a, double _b): a(_a), b(_b) {
 		this->c = 0;
	}
 	Vector(double _a, double _b, double _c): a(_a), b(_b), c(_c) {}
 	double getA() const {
 		return this->a;
 	}
 	double getB() const {
 		return this->b;
 	}
  	double getC() const{
 		return this->c;
 	}
 	void setA(double _a){
 		this->a = _a;
 	}
 	void setB(double _b){
 		this->b = _b;
 	}
 	void setC(double _c) {
 		this->c = _c;
 	}
 	Vector& operator+(const Vector& rhs){
		Vector v(this->a + rhs.a, this->b + rhs.b + this->c + rhs.c);
		return *this = v;
	}
    Vector& operator-(const Vector& rhs) {
        Vector v(this->a - rhs.a, this->b - rhs.b, this->c - rhs.c);
        return *this = v;
    }
	double operator*(Vector v) const{
		return (this->a * v.getA()) + (this->b * v.getB()) + (this->c * v.getC());
	}
	Vector& operator*(double k) {
		Vector v(this->a * k, this->b * k, this->c * k);
		return *this = v;
	}
	double getNorm() const{
		double prod = (*this) * (*this);
		return sqrt(prod);
	}
	double operator^(const Vector& rhs) const {
		//return acos(((*this) * (rhs)) / ((this->getNorm()) * (rhs.getNorm())));
		return acos((*this) * (rhs) / this->getNorm() * rhs.getNorm());
	}

};

#endif

}
