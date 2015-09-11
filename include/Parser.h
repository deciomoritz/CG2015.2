/*
 * Parser.h
 *
 *  Created on: 30 Aug 2015
 *      Author: decio
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>
#include <iostream>
#include <fstream>

#include "DisplayFile.hpp"

using namespace std;

class Parser {
public:
	Parser();
	virtual ~Parser();

	void write(DisplayFile * displayFile, string path);
	DisplayFile * read(string path);
};

#endif /* PARSER_H_ */
