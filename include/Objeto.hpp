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

	void setNome(string nome){
		_nome = nome;
	}

	string getNome(){
			return _nome;
		}

	ListaEnc<Coordenada>* pontos(){
		return &_pontos;
	}

	void adiciona(const Coordenada & c){
		_pontos.adiciona(c);
	}

	string to_string();

	string to_obj();

	Coordenada getCentro();

	friend bool operator ==(const Coordenada & a, const Coordenada & b);

};

#endif /* OBJETO_HPP_ */
