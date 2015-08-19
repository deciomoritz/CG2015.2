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
	double x, y;
public:
	Coordenada(double x1, double y1){
		x=x1;
		y=y1;
	}
	virtual ~Coordenada();
	double getX(){
		return x;
	}
	double getY(){
		return y;
	}

};

#endif /* COORDENADAS_HPP_ */
