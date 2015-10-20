#include "../Objeto.hpp"
#include "ManipulaMatriz.hpp"
class ManipulaObjeto {
private:
	ManipulaMatriz *manipulador;
public:
	void transforma2DObjeto(Objeto* b, vector<vector<double> >transformada){
		ListaEnc<Coordenada>* coordenadas = b->pontos();
		for(int i=0; i<coordenadas->getSize(); i++){
			Coordenada antiga = coordenadas->retiraDoInicio();
			vector<vector<double> > nova = manipulador->multiplicaMatriz(antiga.get2DVector(), transformada);
			Coordenada nova_c(nova[0][0],nova[1][0],nova[2][0]);
			coordenadas->adiciona(nova_c);
		}
	}
	void transformaObjeto(Objeto* b, vector<vector<double> >transformada){
			ListaEnc<Coordenada>* coordenadas = b->pontos();
			for(int i=0; i<coordenadas->getSize(); i++){
				Coordenada antiga = coordenadas->retiraDoInicio();
				vector<vector<double> > nova = manipulador->multiplicaMatriz(antiga.getVector(), transformada);
				Coordenada nova_c(nova[0][0],nova[1][0],nova[2][0]);
				coordenadas->adiciona(nova_c);
			}
		}

	ManipulaObjeto(){	manipulador = new ManipulaMatriz();}
	~ManipulaObjeto(){ delete manipulador;	}
	void translada(Objeto* obj, Coordenada c){
		vector<vector<double> > m = manipulador->getTranslacao(c);
		transforma2DObjeto(obj, m);

	}
	void translada3D(Objeto* obj, Coordenada c){
		vector<vector<double> > m = manipulador->getTranslacao3D(c);
		transformaObjeto(obj, m);

	}
	void escalona(Objeto* obj, Coordenada c){
		Coordenada centro = obj->getCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),1);
		vector<vector<double> > m = manipulador->getTranslacao(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getEscalonamento(c));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao(centro));
		transforma2DObjeto(obj, m);
	}
	void escalona3D(Objeto* obj, Coordenada c){
		Coordenada centro = obj->getCentro();
		Coordenada a(-1*centro.getX(),-1*centro.getY(),-1*centro.getZ());
		vector<vector<double> > m = manipulador->getTranslacao3D(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getEscalonamento3D(c));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao3D(centro));
		transformaObjeto(obj, m);
	}
	void rotaciona(Objeto* obj, Coordenada coord, double angulo){
		/* (0,0,1): rotaciona no centro do mundo
		 * getCentro: rotaciona no centro do objeto
		 * (x, y, 1): rotaciona no  ponto
		 */
		Coordenada a(-1*coord.getX(),-1*coord.getY(),1);
		vector<vector<double> > m = manipulador->getTranslacao(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacao(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao(coord));
		transforma2DObjeto(obj, m);
	}
	void rotacionaX(Objeto* obj, Coordenada coord, double angulo){
		/* (0,0,1): rotaciona no centro do mundo
		 * getCentro: rotaciona no centro do objeto
		 * (x, y, 1): rotaciona no  ponto
		 */
		Coordenada a(-1*coord.getX(),-1*coord.getY(), -1*coord.getZ());
		vector<vector<double> > m = manipulador->getTranslacao3D(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoX(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao3D(coord));
		transformaObjeto(obj, m);
	}
	void rotacionaY(Objeto* obj, Coordenada coord, double angulo){
		/* (0,0,1): rotaciona no centro do mundo
		 * getCentro: rotaciona no centro do objeto
		 * (x, y, 1): rotaciona no  ponto
		 */
		Coordenada a(-1*coord.getX(),-1*coord.getY(), -1*coord.getZ());
		vector<vector<double> > m = manipulador->getTranslacao3D(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoY(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao3D(coord));
		transformaObjeto(obj, m);
	}
	void rotacionaZ(Objeto* obj, Coordenada coord, double angulo){
		/* (0,0,1): rotaciona no centro do mundo
		 * getCentro: rotaciona no centro do objeto
		 * (x, y, 1): rotaciona no  ponto
		 */
		Coordenada a(-1*coord.getX(),-1*coord.getY(), -1*coord.getZ());
		vector<vector<double> > m = manipulador->getTranslacao3D(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoZ(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao3D(coord));
		transformaObjeto(obj, m);

		cout << "matriz de rotação" << endl;
		manipulador->printaMatriz(manipulador->getRotacaoZ(angulo));

		cout << obj->to_string() << endl;
	}
	void rotacionaEixo(Objeto* obj, Coordenada coordA, Coordenada coordB, double angulo){
		/* (0,0,1): rotaciona no centro do mundo
		 * getCentro: rotaciona no centro do objeto
		 * (x, y, 1): rotaciona no  ponto
		 */
		Coordenada a(-1*coordA.getX(),-1*coordA.getY(), -1*coordA.getZ());
		Coordenada novoB = a+coordB;
		double cosX = novoB.getZ()/(pow((pow(novoB.getY(),2)+pow(novoB.getZ(),2)),0.5));
		double senX = novoB.getY()/(pow((pow(novoB.getY(),2)+pow(novoB.getZ(),2)),0.5));

		vector<vector<double> > m = manipulador->getTranslacao3D(a);
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoX(senX, cosX));
		vector<vector<double> > transfNovoB =manipulador->multiplicaMatriz(m,novoB.getVector());
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoY(transfNovoB[0][0], transfNovoB[0][2]));
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoZ(angulo));
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoY(-1*transfNovoB[0][0], -1*transfNovoB[0][2]));
		m = manipulador->multiplicaMatriz(m, manipulador->getRotacaoX(-1*senX, -1*cosX));
		m = manipulador->multiplicaMatriz(m, manipulador->getTranslacao3D(coordA));
		transformaObjeto(obj, m);
	}
};
