#ifndef OBJECT_H
#define OBJECT_H
#include <inttypes.h>
#include <string>
#include "Coordinate.h"

class Object {
 protected:
 	std::string name;
 	uint32_t id;
 public:
 	Object();
 	Object(std::string name, uint32_t id);
 	~Object();
 	std::string getName();
 	uint32_t getId();
 	virtual Object* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
};

#endif