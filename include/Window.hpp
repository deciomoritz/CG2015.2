#ifndef WINDOW_H_
#define WINDOW_H_

#include "DisplayFile.hpp"
#include <math.h>

class Window {

private:
	Coordenada  wMin, wMax;
	DisplayFile disp;
	DisplayFile disp_virtual;
	double altura, largura;

public:

	Window(Coordenada wMax, Coordenada wMin) {
		largura = fabs(wMax.getX() - wMin.getX());
		altura = fabs(wMax.getY() - wMin.getY());
		this->wMax = wMax;
		this->wMin = wMin;
	}

	~Window() {
	}

	void setTamanho(Coordenada c){
		largura *= c.getX();
		altura *= c.getY();
	}

	double getLargura(){
		return largura;
	}

	double getAltura(){
		return altura;
	}

	Coordenada* getwMax() {
		return &wMax;
	}
	Coordenada* getwMin() {
		return &wMin;
	}
	Coordenada getwCentro() {
		Coordenada result = wMax+wMin;
		Coordenada centro(result.getX()/2,result.getY()/2,result.getZ()/2);
		return centro;
	}
	DisplayFile getDisplay() {
		return disp;
	}
	DisplayFile* getDisplay_virtual() {
			return &disp_virtual;
	}

//	void deslocarWindow(Coordenada desl) {
//		wMax = wMax + desl;
//		wMin = wMin + desl;
//	}
//
//	void zoomWindow(double z) {
//		wMax = wMax * z;
//		wMin = wMin * z;
//	}

	void adicionaObjeto(Objeto* obj) {
		disp.adiciona(obj);
	}

	bool contem(string nomeDoObjeto) {
		for (int i = 0; i < disp.getSize(); ++i) {
			Objeto & obj = **disp.posicaoMem(i);
			if (obj.nome().compare(nomeDoObjeto.c_str()) == 0)
				return true;
		}
		return false;
	}

	Objeto* getObjeto(string nome) {
		for (int i = 0; i < disp.getSize(); ++i) {
			Objeto & obj = **disp.posicaoMem(i);
			if (obj.nome().compare(nome.c_str()) == 0)
				return &obj;
		}
		return NULL;
	}

	void clear(){
		disp.destroiLista();
	}

	void load(DisplayFile displayFile){
		disp.destroiLista();
		for (int i = 0; i < displayFile.getSize(); ++i) {
			disp.adiciona(*displayFile.posicaoMem(i));
		}
	}
};
#endif /* WINDOW_H_ */
