/*
 * DisplayFile.hpp
 *
 *  Created on: Aug 18, 2015
 *      Author: luiz
 */

#ifndef DISPLAYFILE_HPP_
#define DISPLAYFILE_HPP_
#include "Objeto.hpp"

class DisplayFile {
private:
	ListaEnc<Objeto> _objetos;
public:
	DisplayFile();
	ListaEnc<Objeto>* objetos();
	void desenha(DisplayFile* displayFile);
	virtual ~DisplayFile();
};

#endif /* DISPLAYFILE_HPP_ */
