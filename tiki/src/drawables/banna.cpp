/*
 *  banna.cpp
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/20/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/drawable.h>
#include <Tiki/plxcompat.h>
#include <Tiki/tikitime.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;

#include "banna.h"
#include "squares.h"
#include "theme.h"
#include "texture.h"

void Banna::setSize(float w, float h) {
	m_w=w;
	m_h=h;
}

void Banna::draw(ObjType list) {
	Color c=getColor();
	const Vector & tv = getPosition();
	plx_vertex_t vert;
	
	if(list==Trans) {
		switch_tex(m_tex);
		
		vert.argb = c;
		vert.oargb = 0;
		
		vert.flags = PLX_VERT;
		vert.x = tv.x-m_w/2;
		vert.y = tv.y+m_h/2;
		vert.z = tv.z;
		plx_prim(&vert, sizeof(vert));
		
		vert.y = tv.y-m_h/2;
		plx_prim(&vert, sizeof(vert));
		
		vert.x = tv.x+m_w/2;
		vert.y = tv.y+m_h/2;
		plx_prim(&vert, sizeof(vert));
		
		vert.flags = PLX_VERT_EOS;
		vert.y = tv.y-m_h/2;
		plx_prim(&vert, sizeof(vert));
	}
}

