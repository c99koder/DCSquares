/*
 *  box.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/25/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/drawable.h>
#include <Tiki/tikitime.h>
#include <Tiki/texture.h>
#include <Tiki/plxcompat.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;

#include "box.h"
#include "squares.h"
#include "hud.h"

Box::Box(float w, float h) {
	m_border=false;
	m_w=w;
	m_h=w;
}

Box::~Box() {
};

void Box::drawBox(float w, float h, Color c) {
	const Vector & tv = getPosition();
	plx_vertex_t vert;
	
	Texture::deselect();
	
	vert.argb = c;
	vert.oargb = 0;
	
	vert.flags = PLX_VERT;
	vert.x = tv.x-w/2;
	vert.y = tv.y+h/2;
	vert.z = tv.z;
	plx_prim(&vert, sizeof(vert));
	
	vert.y = tv.y-h/2;
	plx_prim(&vert, sizeof(vert));
	
	vert.x = tv.x+w/2;
	vert.y = tv.y+h/2;
	plx_prim(&vert, sizeof(vert));
	
	vert.flags = PLX_VERT_EOS;
	vert.y = tv.y-h/2;
	plx_prim(&vert, sizeof(vert));
}

void Box::draw(ObjType list) {
	if(list==Trans) {
		if(m_border) drawBox(m_w+8,m_h+8,m_borderColor);
		drawBox(m_w,m_h,getColor());
	}
}