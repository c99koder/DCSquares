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

#include <Tiki/anims/tintfader.h>

using namespace Tiki;
using namespace Tiki::GL;
using namespace Tiki::GL::Plxcompat;

#include "drawables/box.h"
#include "squares.h"
#include "hud.h"

Box::Box(float w, float h) {
	m_border=false;
	m_w=w;
	m_h=h;
	m_stopAnim=false;
	m_fadeIn=m_fadeOut=false;
}

void Box::fadeIn() {
	m_stopAnim=true;
	m_fadeIn=true;
}

void Box::fadeOut() {
	m_stopAnim=true;
	m_fadeOut=true;
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
	if(m_stopAnim) {
		animRemoveAll();
		m_stopAnim=false;
	}
	
	if(m_fadeIn) {
		m_fadeIn=false;
		setTint(Color(1,1,1,1));
		animAdd(new TintFader(Color(0,1,1,1),Color(-1.0f/30.0f,0,0,0)));
	}

	if(m_fadeOut) {
		m_fadeOut=false;
		setTint(Color(0,1,1,1));
		animAdd(new TintFader(Color(1,1,1,1),Color(1.0f/30.0f,0,0,0)));
	}
	
	if(list==Trans) {
		if(m_border) drawBox(m_w+8,m_h+8,m_borderColor);
		drawBox(m_w,m_h,getColor());
	}
}
