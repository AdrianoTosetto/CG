#include "Coordinate.h"

class Viewport {
 private:
 	Coordinate origin;
 	Coordinate limit;
 public:
 	Viewport(Coordinate _origin, Coordinate _limit) : origin(_origin), limit(_limit) {
 		
 	}
};