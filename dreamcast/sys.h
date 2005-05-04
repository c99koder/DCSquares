#ifdef __cplusplus
extern "C" {
#endif

void c99_sys_init(char *title);
bool sys_render_begin();
void sys_render_finish();
void delay(float seconds);

#ifdef __cplusplus
}
#endif

