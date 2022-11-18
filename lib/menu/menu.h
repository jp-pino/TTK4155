#ifndef __LIB_MENU_MENU_H__
#define __LIB_MENU_MENU_H__

#define MENU_MAX_OPTIONS 10

typedef struct {
  char* title;
  void (*f)(void);
} menu_option_t;

void MENU_add_option(menu_option_t *option);
void MENU_PrintCurrent();
void MENU_init();



#endif//__LIB_MENU_MENU_H__