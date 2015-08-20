/*
 * Viewport.cpp
 *
 *  Created on: 19 Aug 2015
 *      Author: decio
 */

#include "include/Viewport.hpp"

Viewport::Viewport() {
	// TODO Auto-generated constructor stub

}

Viewport::~Viewport() {
	// TODO Auto-generated destructor stub
}

void Viewport::desenha(cairo_t   *cr, Objeto obj){
	cairo_set_source_rgb(cr, 0, 0, 0);
	cairo_set_line_width(cr, 0.5);

	cairo_move_to(cr, 0, 0);
	cairo_line_to(cr, 500, 500);

	cairo_stroke(cr);
}
