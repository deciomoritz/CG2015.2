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
	ListaEnc<Objeto> objetos();
public:
	DisplayFile();
	void inserirObjeto(Objeto obj);
	void removerObjeto(Objeto obj);
	virtual ~DisplayFile();
};

#endif /* DISPLAYFILE_HPP_ */
