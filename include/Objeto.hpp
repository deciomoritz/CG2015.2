#ifndef OBJETO_HPP_
#define OBJETO_HPP_
#include <string>

#include "Coordenada.hpp"
#include "ListaEnc.hpp"

using namespace std;
class Objeto {

protected:
	string _nome;
	ListaEnc<Coordenada> _pontos;

public:
	Objeto(){};
	Objeto(string nome){
		this->_nome = nome;
	};

	~Objeto(){
	};

	string nome(){
		return _nome;
	}

	ListaEnc<Coordenada>* pontos(){
		return &_pontos;
	}

	void adiciona(const Coordenada & c){
		_pontos.adiciona(c);
	}

	string to_string();
	Coordenada getCentro(){
		Coordenada centro(0,0,0);
		Elemento<Coordenada>* it_pontos = _pontos.getHead();
		int i=0;
		for(; i<_pontos.getSize();i++){
			centro = centro + it_pontos->getInfo();
			it_pontos = it_pontos->getProximo();
		}
		Coordenada teste(centro.getX()/i,centro.getY()/i,centro.getZ()/i);
		return teste;
	}
	friend bool operator ==(const Coordenada & a, const Coordenada & b);

};

#endif /* OBJETO_HPP_ */
