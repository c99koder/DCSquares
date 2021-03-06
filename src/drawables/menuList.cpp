/* menuList.cc - menu list drawable
 * Copyright (c) 2001-2005 Sam Steele
 *
 * This file is part of DCBlap.
 *
 * DCBlap is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2
 * as published by the Free Software Foundation.
 *
 * DCBlap is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA
 */

#include <Tiki/tiki.h>
#include <Tiki/gl.h>
#include <Tiki/anims/logxymover.h>
#include <Tiki/anims/alpharotate.h>

using namespace Tiki;
using namespace Tiki::GL;

#include "text.h"
#include "drawables/box.h"
#include "drawables/menuList.h"

menuList::menuList(Font *fnt) {
	m_bg=new Box(0,0);
	m_bg->setTint(Color(0.4,1,1,1));
	m_bg->setBorder(Color(0.4,0,0,0));
	subAdd(m_bg);
	m_bar=new Box(0,22);
	m_bar->setTint(Color(0.6,1,1,1));
	//m_bar->animAdd(new AlphaRotate(1.0f/60.0f,0.4));
	subAdd(m_bar);
	m_fnt=fnt;
	m_textColor=Color(1,1,1,1);
}

menuList::~menuList() {
	while(!m_itemList.empty()) {
		m_itemList.pop_back();
	}
}

void menuList::setSize(float w, float h) {
	m_w=w;
	m_h=h;
	m_bg->setSize(m_w,m_h);
	m_bg->setTranslate(Vector(0,0,-0.002));
	m_bar->setSize(m_w,22);
	m_bar->setTranslate(Vector(0,-m_h/2 - 11 + 22,-0.001));
}

void menuList::setTextColor(Color c) {
	m_textColor = c;
}

void menuList::addItem(char *text) {
	string *s=new string(text);
	m_itemList.push_back(*s);
}

void menuList::selectItem(int num) {
	m_currentItem=num;
	m_changed=true;
}

void menuList::draw(ObjType list) {
	std::vector<string>::iterator item_iter;
	int i=0;
	
	if(m_changed) {
		m_bar->animRemoveAll();
		m_bar->animAdd(new LogXYMover(0,-m_h/2 - 11 + ((m_currentItem+1)*22)));
		m_changed=false;
	}
	
	if(list==Trans) {
		Drawable::draw(list);	
		
		m_fnt->setSize(18);
		set_font_size(18);
		if(m_itemList.size() > 0) {
			for(item_iter = m_itemList.begin(); item_iter != m_itemList.end(); item_iter++) {
				m_fnt->setColor(getColor() * Color(0.4,0,0,0));
				m_fnt->draw(getPosition() + Vector(2,2,0) + Vector(-txt_width((char *)(*(item_iter)).c_str())/2,-m_h/2 - 6 + ((i+1)*22),0.1),*(item_iter));
				m_fnt->setColor(getColor() * m_textColor);
				m_fnt->draw(getPosition() + Vector(-txt_width((char *)(*(item_iter)).c_str())/2,-m_h/2 - 6 + ((i+1)*22),0.11),*(item_iter));
				i++;
			}
		}
	}
}
