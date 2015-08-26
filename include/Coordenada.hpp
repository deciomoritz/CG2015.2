/*
 * Coordenadas.hpp
 *
 *  Created on: Aug 19, 2015
 *      Author: luiz
 */

#ifndef COORDENADA_HPP_
#define COORDENADA_HPP_
#include <vector>
using namespace std;

class Coordenada {
private:
	vector<vector<double> > ponto;
public:
	Coordenada(double x1, double y1, double z1):ponto(3,vector<double>(1,0)){
		ponto[0][0]=x1;
		ponto[1][0]=y1;
		ponto[2][0]=z1;
	}

	Coordenada(){}
	 ~Coordenada(){};
	double getX(){
		return ponto[0][0];
	}
	double getY(){
		return ponto[1][0];
	}
	double getZ(){
		return ponto[2][0];
	}
	Coordenada operator* (double o);
	friend Coordenada operator +(const Coordenada & a, const Coordenada & b);
	friend Coordenada operator -(const Coordenada & a, const Coordenada & b);
	friend bool operator ==(const Coordenada & a, const Coordenada & b);
};

#endif /* COORDENADAS_HPP_ */
