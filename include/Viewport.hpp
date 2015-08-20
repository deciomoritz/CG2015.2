/*
 * Viewport.h
 *
 *  Created on: 19 Aug 2015
 *      Author: decio
 */

#ifndef VIEWPORT_H_
#define VIEWPORT_H_

#include "Objeto.hpp"
#include <cairo/cairo.h>

class Viewport {
public:
	Viewport();
	virtual ~Viewport();
	void desenha(cairo_t   *cr, Objeto obj);
};

#endif /* VIEWPORT_H_ */
