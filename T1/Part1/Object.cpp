#include "Object.h"
#include <string>

std::string Object::getName(){ 

	return this->name;
}

uint32_t Object::getId() {
	return this->id;
}

Object::Object(std::string name, uint32_t id) {
	this->id = id;
	this->name = name;
}
Object::Object() {
	this->name = "default_name";
	this-> id  = 1;
}
Object:: ~Object() {

}
Object* Object::transformToViewport(int xwmin, int xwmax, int xvpmin, int xvpmax,
						  		 	int ywmin, int ywmax, int yvpmin, int yvpmax) { };