#ifndef STATE_LIST_H_INCLUDED
#define STATE_LIST_H_INCLUDED

#include "const.h"
#include "seed.h"
#include "process.h"

extern Fl_Window state_list;
extern Fl_Box state_title;
extern Fl_Hold_Browser browser;
extern Fl_Button open_button;
extern Fl_Button delete_button;
extern Fl_Button close_button;
extern Fl_Input seed_input;
extern Fl_Float_Input goal_input;
extern Fl_Float_Input limit_input;
extern StatusBar status_bar;

extern seed cur_seed;
extern challenge_type type;

extern void error_out(const std::string& str, bool _exit);
extern void update_change_button(void);

void init_state_list(void);
void browser_cb(Fl_Widget* widget, void*);
void delete_cb(Fl_Widget* widget, void*);
void close_cb(Fl_Widget* widget, void*);
void open_cb(Fl_Widget* widget, void*);

extern void reset_changes_cb(Fl_Widget* widget, void*);

#endif // STATELIST_H_INCLUDED
