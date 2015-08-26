#include "DisplayFile.hpp"
class Window {
private:
	Coordenada  wMax, wMin;
	DisplayFile disp;
public:
	Window(Coordenada wMax, Coordenada wMin){
		//verificar coordenadas
		this->wMax = wMax;
		this->wMin = wMin;
	}
	~Window(){

	}

	Coordenada getwMax(){
		return wMax;
	}
	Coordenada getwMin(){
		return wMin;
	}
	DisplayFile getDisplay(){
		return disp;
	}
//Chance de dar merda, por não retornar novo objeto e o conteúdo estar na stack!!!!!!!!!!!!!!!!
	void deslocarWindow(Coordenada desl){
		wMax = wMax+desl;
		wMin = wMin+desl;
	}

	void zoomWindow(double z){
		wMax = wMax*z;
		wMin = wMin*z;
	}

	void adicionaObjeto(Objeto* obj){
		disp.adiciona(obj);
	}


	Objeto* getObjeto(string nome){
		Objeto *ref = new Objeto(nome);
		Objeto* retorno = *disp.posicaoMem(ref);
		delete ref;
		return retorno;
	}
};
