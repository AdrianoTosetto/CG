#ifndef OBJECT_H
#define OBJECT_H
#include <inttypes.h>
#include <string>

class Object {
 protected:
 	std::string name;
 	uint32_t id;
 public:
 	Object();
 	Object(std::string name, uint32_t id);
 	~Object();
 	uint32_t getId();
 	std::string getName();
};

#endif