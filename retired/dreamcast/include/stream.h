class c99_output_stream {
  public:
  c99_output_stream();
	c99_output_stream& operator <<(const char *input);
	c99_output_stream& operator <<(char input);
	c99_output_stream& operator <<(const int input);
	c99_output_stream& operator <<(const float input);
};

class c99_input_stream {
  public:
  c99_input_stream();
	c99_input_stream& operator >>(char *input);
	c99_input_stream& operator >>(char &input);
	c99_input_stream& operator >>(int &input);
	c99_input_stream& operator >>(float &input);
};

extern c99_output_stream dcout;
extern c99_input_stream dcin;
#define endl "\n"
#define cout dcout
#define cin dcin
