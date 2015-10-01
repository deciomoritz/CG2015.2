#include "include/Objeto.hpp"
#include <string>

string Objeto::to_string() {
	string s = _nome + " ";

	switch (tipo) {
	case Ponto:
		s += " Ponto ";
		break;
	case Reta:
		s += " Reta ";
		break;
	case Poligono:
		s += " Polígono ";
		break;
	case CurvaBezier:
		s += " Bezier ";
		break;
	case CurvaSpline:
		s += " Spline ";
		break;
	default:
		break;
	}

	for (int j = 0; j < _pontos.getSize(); ++j) {
		Coordenada & coord = *_pontos.posicaoMem(j);
		s += "(" + std::to_string(coord.getX()) + ","
				+ std::to_string(coord.getY()) + "),";
	}
	return s;
}

Coordenada Objeto::getCentro() {
	Coordenada centro(0, 0, 0);
	Elemento<Coordenada>* it_pontos = _pontos.getHead();
	int i = 0;
	for (; i < _pontos.getSize(); i++) {
		centro = centro + it_pontos->getInfo();
		it_pontos = it_pontos->getProximo();
	}
	Coordenada teste(centro.getX() / i, centro.getY() / i, centro.getZ() / i);
	return teste;
}

string Objeto::to_obj(){
	string s = "#" + _nome + " " + std::to_string(preenchido) + "\n" + "v ";

	for (int j = 0; j < _pontos.getSize(); ++j) {
		Coordenada & coord = *_pontos.posicaoMem(j);
		s += std::to_string(coord.getX()) + " "
				+ std::to_string(coord.getY()) + " ";
	}
	s += "\n\n";
	return s;
}

bool operator ==(const Objeto & a, const Objeto & b) {
//	return a.nome().compare(b.nome())==0;
	return true;
}

