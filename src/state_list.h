#ifndef STATE_LIST_H_INCLUDED
#define STATE_LIST_H_INCLUDED

#include "const.h"
#include "process.h"

extern Fl_Window state_list;
extern Fl_Box state_title;
extern Fl_Hold_Browser browser;
extern Fl_Button open_button;
extern Fl_Button delete_button;
extern Fl_Button rename_button;

extern challenge_type type;

void init_state_list(void);
void browser_cb(Fl_Widget* widget, void*);
void delete_cb(Fl_Widget* widget, void*);
/*void open_cb(Fl_Widget* widget, void*);
void save_cb(Fl_Widget* widget, void*);
void up_cb(Fl_Widget* widget, void*);
void down_cb(Fl_Widget* widget, void*);*/

#endif // STATELIST_H_INCLUDED
