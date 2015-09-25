/*
 * Curva2D.hpp
 *
 *  Created on: 24 Sep 2015
 *      Author: decio
 */

#ifndef INCLUDE_CURVA2D_HPP_
#define INCLUDE_CURVA2D_HPP_

class Curva2D : public Objeto{

private:

public:

	DisplayFile* getRetas(ListaEnc<Coordenada> entrada){
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

			vector<Coordenada> geometria = manipulador->vetorGeometria(pontos);
//			cout << "pegou o vetor" << endl;
			Coordenada* ponto1, *ponto2;

			double passo = 0;
			int numPassos = 50;
			double tamanhoPasso = 1.0/numPassos;

//			cout << "vetor de geometria: " << geometria[0].getX() << " "  << geometria[1].getX() << " " << geometria[2].getX() << " " << geometria[3].getX() << endl;
//			cout << "vetor de geometria: " << geometria[0].getY() << " "  << geometria[1].getY() << " " << geometria[2].getY() << " " << geometria[3].getY() << endl;
			for (int j = 0; j < numPassos+1; j+=2) {
//				cout << "passo " << passo;
//				cout << "tamanho passo " << tamanhoPasso;
				double x = geometria[3].getX()+passo*(geometria[2].getX()+passo*(geometria[1].getX()+geometria[0].getX()*passo));
				double y = geometria[3].getY()+passo*(geometria[2].getY()+passo*(geometria[1].getY()+geometria[0].getY()*passo));
//				cout << x << " " << y << endl;
				ponto1 = new Coordenada(x,y,1);
				passo += tamanhoPasso;

				x = geometria[3].getX()+passo*(geometria[2].getX()+passo*(geometria[1].getX()+geometria[0].getX()*passo));
				y = geometria[3].getY()+passo*(geometria[2].getY()+passo*(geometria[1].getY()+geometria[0].getY()*passo));
				ponto2 = new Coordenada(x,y,1);
//				passo += tamanhoPasso;

				Objeto * obj = new Objeto(" ",Reta, false);
				obj->adiciona(*ponto1);
				obj->adiciona(*ponto2);
				retas->adiciona(obj);
			}
			coord1 = coord4;
		}
//		cout << "fucking retas" << endl;
//		cout << retas->to_string() << endl;
		delete manipulador;
		return retas;
	}

};

#endif /* INCLUDE_CURVA2D_HPP_ */
