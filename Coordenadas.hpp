/*
 * Coordenadas.hpp
 *
 *  Created on: Aug 19, 2015
 *      Author: luiz
 */

#ifndef COORDENADAS_HPP_
#define COORDENADAS_HPP_

class Coordenadas {
private:
	double x, y;
public:
	Coordenadas(double x1, double y1){
		x=x1;
		y=y1;
	}
	virtual ~Coordenadas();
	double getX(){
		return x;
	}
	double getY(){
		return y;
	}

};

#endif /* COORDENADAS_HPP_ */
