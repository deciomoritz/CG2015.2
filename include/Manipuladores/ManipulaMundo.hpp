#include "../DisplayFile.hpp"
#include "ManipulaObjeto"
class ManipulaMundo {
private:
	ManipulaMatriz *manipulaMtr;
	ManipulaObjeto *manipulaObj;

public:
	ManipulaMundo(){
		manipulaMtr = new ManipulaMatriz();
		manipulaObj = new ManipulaObjeto();
	}
	~ManipulaMundo(){
		delete manipulaMtr;
		delete manipulaObj;
	}
	void fuckMundo(Display ori, Display *virt, vector<vector<double> >m, Coordenada fator){
		virt->destroiLista();
		Elemento<Objeto*>* it_objeto = ori.getHead();

		for(int i =0; i< ori.getSize(); i++){
			Objeto* obj= it_objeto->info;
			Objeto* obj_virtual = new Objeto(obj->getNome());
			ListaEnc<Coordenada>* pontos = obj->pontos();
			Elemento<Coordenada>* it_pontos = pontos->getHead();
			for(int j =0; j< pontos.getSize(); j++){
				Coordenada coord_real = it_pontos->info;
				//Pode dar merda por ser temporário....
				Coordenada coord_virtual(1,1,1);
				coord_virtual.setVector(manipulaMtr->multiplicaMatriz(coord_real.getVector(),m));
				it_pontos = it_pontos->getProximo();
				obj_virtual->adiciona(coord_virtual);
			}
			it_objeto = it_objeto->getProximo();
			manipulaObj->escalona(obj_virtual, fator);
			virt->adicionaNoInicio(obj_virtual);
		}
	}
};
