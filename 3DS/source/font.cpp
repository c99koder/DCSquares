#include <3ds.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <GL/gl.h>

GLuint LoadPNG(const u8 *png_data, const u32 size);

extern const u8 font_png[];
extern const u32 font_png_size;
int font_tex;

void font_init() {
    font_tex = LoadPNG(font_png, font_png_size);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

void draw_char(unsigned char a, float x, float y, float z, float size) {
    float rx,ry;
    rx=((a%16)*8);
    ry=((a/16)*8);
    glTexCoord2f(rx/128.0f,ry/128.0f);
    glVertex3f(x,y,z);
    glTexCoord2f((rx+8)/128.0f,ry/128.0f);
    glVertex3f(x+size,y,z);
    glTexCoord2f((rx+8)/128.0f,(ry+8)/128.0f);
    glVertex3f(x+size,y+size,z);
    glTexCoord2f(rx/128.0f,(ry+8)/128.0f);
    glVertex3f(x,y+size,z);
}

void draw_string(char const *text, float x, float y, float z, float size) {
    unsigned int i, len = strlen(text);
    
    glColor4f(1,1,1,0.1);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glBegin(GL_QUADS);
    for(i=0;i<len;i++) {
        draw_char(text[i],x+(i*size),y,z,size);
    }
    glEnd();
}

void draw_centered(char const *text, float y, float z, float size) {
    float x = 200 - (strlen(text) * (size / 2.0f));
    draw_string(text, x, y, z, size);
}