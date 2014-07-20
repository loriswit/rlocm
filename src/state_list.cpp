#include "state_list.h"

void init_state_list(void)
{
    state_list.set_modal();
    state_list.set_tooltip_window();

    state_title.align(FL_ALIGN_CENTER|FL_ALIGN_INSIDE);
    browser.when(FL_WHEN_CHANGED);
    browser.callback(browser_cb);

    delete_button.shortcut(FL_Delete);
    delete_button.callback(delete_cb);

    close_button.shortcut(FL_Escape);
    close_button.callback(close_cb);

    open_button.shortcut(FL_Enter);
    open_button.callback(open_cb);

    open_button.deactivate();
    delete_button.deactivate();
    state_list.end();
}

void browser_cb(Fl_Widget* widget, void*)
{
    if(browser.value()){
        open_button.activate();
        delete_button.activate();
    }
    else{
        open_button.deactivate();
        delete_button.deactivate();
    }
}

void delete_cb(Fl_Widget* widget, void*)
{
    const std::string key = "RLOCM/" + ALT_LEVEL_LIST[type.level] + "_" + ALT_EVENT_LIST[type.event] + "_" + ALT_DIFFICULTY_LIST[type.difficulty];
    //std::cout << key << std::endl << browser.text(browser.value()) << std::endl;
    Fl_Preferences app(Fl_Preferences::USER, "RLM Team", key.c_str());
    app.deleteGroup(browser.text(browser.value()));
    if(browser.value()) browser.remove(browser.value());
}

void close_cb(Fl_Widget* widget, void*)
{
    open_button.deactivate();
    delete_button.deactivate();
    state_list.hide();
}

void open_cb(Fl_Widget* widget, void*)
{
    const std::string key = "RLOCM/" + ALT_LEVEL_LIST[type.level] + "_" + ALT_EVENT_LIST[type.event] + "_" + ALT_DIFFICULTY_LIST[type.difficulty];
    Fl_Preferences app(Fl_Preferences::USER, "RLM Team", key.c_str());

    Fl_Preferences group(app, browser.text(browser.value()));
    char *seed_str;
    group.get("seed", seed_str, "error");
    if(std::string(seed_str) == "error"){
        error_out("Could not load the state.", false);
        close_button.do_callback();
        return;
    }

    float goal, limit;
    group.get("goal", goal, -1);
    group.get("limit", limit, -1);

    seed_input.value(seed_str);
    seed_input.do_callback();

    std::ostringstream sgoal;
    sgoal << goal;
    goal_input.value(sgoal.str().c_str());
    goal_input.do_callback();

    std::ostringstream slimit;
    slimit << limit;
    limit_input.value(slimit.str().c_str());
    limit_input.do_callback();

    status_bar.set((std::string(browser.text(browser.value())) + " loaded!").c_str());

    close_button.do_callback();
}

