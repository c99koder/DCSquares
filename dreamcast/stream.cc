#ifdef WIN32
#include <windows.h>
#endif
#include <stdlib.h>
#include "stream.h"
#include "screen.h"
#include "sys.h"
#include "input.h"

c99_output_stream dcout;
c99_input_stream dcin;

c99_output_stream::c99_output_stream() {
}

c99_output_stream& c99_output_stream::operator <<(const char *input) {
  c99_printf("%s",input);
  return *this;
}

c99_output_stream& c99_output_stream::operator <<(const char input) {
  c99_printf("%c",input);
  return *this;
}

c99_output_stream& c99_output_stream::operator <<(const int input) {
  c99_printf("%i",input);
  return *this;
}

c99_output_stream& c99_output_stream::operator <<(const float input) {
  c99_printf("%f",input);
  return *this;
}

c99_input_stream::c99_input_stream() {
}

c99_input_stream& c99_input_stream::operator >>(int &input) {
  char buf[256];
  get_string(buf);
  dcout << endl;
  input=atoi((char *)buf);
  return *this;
}

c99_input_stream& c99_input_stream::operator >>(float &input) {
#ifndef DREAMCAST
  char buf[256];
  get_string(buf);
  dcout << endl;
  input=atof((char *)buf);
#endif
  return *this;
}

c99_input_stream& c99_input_stream::operator >>(char &input) {
  char buf[256];
  get_string(buf);
  dcout << endl;
  input=buf[0];
  return *this;
}

c99_input_stream& c99_input_stream::operator >>(char *input) {
  get_string(input);
  dcout << endl;
  return *this;
}
