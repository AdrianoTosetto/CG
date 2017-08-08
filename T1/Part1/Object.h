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
 	std::string getName();
 	uint32_t getId();
 	virtual Object* transformToViewport(int xwmin, int xwmax, int xvpmin, int xvpmax,
						  			    int ywmin, int ywmax, int yvpmin, int yvpmax);
};

#endif