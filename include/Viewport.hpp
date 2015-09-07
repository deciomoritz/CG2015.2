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
		Elemento<Objeto*>* it_originalDisplay = originalDisplay.getHead();
		for (int i = 0; i < originalDisplay.getSize(); i++) {
			Objeto *novoObj = new Objeto((it_originalDisplay->getInfo())->nome());
			ListaEnc<Coordenada>* coordenadas_obj = (it_originalDisplay->getInfo())->pontos();
			Elemento<Coordenada>* it_coordenada = coordenadas_obj->getHead();
			//iterando as coordenadas do objeto
			for (int j = 0; j < coordenadas_obj->getSize(); j++) {
				Coordenada atual = it_coordenada->getInfo();
				//Transformacao
				double newX, newY, newZ;
				//				newX = ((atual.getX() - w.getwMin().getX()) / divX) * multX;
				//				newY = (1 - (atual.getY() - w.getwMin().getY()) / divY) * multY;
				//Alterado para 2 devido ao processo de normalizacao das coordenadas do mundo: window=(-1,-1), (1,1)
				newX = ((atual.getX() +1) / divX) * multX;
				newY = (1 - (atual.getY() +1) / divY) * multY;
				newZ = 1;
				//criando novo ponto
				Coordenada* novoPonto = new Coordenada(newX, newY, newZ);
				novoObj->adiciona(*novoPonto);
				it_coordenada = it_coordenada->getProximo();
			}
			novoDisplay.adicionaNoInicio(novoObj);
			it_originalDisplay = it_originalDisplay->getProximo();
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
