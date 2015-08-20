/*
 * DisplayFile.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: luiz
 */

#include "include/DisplayFile.hpp"

DisplayFile::DisplayFile() {
}

DisplayFile::~DisplayFile() {
}

void DisplayFile::desenha(DisplayFile* displayFile){

}

ListaEnc<Objeto>* DisplayFile::objetos(){
	return &_objetos;
}
