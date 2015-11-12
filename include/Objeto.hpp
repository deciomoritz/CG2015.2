#ifndef OBJETO_HPP_
#define OBJETO_HPP_
#include <string>

#include "Coordenada.hpp"
#include "ListaEnc.hpp"

#include <utility>

using namespace std;

typedef pair<Coordenada*, Coordenada*> aresta;

enum Tipo{ Ponto, Reta, Poligono, CurvaBezier, CurvaSpline, Solido};

class Objeto {

protected:
	string _nome;
	ListaEnc<Coordenada> _pontos;
	Tipo tipo;
	bool preenchido;
	ListaEnc<aresta> _arestas;

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

	Coordenada * contem(Coordenada coord){
		for (int j = 0; j < _pontos.getSize(); ++j) {
			Coordenada *c = _pontos.posicaoMem(j);
			if(coord == *c)
				return c;
		}
		return NULL;
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

	ListaEnc<Objeto*> getRetas(){
		ListaEnc<Objeto*> retas;
		for(int i =0; i<_arestas.getSize(); i++){
			aresta* a = _arestas.posicaoMem(i);
			Objeto* novo = new Objeto(" ", Reta, preenchido);
			novo->adiciona(*a->first);
			novo->adiciona(*a->second);
			retas.adiciona(novo);
		}
		return retas;
	}
	string getNome(){
		return _nome;
	}

	void adiciona(Coordenada * c1, Coordenada * c2){
		_arestas.adiciona(make_pair(c1,c2));
	}

	ListaEnc<aresta> * arestas(){
		return &_arestas;
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
