/*
 * DisplayFile.hpp
 *
 *  Created on: Aug 18, 2015
 *      Author: luiz
 */

#ifndef DISPLAYFILE_HPP_
#define DISPLAYFILE_HPP_
#include "Objeto.hpp"

class DisplayFile : ListaEnc<Objeto*>{
public:
	using ListaEnc<Objeto*>::getHead;
	using ListaEnc<Objeto*>::getSize;
	using ListaEnc<Objeto*>::adicionaNoInicio;
	using ListaEnc<Objeto*>::adiciona;
	using ListaEnc<Objeto*>::posicaoMem;
	DisplayFile(){
	}

	~DisplayFile(){
	}

	void desenha(DisplayFile* displayFile);


};

#endif /* DISPLAYFILE_HPP_ */
