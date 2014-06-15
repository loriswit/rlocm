#include "state_list.h"

void init_state_list(void)
{
    //state_list.set_modal();
    //state_list.set_menu_window();
    state_list.set_tooltip_window();

    state_title.align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    browser.when(FL_WHEN_CHANGED);
    browser.callback(browser_cb);

    delete_button.shortcut(FL_Delete);
    delete_button.callback(delete_cb);

    open_button.deactivate();
    delete_button.deactivate();
    rename_button.deactivate();
    state_list.end();
    //state_list.show();
}

void browser_cb(Fl_Widget* widget, void*)
{
    if(browser.value()){
        open_button.activate();
        delete_button.activate();
        rename_button.activate();
    }
    else{
        open_button.deactivate();
        delete_button.deactivate();
        rename_button.deactivate();
    }
}

void delete_cb(Fl_Widget* widget, void*)
{
    const std::string key = "RLOCM/" + ALT_LEVEL_LIST[type.level] + "_" + ALT_EVENT_LIST[type.event] + "_" + ALT_DIFFICULTY_LIST[type.difficulty];
    std::cout << key << std::endl << browser.text(browser.value()) << std::endl;
    Fl_Preferences app(Fl_Preferences::USER, "RLM Team", key.c_str());
    app.deleteGroup(browser.text(browser.value()));
    if(browser.value()) browser.remove(browser.value());
}

/*void open_cb(Fl_Widget* widget, void*)
{*/

