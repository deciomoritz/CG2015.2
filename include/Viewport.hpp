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
	cairo_t* _cr;
public:
	Viewport(Coordenada vMax, Coordenada vMin){
		this->vMax = vMax;
		this->vMin = vMin;
	}
	 ~Viewport(){

	 }
	void setCairo(cairo_t* cr){
		this->_cr = cr;
	}
	void desenha(cairo_t *cr, Objeto obj);

	DisplayFile transformadaViewport(Window w){
		DisplayFile novoDisplay;
		DisplayFile ori = w.getDisplay();
		double divX, divY, multX, multY;
		divX = (w.getwMax()-w.getwMin()).getX();
		divY = (w.getwMax()-w.getwMin()).getY();
		multX = (vMax-vMin).getX();
		multY = (vMax-vMin).getY();
		Elemento<Objeto*>* it_ori = ori.getHead();
		for(int i = 0; i<ori.getSize();i++){
			ListaEnc<Coordenada> novasCoordenadas;
			ListaEnc<Coordenada>* coordenadas_obj = (it_ori->getInfo())->pontos();
			Elemento<Coordenada>* it_coordenada = coordenadas_obj->getHead();
			for(int j =0; j<coordenadas_obj->getSize();j++){
				Coordenada atual = it_coordenada->getInfo();
				double newX, newY, newZ;
				newX = ((atual.getX()-w.getwMin().getX())/divX)*multX;
				newY = (1-(atual.getY()-w.getwMin().getY())/divY)*multY;
				newZ=1;
				Coordenada novoPonto(newX, newY, newZ);
//				novasCoordenadas.adicionaNoInicio(novoPonto);
				it_coordenada = it_coordenada->getProximo();
			}

//			novoDisplay.adicionaNoInicio(novoPonto);
			it_ori = it_ori->getProximo();
		}
		return novoDisplay;
	}
};

#endif /* VIEWPORT_H_ */
