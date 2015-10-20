#include <math.h>
#include <iostream>
#include "../Coordenada.hpp"
using namespace std;

using matrix=vector<vector<double> >;
class ManipulaMatriz {

private:
	int fuck = 0;
	matrix translacaoBase, rotacaoBase, escalonamentoBase, translacaoBase3D, rotacaoBaseX, rotacaoBaseY, rotacaoBaseZ, escalonamentoBase3D, bezier, spline;

public:

	void initSpline() {
		spline[0][0] = -1.0 / 6.0;
		spline[1][0] = 1.0 / 2.0;
		spline[2][0] = -1.0 / 2.0;
		spline[3][0] = 1.0 / 6.0;
		spline[0][1] = 1.0 / 2.0;
		spline[1][1] = -1.0;
		spline[2][1] = 1.0 / 2.0;
		spline[2][2] = 1.0 / 2.0;
		spline[0][2] = -1.0 / 2.0;
		spline[1][2] = 0.0;
		spline[0][3] = 1.0 / 6.0;
		spline[1][3] = 2.0 / 3.0;
		spline[2][3] = 1.0 / 6.0;
	}

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
		initSpline();
		return multiplicaMatriz(spline, geometriaBSMatrix);
	}

	matrix getSplineCoefficientsY(vector<Coordenada*> xy){
		vector<Coordenada> geometriaBS = vetorGeometriaBSpline(xy);
		matrix geometriaBSMatrix(1,vector<double>(4,0));
		geometriaBSMatrix[0][0] = geometriaBS[0].getY();
		geometriaBSMatrix[0][1] = geometriaBS[1].getY();
		geometriaBSMatrix[0][2] = geometriaBS[2].getY();
		geometriaBSMatrix[0][3] = geometriaBS[3].getY();
		initSpline();
		return multiplicaMatriz(spline, geometriaBSMatrix);
	}

	vector<Coordenada> vetorGeometriaBSpline(vector<Coordenada*> xy){
		Coordenada a(xy[0]->getX(),xy[0]->getY(),1);
		Coordenada b(xy[1]->getX(),xy[1]->getY(),1);
		Coordenada c(xy[2]->getX(),xy[2]->getY(),1);
		Coordenada d(xy[3]->getX(),xy[3]->getY(),1);
		vector<Coordenada> geo = {a,b,c,d};
		return geo;
	}


	ManipulaMatriz():translacaoBase(3,vector<double>(3,0)), rotacaoBase(3,vector<double>(3,0)), escalonamentoBase(3,vector<double>(3,0)), bezier(4,vector<double>(4,0)), spline(4,vector<double>(4,0))
	,translacaoBase3D(4,vector<double>(4,0)), rotacaoBaseX(4,vector<double>(4,0)), rotacaoBaseY(4,vector<double>(4,0)), rotacaoBaseZ(4,vector<double>(4,0)), escalonamentoBase3D(4,vector<double>(4,0))
	{
		for(int i =0; i<3; i++){
			translacaoBase[i][i]=1.0;
			translacaoBase3D[i][i]=1.0;
		}
		rotacaoBase[2][2]=1.0;
		escalonamentoBase[2][2] =1.0;
		escalonamentoBase3D[3][3] = 1.0;

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

		initSpline();

		translacaoBase3D[3][3]=1.0;
		rotacaoBaseX[0][0]= 1;
		rotacaoBaseX[3][3]= 1;
		rotacaoBaseY[1][1]= 1;
		rotacaoBaseY[3][3]= 1;
		rotacaoBaseZ[2][2]= 1;
		rotacaoBaseZ[3][3]= 1;
	};
	~ManipulaMatriz(){};
	matrix getTranslacao(Coordenada deslocamento){
		translacaoBase[0][2] = deslocamento.getX();
		translacaoBase[1][2] = deslocamento.getY();

		return translacaoBase;
	}
	matrix getTranslacao3D(Coordenada deslocamento){
		translacaoBase3D[0][3] = deslocamento.getX();
		translacaoBase3D[1][3] = deslocamento.getY();
		translacaoBase3D[2][3] = deslocamento.getZ();
		return translacaoBase3D;
	}
	matrix getEscalonamento(Coordenada fator){
		escalonamentoBase[0][0] = fator.getX();
		escalonamentoBase[1][1] = fator.getY();
		return escalonamentoBase;
	}
	matrix getEscalonamento3D(Coordenada fator){
		escalonamentoBase3D[0][0] = fator.getX();
		escalonamentoBase3D[1][1] = fator.getY();
		escalonamentoBase3D[2][2] = fator.getZ();
		return escalonamentoBase3D;
	}
	matrix getRotacao(double theta){
		rotacaoBase[0][0] = cos(theta);
		rotacaoBase[0][1] = -sin(theta);
		rotacaoBase[1][0] = sin(theta);
		rotacaoBase[1][1] = cos(theta);
		return rotacaoBase;
	}
	matrix getRotacaoX(double theta){
		rotacaoBaseX[1][1] = cos(theta);
		rotacaoBaseX[1][2] = -sin(theta);
		rotacaoBaseX[2][1] = sin(theta);
		rotacaoBaseX[2][2] = cos(theta);
		return rotacaoBaseX;
	}
	matrix getRotacaoY(double theta){
		rotacaoBaseY[0][0] = cos(theta);
		rotacaoBaseY[2][0] = -sin(theta);
		rotacaoBaseY[0][2] = sin(theta);
		rotacaoBaseY[2][2] = cos(theta);
		return rotacaoBaseY;
	}
	matrix getRotacaoZ(double theta){
		rotacaoBaseZ[0][0] = cos(theta);
		rotacaoBaseZ[0][1] = -sin(theta);
		rotacaoBaseZ[1][0] = sin(theta);
		rotacaoBaseZ[1][1] = cos(theta);
		return rotacaoBaseZ;
	}
	matrix getRotacaoX(double sen, double cos){
		rotacaoBaseX[1][1] = cos;
		rotacaoBaseX[1][2] = -1*sen;
		rotacaoBaseX[2][1] = sen;
		rotacaoBaseX[2][2] = cos;
		return rotacaoBaseX;
	}
	matrix getRotacaoY(double sen, double cos){
		rotacaoBaseY[0][0] = cos;
		rotacaoBaseY[2][0] = -1*sen;
		rotacaoBaseY[0][2] = sen;
		rotacaoBaseY[2][2] = cos;
		return rotacaoBaseY;
	}
	matrix getRotacaoZ(double sen, double cos){
		rotacaoBaseZ[0][0] = cos;
		rotacaoBaseZ[0][1] = -1*sen;
		rotacaoBaseZ[1][0] = sen;
		rotacaoBaseZ[1][1] = cos;
		return rotacaoBaseZ;
	}
	matrix multiplicaMatriz(matrix a, matrix b){
		matrix result(b.size(), vector<double>(a[0].size(),0));
		for(int i=0;i<a[0].size();++i)
		{
			for(int j=0;j<b.size();++j)
			{
				for(int k=0;k<a.size();++k){
					result[j][i]+=(a[k][i]*b[j][k]);
				}
			}
		}
		return result;
	}

};
