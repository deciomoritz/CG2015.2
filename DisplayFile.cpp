/*
 * DisplayFile.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: luiz
 */

#include "include/DisplayFile.hpp"
#include <string>

string DisplayFile::to_string(){
	string s = "";
	for (int i = 0; i < getSize(); ++i) {
		Objeto & obj = **posicaoMem(i);
		s += obj.to_string() + "\n";
	}
	return s;
}

string DisplayFile::to_string2(){
	string s = "";
	for (int i = 0; i < getSize(); ++i) {
		Objeto & obj = **posicaoMem(i);
		ListaEnc<aresta> *arestas = obj.arestas();
		for (int k = 0; k < arestas->getSize(); ++k) {
			aresta *a = arestas->posicaoMem(k);
			s += "(" + std::to_string(a->first->getX())+ " "+ std::to_string(a->first->getY())+ " " + std::to_string(a->first->getZ())+ " "+ std::to_string(a->second->getX())+ " "+ std::to_string(a->second->getY())+ " " + std::to_string(a->second->getZ())+ " " + ")\n";
		}
	}
	return s;
}

Coordenada * DisplayFile::contem(Coordenada coord){
	for (int i = 0; i < getSize(); ++i) {
		Objeto & obj = **posicaoMem(i);
		ListaEnc<Coordenada> * pontos = obj.pontos();
		for (int j = 0; j < pontos->getSize(); ++j) {
			Coordenada *c = pontos->posicaoMem(j);
			if(coord == *c)
				return c;
		}
	}
	return NULL;
}
