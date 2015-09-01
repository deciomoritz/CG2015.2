#include "ManipulaObjeto.hpp"
#include "../DisplayFile.hpp"
class ManipulaMundo {
private:
	ManipulaMatriz *manipulaMtr;
	ManipulaObjeto *manipulaObj;

	void transformaMundo(Objeto* Obj, vector<vector<double> >transformada){
		Elemento<Objeto*>* it_Objeto = display->getHead();
		for (int i=0; i<display->getSize(); i++){
			Objeto* obj = it_Objeto->info;
			ListaEnc<Coordenada>* coord_real = obj->pontos();
			ListaEnc<Coordenada>* coord_virtual = obj->pontos_virtual();
			Elemento<Coordenada>* it_coord = coord_real->getHead();
			for(int j=0; j<coord_real->getSize(); j++){
				Coordenada *real = it_coord->info;
				coord_virtual->retiraDoInicio();
				vector<vector<double> > nova = manipulaMtr->multiplicaMatriz(real->getVector(), transformada);
				Coordenada nova_c(nova[0][0],nova[1][0],nova[2][0]);
				coord_virtual->adiciona(nova_c);
				it_coord = it_coord->getProximo();
			}
			it_Objeto = it_Objeto->getProximo();
		}
	}




public:
	ManipulaMundo(){
		manipulaMtr = new ManipulaMatriz();
		manipulaObj = new ManipulaObjeto();

	}
	~ManipulaMundo(){
		delete manipulaMtr;
	}

	void Rotaciona(DisplayFile* ori, DisplayFile* virt, double angulo){
		for (int i=0; i<display->getSize(); i++){
					transformaMundo(it_Objeto->info,m);
					it_Objeto = it_Objeto->getProximo();
				}
		Coordenada centro = window->getwCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulaMtr->getTranslacao(a);
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getRotacao(angulo));
		m = manipulaMtr->multiplicaMatriz(m, manipulaMtr->getTranslacao(coord));
		transformaMundo(display, m);
	}
};
