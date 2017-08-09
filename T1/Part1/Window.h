#ifndef WINDOW_H
#define WINDOW_H
#include "Coordinate.h"

class Window {

 private:
  	Coordinate origin, limit;
 public:
	Window(Coordinate _origin, Coordinate _limit);
	~Window();
	Coordinate getOrigin();
	Coordinate getLimit();	
};

#endif