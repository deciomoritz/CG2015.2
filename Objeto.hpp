#ifndef OBJETO_HPP_
#define OBJETO_HPP_
#include <string>
#include "ListaEnc.hpp"
#include "Coordenadas.hpp"

using namespace std;
class Objeto {
private:
	string nome;
	ListaEnc<Coordenadas> pontos;
public:
	Objeto(string nome);
	virtual ~Objeto();
};

#endif /* OBJETO_HPP_ */
