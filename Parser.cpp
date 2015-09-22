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

void Parser::write(DisplayFile * displayFile, string path) {
	ofstream file;
	file.open(path.c_str());
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
		Objeto * obj;
		while (getline(file, line)) {
			Coordenada * coord;
			if(!line.find("#")){
				obj = new Objeto();
				string nome = line.erase(0,1);
				nome = split(nome," ").front();
				obj->setNome(nome);
				continue;
			}else if(!line.find("v")){
				vector<string> aux = split(line," ");
				Tipo tipo;
				if(aux.size() == 3){
					tipo = Ponto;
				}else if(aux.size() == 5){
					tipo = Reta;
				}else{
					tipo = Poligono;
				}
				obj->setTipo(tipo);
				if(tipo != Ponto){
					for (int i = 1; i < aux.size() - 1; i += 2) {
						coord = new Coordenada(atoi(aux[i].c_str()), atoi(aux[i + 1].c_str()), 1);
						obj->adiciona(*coord);
					}
				}else{
					coord = new Coordenada(atoi(aux[1].c_str()), atoi(aux[2].c_str()), 1);
					obj->adiciona(*coord);
				}
			}else{
				continue;
			}
			displayFile->adiciona(obj);
		}
		file.close();
	}
	displayFile->retira();
	return displayFile;
}

