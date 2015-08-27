#include "Objeto.hpp"
#include "ManipulaMatriz.hpp"
class ManipulaObjeto {
	ManipulaMatriz *manipulador;
private:
	void transformaObjeto(Objeto* b, vector<vector<double> >transformada){
		ListaEnc<Coordenada>* coordenadas = b->pontos();
		for(int i=0; i<coordenadas->getSize(); i++){
			Coordenada antiga = coordenadas->retiraDoInicio();
			vector<vector<double> > nova = manipulador->multiplicaMatriz(antiga.getVector(), transformada);
			Coordenada nova_c(nova[0][0],nova[1][0],nova[2][0]);
			//cout << nova[0][0]<<","<< nova[1][0]<<","<<nova[2][0]<<"\n";
			coordenadas->adiciona(nova_c);
		}
	}
public:
	ManipulaObjeto(){	manipulador = new ManipulaMatriz();}
	~ManipulaObjeto(){ delete manipulador;	}
	void Translada(Objeto* obj, Coordenada c){
		vector<vector<double> > m = manipulador->getTranslacao(c);
		transformaObjeto(obj, m);

	}
	void Escalona(Objeto* obj, Coordenada c){
		Coordenada centro = obj->getCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulador->getTranslacao(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getEscalonamento(c));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao(centro));
		transformaObjeto(obj, m);
	}
	void Rotaciona(Objeto* obj, Coordenada coord, double angulo){
		/* (0,0,1): rotaciona no centro do mundo
		 * getCentro: rotaciona no centro do objeto
		 * (x, y, 1): rotaciona no  ponto
		 */
		Coordenada a(-1*coord.getX(),-1*coord.getY(),1);
		vector<vector<double> > m = manipulador->getTranslacao(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacao(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao(coord));
		transformaObjeto(obj, m);
	}
};
