#include "include/Coordenada.hpp"
Coordenada Coordenada::operator *(double o){

	double newX, newY, newZ;
	newX = this->ponto[0][0]*o;
	newY = this->ponto[1][0]*o;
	newZ = 1;
	return  Coordenada(newX, newY, newZ);
}
Coordenada operator +(const Coordenada & a, const Coordenada & b){
	double newX, newY, newZ;
	newX = a.ponto[0][0] + b.ponto[0][0];
	newY = a.ponto[1][0] + b.ponto[1][0];
	newZ = 1;
	return  Coordenada(newX, newY, newZ);
}
Coordenada operator -(const Coordenada & a, const Coordenada & b){
	double newX, newY, newZ;
	newX = a.ponto[0][0] - b.ponto[0][0];
	newY = a.ponto[1][0] - b.ponto[1][0];
	newZ = 1;
	return  Coordenada(newX, newY, newZ);
}

bool operator ==(const Coordenada & a, const Coordenada & b){
	return (a.ponto[0][0] == b.ponto[0][0] && a.ponto[1][0] == b.ponto[1][0]) && a.ponto[2][0] == b.ponto[2][0];
}
