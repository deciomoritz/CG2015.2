#ifndef OBJETO_HPP_
#define OBJETO_HPP_
#include <string>

#include "Coordenada.hpp"
#include "ListaEnc.hpp"

using namespace std;
class Objeto {

private:
	string nome;
	ListaEnc<Coordenada> pontos;

public:
	Objeto(string nome){
		this->nome = nome;
	};

	~Objeto(){
	};

	void insert_gambiarra(Coordenada c){
		pontos.adiciona(c);
	}

};

#endif /* OBJETO_HPP_ */
