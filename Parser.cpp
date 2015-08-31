/*
 * Parser.cpp
 *
 *  Created on: 30 Aug 2015
 *      Author: decio
 */

#include "include/Parser.h"

Parser::Parser() {
}

Parser::~Parser() {
}

void Parser::write(DisplayFile * displayFile) {
	ofstream file;
	file.open("world.obj");
	if(file.is_open()){
		for (int i = 0; i < displayFile->getSize(); ++i) {
			Objeto * obj = *displayFile->posicaoMem(i);
			file << obj->to_obj();
		}
	}
	file.close();
}

vector<string> split(string s, string delimiter) {
	vector<string> tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	tokens.push_back(s);
	return tokens;
}

DisplayFile * Parser::read(string path) {
	string line;
	ifstream file(path);
	DisplayFile * displayFile = new DisplayFile();
	if (file.is_open()) {
		while (getline(file, line)) {
			Objeto * obj = new Objeto();
			if(line.find("#")){
				obj->setNome(line.substr(1,line.size()));
			}else{
				vector<string> aux = split(line," ");
				Coordenada * coord = new Coordenada(atoi(aux[1].c_str()),atoi(aux[2].c_str()),atoi(aux[3].c_str()));
				obj->adiciona(*coord);
			}
			displayFile->adiciona(obj);
		}
		file.close();
	}
	return 0;
}

