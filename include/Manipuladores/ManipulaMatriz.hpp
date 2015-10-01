#include <math.h>
#include <iostream>
#include "../Coordenada.hpp"
using namespace std;

using matrix=vector<vector<double> >;
class ManipulaMatriz {

private:
	int fuck = 0;
	matrix translacaoBase, rotacaoBase, escalonamentoBase, bezier, spline, splineInverse;

public:
	void printaMatriz(matrix m){
		for (int j = 0; j < m[0].size(); j++) {
			for (int k = 0; k < m.size(); k++) {
				cout << m[k][j]<<"  ";
			}
			cout <<"\n";
		}
	}

	void printaBezier(){
		printaMatriz(bezier);
	}

	vector<Coordenada> vetorGeometriaBezier(vector<Coordenada*> xy){
		vector<vector<double>> x(1,vector<double>(4,0));
		x[0][0] = xy[0]->getX();
		x[0][1] = xy[1]->getX();
		x[0][2] = xy[2]->getX();
		x[0][3] = xy[3]->getX();
		vector<vector<double>> y(1,vector<double>(4,0));
		y[0][0] = xy[0]->getY();
		y[0][1] = xy[1]->getY();
		y[0][2] = xy[2]->getY();
		y[0][3] = xy[3]->getY();

		matrix vetorX = multiplicaMatriz(bezier,x);
		matrix vetorY = multiplicaMatriz(bezier,y);

		Coordenada a(vetorX[0][0],vetorY[0][0],1);
		Coordenada b(vetorX[0][1],vetorY[0][1],1);
		Coordenada c(vetorX[0][2],vetorY[0][2],1);
		Coordenada d(vetorX[0][3],vetorY[0][3],1);

		vector<Coordenada> geo = {a,b,c,d};
		return geo;
	}

	matrix getSplineCoefficientsX(vector<Coordenada*> xy){
		vector<Coordenada> geometriaBS = vetorGeometriaBSpline(xy);
		matrix geometriaBSMatrix(1,vector<double>(4,0));
		geometriaBSMatrix[0][0] = geometriaBS[0].getX();
		geometriaBSMatrix[0][1] = geometriaBS[1].getX();
		geometriaBSMatrix[0][2] = geometriaBS[2].getX();
		geometriaBSMatrix[0][3] = geometriaBS[3].getX();
		return multiplicaMatriz(splineInverse, geometriaBSMatrix);
	}

	matrix getSplineCoefficientsY(vector<Coordenada*> xy){
		vector<Coordenada> geometriaBS = vetorGeometriaBSpline(xy);
		matrix geometriaBSMatrix(1,vector<double>(4,0));
		geometriaBSMatrix[0][0] = geometriaBS[0].getY();
		geometriaBSMatrix[0][1] = geometriaBS[1].getY();
		geometriaBSMatrix[0][2] = geometriaBS[2].getY();
		geometriaBSMatrix[0][3] = geometriaBS[3].getY();
		return multiplicaMatriz(splineInverse, geometriaBSMatrix);
	}

	vector<Coordenada> vetorGeometriaBSpline(vector<Coordenada*> xy){
//		cout << "chegou" << endl;
		vector<vector<double>> x(1,vector<double>(4,0));
		x[0][0] = xy[0]->getX();
		x[0][1] = xy[1]->getX();
		x[0][2] = xy[2]->getX();
		x[0][3] = xy[3]->getX();
		vector<vector<double>> y(1,vector<double>(4,0));
		y[0][0] = xy[0]->getY();
		y[0][1] = xy[1]->getY();
		y[0][2] = xy[2]->getY();
		y[0][3] = xy[3]->getY();
//
//		cout << "vetores de pontos " << endl;
//		printaMatriz(x);
//		printaMatriz(y);
//		printaBezier();

		matrix vetorX = multiplicaMatriz(spline,x);
		matrix vetorY = multiplicaMatriz(spline,y);
//
//		cout << "resultado " << endl;
//		printaMatriz(vetorX);
//		printaMatriz(vetorY);
//		cout << "--------------------" << endl;
////
////		cout << "v geometria x" << endl;
////		printaMatriz(vetorX);
////		cout << "v geometria y" << endl;
////		printaMatriz(vetorY);
////
////		cout << endl << endl;
//
//		cout << vetorX[0][0]<<" "<<vetorY[0][0] << endl;
//		cout << vetorX[0][1]<<" "<<vetorY[0][1] << endl;
//		cout << vetorX[0][2]<<" "<<vetorY[0][2] << endl;
//		cout << vetorX[0][3]<<" "<<vetorY[0][3] << endl;

		Coordenada a(vetorX[0][0],vetorY[0][0],1);
		Coordenada b(vetorX[0][1],vetorY[0][1],1);
		Coordenada c(vetorX[0][2],vetorY[0][2],1);
		Coordenada d(vetorX[0][3],vetorY[0][3],1);

		vector<Coordenada> geo = {a,b,c,d};
		return geo;
	}


	ManipulaMatriz():translacaoBase(3,vector<double>(3,0)), rotacaoBase(3,vector<double>(3,0)), escalonamentoBase(3,vector<double>(3,0)), bezier(4,vector<double>(4,0)), spline(4,vector<double>(4,0)), splineInverse(4,vector<double>(4,0)){
		for(int i =0; i<3; i++){
			translacaoBase[i][i]=1.0;

		}
		rotacaoBase[2][2]=1.0;
		escalonamentoBase[2][2] =1.0;

		bezier[0][0] = -1;
		bezier[1][0] = 3;
		bezier[2][0] = -3;
		bezier[3][0] = 1;

		bezier[0][1] = 3;
		bezier[1][1] = -6;
		bezier[2][1] = 3;

		bezier[0][2] = -3;
		bezier[1][2] = 3;

		bezier[0][3] = 1;

		spline[0][0] = -1/6;
		spline[1][0] = 3/6;
		spline[2][0] = -3/6;
		spline[3][0] = 1/6;

		spline[0][1] = 3/6;
		spline[1][1] = -6/6;
		spline[2][1] = 3/6;

		spline[0][2] = -3/6;
		spline[1][2] = 3/6;

		spline[0][3] = 1/6;
		spline[1][3] = 4/6;
		spline[2][3] = 1/6;
		spline[3][3] = 0/6;

		splineInverse[0][0] = 0;
		splineInverse[1][0] = (1/9)/6;
		splineInverse[2][0] = -1;
		splineInverse[3][0] = 1;

		splineInverse[0][1] = 0;
		splineInverse[1][1] = -1/3;
		splineInverse[2][1] = (1/9)/6;
		splineInverse[3][1] = (11/18)/6;

		splineInverse[0][2] = 1/6;
		splineInverse[1][2] = (11/18)/6;
		splineInverse[2][2] = 1;
		splineInverse[3][2] = 1;

		splineInverse[0][3] = 1/6;
		splineInverse[1][3] = (11/18)/6;
		splineInverse[2][3] = (1/3)/6;
		splineInverse[3][3] = 1;
	};
	~ManipulaMatriz(){};
	matrix getTranslacao(Coordenada deslocamento){
		translacaoBase[0][2] = deslocamento.getX();
		translacaoBase[1][2] = deslocamento.getY();

		return translacaoBase;
	}
	matrix getEscalonamento(Coordenada fator){
		escalonamentoBase[0][0] = fator.getX();
		escalonamentoBase[1][1] = fator.getY();
		return escalonamentoBase;
	}
	matrix getRotacao(double theta){
		rotacaoBase[0][0] = cos(theta);
		rotacaoBase[0][1] = -sin(theta);
		rotacaoBase[1][0] = sin(theta);
		rotacaoBase[1][1] = cos(theta);
		return rotacaoBase;
	}
	matrix multiplicaMatriz(matrix a, matrix b){
		matrix result(b.size(), vector<double>(a[0].size(),0));
		for(int i=0;i<a.size();++i)
		{
			for(int j=0;j<b.size();++j)
			{
				for(int k=0;k<a.size();++k)
					result[j][i]+=(a[k][i]*b[j][k]);
			}
		}
		return result;
	}
};
