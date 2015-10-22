/*
 * Objeto3D.hpp
 *
 *  Created on: 22 Oct 2015
 *      Author: decio
 */

#ifndef INCLUDE_OBJETO3D_HPP_
#define INCLUDE_OBJETO3D_HPP_

#include <utility>

typedef pair<Coordenada*, Coordenada*> aresta;

class Objeto3D : public Objeto{

	ListaEnc<aresta> _arestas;

	void adiciona(Coordenada * c1, Coordenada * c2){
		_arestas.adiciona(make_pair(c1,c2));
	}

	ListaEnc<aresta> * arestas(){
		return &_arestas;
	}
};

#endif /* INCLUDE_OBJETO3D_HPP_ */
