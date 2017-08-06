#include <iostream>
#include "Object.h"
#include "Straight.h"

int main(int argc, char const *argv[])
{
	Point2D a(5,5);
	Point2D b(1,1);
	Straight r(a,b);
	std::cout << a.getX() << " " << a.getY() <<  "\n";
	return 0;
}