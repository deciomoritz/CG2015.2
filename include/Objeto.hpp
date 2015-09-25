#ifndef OBJETO_HPP_
#define OBJETO_HPP_
#include <string>

#include "Coordenada.hpp"
#include "ListaEnc.hpp"

using namespace std;


enum Tipo{ Ponto, Reta, Poligono, Curva};

class Objeto {

protected:
	string _nome;
	ListaEnc<Coordenada> _pontos;
	Tipo tipo;
	bool preenchido;

public:
	Objeto(){};
	virtual ~Objeto(){};

	Objeto(string nome, Tipo tipo, bool preenchido){
		this->_nome = nome;
		this->tipo = tipo;
		this->preenchido = preenchido;
	};

	Objeto(string nome){
		this->_nome = nome;
		this->tipo = tipo;
		this->preenchido = preenchido;
	};

	Tipo getTipo(){
		return tipo;
	}

	void setTipo(Tipo tipo){//usado na leitura de objetos de arquivo
		this->tipo = tipo;
	}

	bool isPreenchido(){
		return preenchido;
	}

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
