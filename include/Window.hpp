#include "DisplayFile.hpp"
class Window {
private:
	Coordenada  wMax, wMin;
	DisplayFile disp;
public:
	Window(Coordenada wMax, Coordenada wMin){
		this->wMax = wMax;
		this->wMin = wMin;
	}
	~Window(){

	}

	Coordenada getwMax(){
		return wMax;
	}
	Coordenada getwMin(){
		return wMax;
	}
	DisplayFile getDisplay(){
		return disp;
	}
//Chance de dar merda, por não retornar novo objeto e o conteúdo estar na stack!!!!!!!!!!!!!!!!
	void delocarWindow(Coordenada desl){
		wMax = wMax+desl;
		wMin = wMin+desl;
	}

	void zoomWindow(double z){
		wMax = wMax*z;
		wMin = wMin*z;
	}


};