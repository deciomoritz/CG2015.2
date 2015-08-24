/*
 * Coordenadas.hpp
 *
 *  Created on: Aug 19, 2015
 *      Author: luiz
 */

#ifndef COORDENADA_HPP_
#define COORDENADA_HPP_

class Coordenada {
private:
	double x, y, z;
public:
	Coordenada(double x1, double y1, double z1){
		x=x1;
		y=y1;
		z=z1;
	}

	Coordenada(){}
	 ~Coordenada(){};
	double getX(){
		return x;
	}
	double getY(){
		return y;
	}
	double getZ(){
		return z;
	}
	Coordenada operator* (double o);
	friend Coordenada operator +(const Coordenada & a, const Coordenada & b);
	friend Coordenada operator -(const Coordenada & a, const Coordenada & b);
};

#endif /* COORDENADAS_HPP_ */
