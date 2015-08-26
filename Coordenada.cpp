#include "include/Coordenada.hpp"
Coordenada Coordenada::operator *(double o){

	double newX, newY, newZ;
	newX = this->x*o;
	newY = this->y*o;
	newZ = 1;
	return  Coordenada(newX, newY, newZ);
}
Coordenada operator +(const Coordenada & a, const Coordenada & b){
	double newX, newY, newZ;
	newX = a.x + b.x;
	newY = a.y + b.y;
	newZ = 1;
	return  Coordenada(newX, newY, newZ);
}
Coordenada operator -(const Coordenada & a, const Coordenada & b){
	double newX, newY, newZ;
	newX = a.x - b.x;
	newY = a.y - b.y;
	newZ = 1;
	return  Coordenada(newX, newY, newZ);
}

bool operator ==(const Coordenada & a, const Coordenada & b){
	return (a.x == b.x && a.y == b.y) && a.z == b.z;
}
