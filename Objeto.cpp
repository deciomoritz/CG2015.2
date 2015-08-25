/*
 * Objeto.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: luiz
 */

#include "include/Objeto.hpp"
#include <string>

string Objeto::to_string(){
	string s = _nome + " ";

	for (int j = 0; j < _pontos.getSize(); ++j) {
		Coordenada & coord = *_pontos.posicaoMem(j);
		s += "(" + std::to_string(coord.getX()) + "," + std::to_string(coord.getY()) + "),";
	}

	return s;
}

