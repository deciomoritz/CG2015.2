/*
 * Viewport.h
 *
 *  Created on: 19 Aug 2015
 *      Author: decio
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "Window.hpp"
#include <cairo/cairo.h>

class Viewport {
	Coordenada vMax, vMin;
public:
	Viewport(Coordenada vMax, Coordenada vMin) {
		//verificar coordenadas
		this->vMax = vMax;
		this->vMin = vMin;
	}

	~Viewport() {
	}

	Coordenada& getVMax(){
		return vMax;
	}

	Coordenada& getVMin(){
			return vMin;
		}

	DisplayFile transformadaViewport(Window w) {
		DisplayFile novoDisplay;
		DisplayFile originalDisplay = *w.getDisplay_virtual();
		//constantes da transformacao
		double divX, divY, multX, multY;
		//		divX = (w.getwMax() - w.getwMin()).getX();
		//		divY = (w.getwMax() - w.getwMin()).getY();
		//Alterado para 2 devido ao processo de normalizacao das coordenadas do mundo: window=(-1,-1), (1,1)
		divX = 2;
		divY = 2;
		multX = (vMax - vMin).getX();
		multY = (vMax - vMin).getY();
		// iterando lista de objetos
		for (int i = 0; i < originalDisplay.getSize(); i++) {
			Objeto &obj = **originalDisplay.posicaoMem(i);
			Objeto *novoObj = new Objeto(obj.getNome(),obj.getTipo(), obj.isPreenchido());
			ListaEnc<Coordenada>* coordenadas_obj = obj.pontos();

			ListaEnc<aresta> *arestas = obj.arestas();
			cout << "num de arestas" << arestas->getSize() << endl;
			for (int k = 0; k < arestas->getSize(); k++) {
				aresta *a = arestas->posicaoMem(k);

				Coordenada &c1 = *a->first;
				Coordenada &c2 = *a->second;

				double newX, newY, newZ;

				newX = ((c1.getX() +1) / divX) * multX + vMin.getX();
				newY = (1 - (c1.getY() +1) / divY) * multY + vMin.getY();
				newZ = 1;

				Coordenada* novoPonto = new Coordenada(newX, newY, newZ);
				novoObj->adiciona(*novoPonto);

				newX = ((c2.getX() +1) / divX) * multX + vMin.getX();
				newY = (1 - (c2.getY() +1) / divY) * multY + vMin.getY();
				newZ = 1;

				Coordenada* novoPonto2 = new Coordenada(newX, newY, newZ);
				novoObj->adiciona(*novoPonto);

				novoObj->adiciona(novoPonto, novoPonto2);
			}
			novoDisplay.adicionaNoInicio(novoObj);
		}
		return novoDisplay;
	}

	Coordenada* transformadaViewport(Coordenada atual) {
		//constantes da transformacao
		double divX, divY, multX, multY;
		divX = 2;
		divY = 2;
		multX = (vMax - vMin).getX();
		multY = (vMax - vMin).getY();
		// iterando lista de objetos
		//iterando as coordenadas do objeto
		//Transformacao
		double newX, newY, newZ;
//		newX = ((atual.getX() +1) / divX) * multX;
		newX = (1 - (atual.getX() +1) / divX) * multX;
		newY = (1 - (atual.getY() +1) / divY) * multY;
		newZ = 1;
		//criando novo ponto
		return new Coordenada(newX, newY, newZ);
	}
};

#endif /* VIEWPORT_H_ */
