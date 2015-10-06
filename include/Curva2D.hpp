/*
 * Curva2D.hpp
 *
 *  Created on: 24 Sep 2015
 *      Author: decio
 */

#ifndef INCLUDE_CURVA2D_HPP_
#define INCLUDE_CURVA2D_HPP_

class Curva2D : public Objeto{

public:

	DisplayFile* getRetasBezier(ListaEnc<Coordenada> entrada){
			ManipulaMatriz* manipulador = new ManipulaMatriz();

			if(entrada.getSize() % 3 != 1){
				return 0;
			}
			Coordenada* coord1 = entrada.posicaoMem(0);
			DisplayFile* retas = new DisplayFile();
			for (int i = 1; i < entrada.getSize(); i+=3) {
				Coordenada* coord2 = entrada.posicaoMem(i);
				Coordenada* coord3 = entrada.posicaoMem(i+1);
				Coordenada* coord4 = entrada.posicaoMem(i+2);

				vector<Coordenada*> pontos;
				pontos.reserve(4);
				pontos.push_back(coord1);
				pontos.push_back(coord2);
				pontos.push_back(coord3);
				pontos.push_back(coord4);

				vector<Coordenada> geometria = manipulador->vetorGeometriaBezier(pontos);
				Coordenada* ponto1, *ponto2;

				double passo = 0;
				int numPassos = 50;
				double tamanhoPasso = 1.0/numPassos;

				for (int j = 0; j < numPassos+1; j++) {
					double x = geometria[3].getX()+passo*(geometria[2].getX()+passo*(geometria[1].getX()+geometria[0].getX()*passo));
					double y = geometria[3].getY()+passo*(geometria[2].getY()+passo*(geometria[1].getY()+geometria[0].getY()*passo));
					ponto1 = new Coordenada(x,y,1);
					passo += tamanhoPasso;

					x = geometria[3].getX()+passo*(geometria[2].getX()+passo*(geometria[1].getX()+geometria[0].getX()*passo));
					y = geometria[3].getY()+passo*(geometria[2].getY()+passo*(geometria[1].getY()+geometria[0].getY()*passo));
					ponto2 = new Coordenada(x,y,1);

					Objeto * obj = new Objeto(" ",Reta, false);
					obj->adiciona(*ponto1);
					obj->adiciona(*ponto2);
					retas->adiciona(obj);
				}
				coord1 = coord4;
				passo = 0;
				tamanhoPasso = 1.0/numPassos;
			}
			delete manipulador;
			return retas;
		}

	DisplayFile* getRetasSpline(ListaEnc<Coordenada> entrada){
			ManipulaMatriz* manipulador = new ManipulaMatriz();

			if(entrada.getSize() % 3 != 1){
				return 0;
			}

			Coordenada* coord1 = entrada.posicaoMem(0);
			DisplayFile* retas = new DisplayFile();
			for (int i = 1; i < entrada.getSize(); i+=3) {
				Coordenada* coord2 = entrada.posicaoMem(i);
				Coordenada* coord3 = entrada.posicaoMem(i+1);
				Coordenada* coord4 = entrada.posicaoMem(i+2);

				vector<Coordenada*> pontos;
				pontos.reserve(4);
				pontos.push_back(coord1);
				pontos.push_back(coord2);
				pontos.push_back(coord3);
				pontos.push_back(coord4);

				matrix Cx = manipulador->getSplineCoefficientsX(pontos);
				matrix Cy = manipulador->getSplineCoefficientsY(pontos);

				double _deltinha = 0.1;
				double _deltinha2 = pow(_deltinha,2);
				double _deltinha3 = pow(_deltinha,3);

				double f0X = Cx[0][3];
				double _deltaX = Cx[0][0]*_deltinha3 + Cx[0][1]*_deltinha2  + Cx[0][2]*_deltinha;
				double _delta2X = 6*Cx[0][0]*_deltinha3 + 2*Cx[0][1]*_deltinha2;
				double _delta3X = 6*Cx[0][0]*_deltinha3;

				cout << "deltaX" << _deltaX << endl;
				cout << "delta2X" << _delta2X << endl;
				cout << "delta3X" << _delta3X << endl;

				double f0Y = Cy[0][3];
				double _deltaY = Cy[0][0]*_deltinha3 + Cy[0][1]*_deltinha2  + Cy[0][2]*_deltinha;
				double _delta2Y = 6*Cy[0][0]*_deltinha3 + 2*Cy[0][1]*_deltinha2;
				double _delta3Y = 6*Cy[0][0]*_deltinha3;

				forwardDiff(retas, Cx[0][0],Cy[0][0],1,_deltaX, _deltaY, 1,_delta2X, _delta2Y, 1, _delta3X, _delta3Y, 1);
//				forwardDiff(retas, Cx[0][1],Cy[0][1],1,_deltaX, _deltaY, 1,_delta2X, _delta2Y, 1, _delta3X, _delta3Y, 1);
//				forwardDiff(retas, Cx[0][2],Cy[0][2],1,_deltaX, _deltaY, 1,_delta2X, _delta2Y, 1, _delta3X, _delta3Y, 1);
//				forwardDiff(retas, Cx[0][3],Cy[0][3],1,_deltaX, _deltaY, 1,_delta2X, _delta2Y, 1, _delta3X, _delta3Y, 1);
				coord1 = coord4;
			}
			cout << "retas" << endl;
			cout << retas->to_string() << endl;
			delete manipulador;
			return retas;
		}

	void forwardDiff(DisplayFile* disp, double x, double y, double z, double delta1X, double delta1Y, double delta1Z, double delta2X, double delta2Y, double delta2Z, double delta3X, double delta3Y, double delta3Z){
		double xOld = x;
		double yOld = y;
		double zOld = z;

		for (int i = 0; i < 10; ++i) {
			cout << "criou retinha" << endl;
			x += delta1X;
			delta1X += delta2X;
			delta2X += delta3X;

			y += delta1Y;
			delta1Y += delta2Y;
			delta2Y += delta3Y;

			z += delta1Z;
			delta1Z += delta2Z;
			delta2Z += delta3Z;

			Objeto * obj = new Objeto(" ", Reta, false);
			Coordenada coord1(xOld,yOld,zOld);
			Coordenada coord2(x,y,z);
			obj->adiciona(coord1);
			obj->adiciona(coord2);
			disp->adiciona(obj);

			xOld = x;
			yOld = y;
			zOld = z;
		}
	}

};

#endif /* INCLUDE_CURVA2D_HPP_ */
