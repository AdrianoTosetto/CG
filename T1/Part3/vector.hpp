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
 		this->c = 1;
	}
 	double getA() {
 		return this->a;
 	}
 	double getB() {
 		return this->b;
 	}
 	Vector& operator+(const Vector& rhs) {
		Vector v(this->a + rhs.a, this->b + rhs.b);
		return *this = v;
	}
    Vector& operator-(const Vector& rhs) {
        Vector v(this->a - rhs.a, this->b - rhs.b);
        return *this = v;
    }
	double operator*(Vector v) {
		return (this->a * v.getA()) + (this->b * v.getB());
	}
	Vector& operator*(double k) {
		Vector v(this->a * k, this->b * k);
		return *this = v;
	}
	double getNorm() {
		double prod = (*this) * (*this);
		return sqrt(prod);
	}

};

#endif

}
