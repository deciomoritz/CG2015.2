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
bool operator ==(const Objeto & a, const Objeto & b){
//	return a.nome().compare(b.nome())==0;
	return true;
}

