/*
 *  box.h
 *  TikiSquares
 *
 *  Created by Sam Steele on 12/25/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#ifndef _BOX_H
#define _BOX_H

class Box : public Drawable {
public:
	Box(float w, float h);
	~Box();
	
	void setBorder(Color c) { m_borderColor=c; m_border=true; };
	void setSize(float w, float h) { m_w=w; m_h=h; }
	void drawBox(float w, float h, Color c);
	void fadeIn();
	void fadeOut();
	
	//Overloaded functions
	void draw(ObjType list);
	
private:
	bool m_border;
	Color m_borderColor;
	float m_w, m_h;
	bool m_stopAnim;
	bool m_fadeIn,m_fadeOut;
};

#endif