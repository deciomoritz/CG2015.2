#include "DisplayFile.hpp"
class Window {
private:
	Coordenada wMax, wMin;
	DisplayFile disp;
public:
	Window(Coordenada wMax, Coordenada wMin) {
		this->wMax = wMax;
		this->wMin = wMin;
	}
	~Window() {

	}

	Coordenada getwMax() {
		return wMax;
	}
	Coordenada getwMin() {
		return wMin;
	}
	DisplayFile getDisplay() {
		return disp;
	}

	void deslocarWindow(Coordenada desl) {
		wMax = wMax + desl;
		wMin = wMin + desl;
	}

	void zoomWindow(double z) {
		wMax = wMax * z;
		wMin = wMin * z;
	}

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
