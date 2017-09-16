#ifndef OBJECT_H
#define OBJECT_H
#include <inttypes.h>
#include <string>
#include "Coordinate.h"

#define TPOINT 0x10
#define TSTRAIGHT 0x20
#define TPOLYGON 0x30
#define TOBJECT 0x00

class Object {
 protected:
 	std::string name;
 	uint32_t id;
 	uint32_t type = TOBJECT; 
 public:
 	Object();
 	Object(std::string name, uint32_t id);
 	~Object();
 	std::string getName();
 	uint32_t getId();
 	virtual Object* transformToViewport(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
 	virtual void transformToWindow(Coordinate wor, Coordinate wli, Coordinate vpor, Coordinate vpli);
 	uint32_t getType();

};

#endif