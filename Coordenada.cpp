#include "include/Coordenada.hpp"
Coordenada Coordenada::operator *(double o){

	double newX, newY, newZ;
	newX = this->ponto[0][0]*o;
	newY = this->ponto[1][0]*o;
	newZ = this->ponto[2][0]*o;
	return  Coordenada(newX, newY, newZ);
}
Coordenada operator +(const Coordenada & a, const Coordenada & b){
	double newX, newY, newZ;
	newX = a.ponto[0][0] + b.ponto[0][0];
	newY = a.ponto[1][0] + b.ponto[1][0];
	newZ = a.ponto[2][0] + b.ponto[2][0];
	return  Coordenada(newX, newY, newZ);
}
Coordenada operator -(const Coordenada & a, const Coordenada & b){
	double newX, newY, newZ;
	newX = a.ponto[0][0] - b.ponto[0][0];
	newY = a.ponto[1][0] - b.ponto[1][0];
	newZ = a.ponto[2][0] - b.ponto[2][0];
	return  Coordenada(newX, newY, newZ);
}

bool operator ==(const Coordenada & a, const Coordenada & b){
	double threshold = 1.e-7;
	return (a.ponto[0][0] - b.ponto[0][0] < threshold) && (a.ponto[1][0] - b.ponto[1][0] < threshold) && (a.ponto[2][0] - b.ponto[2][0] < threshold);
}
