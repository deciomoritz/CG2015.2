#ifndef WINDOW_H_
#define WINDOW_H_

#include "DisplayFile.hpp"
#include <math.h>

class Window {

private:
	Coordenada wMin, wMax;
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

	void adicionaObjeto(vector<string> aux, bool curva, bool isBezier, bool preenchido) {

		Tipo tipo;
		if(aux.size() == 4){
			tipo = Ponto;
		}else if(aux.size() == 7){
			tipo = Reta;
		}else if(curva){
			if(isBezier)
				tipo = CurvaBezier;
			else
				tipo = CurvaSpline;
		}else{
			tipo = Poligono;
		}

		Objeto* obj = new Objeto(aux[0], tipo, preenchido);
		Coordenada* coord1, *coord2;
		for (int i = 1; i < aux.size() - 1; i += 6) {
			coord1 = new Coordenada(atof(aux[i].c_str()),
					atof(aux[i + 1].c_str()), atof(aux[i + 2].c_str()));
			coord2 = new Coordenada(atof(aux[i + 3].c_str()),
					atof(aux[i + 4].c_str()), atof(aux[i + 5].c_str()));

			Coordenada * coordAux1 = contem(*coord1);
			Coordenada * coordAux2 = contem(*coord2);
			cout << coordAux1 << "   " << coordAux2 << endl;
			//nenhuma existe
			if(coordAux1 == NULL && coordAux2 == NULL){
//				cout << "nenhuma existe" << endl;
				coordAux1 = coord1;
				coordAux2 = coord2;
				obj->adiciona(*coord1);
				obj->adiciona(*coord2);
				//existe apenas uma
			}else if(coordAux1 == NULL && coordAux2 != NULL){
//				cout << "coord2 existe" << endl;
				obj->adiciona(*coord1);
				coordAux1 = coord1;
				//existe apenas uma
			}else if(coordAux2 == NULL && coordAux1 != NULL){
//				cout << "coord1 existe" << endl;
				obj->adiciona(*coord2);
				coordAux2 = coord2;
			}else{
//				cout << "vai tomar no cu" << endl;
			}
//						cout << "aresta" << coordAux1->getX() << coordAux1->getY() << coordAux1->getZ() << coordAux2->getX() << coordAux2->getY() << coordAux2->getZ() << endl;
			obj->adiciona(coordAux1, coordAux2);
		}
		disp.adiciona(obj);
	}

	Coordenada * contem(Coordenada coord) {
		return disp.contem(coord);
	}

	void setTamanho(Coordenada c) {
		largura *= c.getX();
		altura *= c.getY();
	}

	double getLargura() {
		return largura;
	}

	double getAltura() {
		return altura;
	}

	Coordenada* getwMax() {
		return &wMax;
	}
	Coordenada* getwMin() {
		return &wMin;
	}
	Coordenada getwCentro() {
		Coordenada result = wMax + wMin;
		Coordenada centro(result.getX() / 2, result.getY() / 2,
				result.getZ() / 2);
		return centro;
	}
	DisplayFile getDisplay() {
		return disp;
	}

	void setDisplay_virtual(DisplayFile virt) {
		disp_virtual = virt;
	}
	DisplayFile* getDisplay_virtual() {
		return &disp_virtual;
	}

	void adicionaObjeto(Objeto* obj) {
		disp.adiciona(obj);
	}

	void adicionaVirtual(Objeto* obj) {
		disp_virtual.adiciona(obj);
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

	void clear() {
		disp.destroiLista();
	}

	void load(DisplayFile displayFile) {
		disp.destroiLista();
		for (int i = 0; i < displayFile.getSize(); ++i) {
			disp.adiciona(*displayFile.posicaoMem(i));
		}
	}
};
#endif /* WINDOW_H_ */
