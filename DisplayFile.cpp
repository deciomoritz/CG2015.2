/*
 * DisplayFile.cpp
 *
 *  Created on: Aug 18, 2015
 *      Author: luiz
 */

#include "include/DisplayFile.hpp"
#include <string>

string DisplayFile::to_string(){
	string s = "";
	for (int i = 0; i < getSize(); ++i) {
		Objeto & obj = **posicaoMem(i);
		s += obj.to_string() + "\n";
	}
	return s;
}
