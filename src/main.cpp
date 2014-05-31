#include "seed_manager.h"
using namespace std;

// HELPERS
void deactive_all(void);
void active_all(void);
string get_bundle_path(void);
void error_out(const string& str, bool _exit = false);
void warning_out(const string& str, bool _exit = false);
bool check_bundle(void);

// MAIN
void install_training_room_cb(Fl_Widget* widget, void*);
void load_challenge_cb(Fl_Widget* widget, void*);
void apply_changes_cb(Fl_Widget* widget, void*);
void reset_changes_cb(Fl_Widget* widget, void*);
void seed_input_cb(Fl_Widget* widget, void*);
void random_seed_cb(Fl_Widget* widget, void*);
void goal_input_cb(Fl_Widget* widget, void*);
void limit_input_cb(Fl_Widget* widget, void*);

Fl_Window window(WIDTH, HEIGHT, "Seed Manager");

Fl_Button load_button(10,10,120,30,"Load challenge");
Fl_Check_Button dojo_check(150,14,210,25,"Current challenge is the Dojo");

Fl_Box level_box(10,44,200,30,"Level:");
Fl_Box event_box(10,70,200,30,"Event:");
Fl_Box diff_box(190,70,200,30,"Difficulty:");

Fl_Input seed_input(53,104,100,30,"Seed:");
Fl_Button random_button(180,102,100,30,"Random seed");

Fl_Float_Input goal_input(53,140,80,30,"Goal: ");
Fl_Box goal_type(134,140,60,30);
Fl_Float_Input limit_input(258,140,80,30,"Time limit:");
Fl_Box limit_type(340,140,60,30);

Fl_Button change_button(10,186,110,30,"Apply changes");
Fl_Button reset_button(130,186,60,30,"Reset");
Fl_Check_Button training_check(210,190,160,25,"Enable training room");

#if DEV_MODE
Fl_Box dev_box(10,220,60,30,"Dev mode enabled");
Fl_Check_Button console_check(150,220,120,30,"Show console");
void console_show_cb(Fl_Widget* widget, void*)
{
    Fl_Check_Button *console_check = (Fl_Check_Button*)widget;
    if(console_check->value()){
        ShowWindow(GetConsoleWindow(), SW_SHOWNA);
        BringWindowToTop(GetActiveWindow());
    }
    else ShowWindow(GetConsoleWindow(), SW_HIDE);
}
#endif

StatusBar status_bar;
string bundle_path = BUNDLE_NAME;
bundle bund;
challenge_type type;

int main(int argc, char **argv)
{
    bool training_mode = check_bundle();
    clrscr();
    cout << "= SEED MANAGER =" << endl << "by Olybri (v" << VERSION_STR << ")" << endl;
    #if DEV_MODE
    textcolor(BROWN);
    cout << "Build number: #" << setw(4) << setfill('0') << BUILD << " (Developer mode)" << endl;
    textcolor(LIGHTGRAY);
    #endif
    cout << endl;

    // Window creation

    window.color(FL_DARK2);

    string str = "Seed Manager version " + VERSION_STR;
    status_bar.set(str.c_str());

    load_button.callback(load_challenge_cb);

    training_check.value(training_mode);
    training_check.callback(install_training_room_cb);

    level_box.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    event_box.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    diff_box.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    goal_type.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    limit_type.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);

    seed_input.maximum_size(14);
    seed_input.when(FL_WHEN_ENTER_KEY|FL_WHEN_RELEASE);
    //seed_input.when(FL_WHEN_ENTER_KEY|FL_WHEN_RELEASE);
    seed_input.callback(seed_input_cb);

    srand(time(NULL));
    random_button.callback(random_seed_cb);

    goal_input.when(FL_WHEN_CHANGED);
    goal_input.callback(goal_input_cb);

    limit_input.when(FL_WHEN_CHANGED);
    limit_input.callback(limit_input_cb);

    change_button.callback(apply_changes_cb);
    reset_button.callback(reset_changes_cb);

    #if DEV_MODE
    dev_box.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    dev_box.labelcolor(84);
    dev_box.labelfont(FL_BOLD);
    console_check.labelcolor(84);
    console_check.labelfont(FL_BOLD);
    console_check.callback(console_show_cb);
    console_check.value(true);
    #endif

    deactive_all();

    window.end();
    window.show(argc, argv);

    return(Fl::run());
}

/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////

// Callbacks
process proc;
seed cur_seed;
string cur_goal;
string cur_limit;

float str_to_float(const string& float_str)
{
    istringstream istr(float_str);
    float f;
    istr >> f;

    return f;
}

void update_change_button(void)
{
    if(goal_input.active()){
        if(seed_input.value() == cur_seed.to_str() && str_to_float(cur_goal) == str_to_float(goal_input.value()) && str_to_float(cur_limit) == str_to_float(limit_input.value())){
            change_button.deactivate();
            reset_button.deactivate();
        }
        else{
            change_button.activate();
            reset_button.activate();
        }
    }
    else{
        if(seed_input.value() == cur_seed.to_str()){
            change_button.deactivate();
            reset_button.deactivate();
        }
        else{
            change_button.activate();
            reset_button.activate();
        }
    }
}

void load_challenge_cb(Fl_Widget* widget, void*)
{
    load_button.deactivate();
    dojo_check.deactivate();
    training_check.deactivate();
    deactive_all();

    cout << endl;

    proc.open(PROCESS_NAME);
    if(!proc){
        status_bar.set("Error!");
        load_button.activate();
        training_check.activate();
        dojo_check.activate();
        error_out(proc.get_last_error());
        proc.reset();
        return;
    }

    int lvl;
    if(dojo_check.value()) lvl = DOJO;
    else lvl = UNKNOWN_LEVEL;
    cur_seed = proc.get_seed(lvl);
    if(!proc){
        status_bar.set("Error!");
        load_button.activate();
        training_check.activate();
        dojo_check.activate();
        error_out(proc.get_last_error());
        proc.reset();
        return;
    }
    seed_input.value(cur_seed.to_str().c_str());

    if(lvl != DOJO){
        type = proc.get_type(cur_seed);
        if(!proc){
            status_bar.set("Error!");
            load_button.activate();
            training_check.activate();
            dojo_check.activate();
            error_out(proc.get_last_error());
            proc.reset();
            return;
        }
    }
    else{
        type.level = DOJO;
        type.event = UNKNOWN_EVENT;
        type.difficulty = UNKNOWN_DIFFICULTY;
        type.distance = UNKNOWN_DISTANCE;
        type.limit = UNKNOWN_LIMIT;
    }
    status_bar.set("Success!");
    Fl::flush();

    load_button.activate();
    dojo_check.activate();
    training_check.activate();
    active_all();

    level_box.label(LEVEL_LIST[type.level].c_str());

    if(type.level != DOJO){
        event_box.label(EVENT_LIST[type.event].c_str());
        diff_box.label(DIFFICULTY_LIST[type.difficulty].c_str());
    }
    else{
        event_box.label(EVENT_LIST[4].c_str());
        diff_box.label(DIFFICULTY_LIST[2].c_str());
    }

    if(type.event == DISTANCE){
        goal_input.deactivate();
        limit_input.deactivate();
    }
    else{
    if(type.distance == UNKNOWN_DISTANCE) goal_input.value("Unknown");
    else{
            ostringstream goal;
            goal << type.distance;
            goal_input.value(goal.str().c_str());
            cur_goal = goal.str();
            if(type.event == SPEED) goal_type.label("m");
            else goal_type.label("lums");
            Fl::flush();
        }
        if(type.limit == UNKNOWN_LIMIT) limit_input.value("Unknown");
        else{
            ostringstream limit;
            limit << type.limit;
            limit_input.value(limit.str().c_str());
            cur_limit = limit.str();
            limit_type.label("sec");
            Fl::flush();
        }
    }

    if(type.level == DOJO){
        event_box.deactivate();
        diff_box.deactivate();
        goal_input.deactivate();
        limit_input.deactivate();
    }

    update_change_button();
}

void install_training_room_cb(Fl_Widget* widget, void*)
{
    cout << endl;

    bund.open(bundle_path);
    if(!bund){
        status_bar.set("Error!");
        string error = bund.get_last_error();
        error_out(error);
        return;
    }

    if(training_check.value()){
        status_bar.set("Installing the training room...");
        Fl::flush();
        bund.install_training_room();
    }
    else{
        status_bar.set("Uninstalling the training room...");
        Fl::flush();
        bund.uninstall_training_room();
    }
    training_check.value(bund.check_training());
    bund.close();

    if(!bund){
        status_bar.set("Error!");
        error_out(bund.get_last_error());
    }
    else status_bar.set("Success!");
}

void apply_changes_cb(Fl_Widget* widget, void*)
{
    status_bar.set("Applying changes...");
    Fl::flush();
    if(!training_check.value()){
        #if !DEV_MODE
        warning_out("You must be playing in the training room to apply changes!");
        status_bar.set("Error!");
        return;
        #else
        textcolor(CYAN);
        cout << endl << "WARNING: You're not playing in the training room!";
        textcolor(LIGHTGRAY);
        #endif
    }

    bool seed_changed = false;
    if(seed_input.value() != cur_seed.to_str()){
        cout << endl;
        seed new_seed(seed_input.value());
        if(type.level != DOJO) proc.change_seed(new_seed);
        else proc.change_seed_dojo(cur_seed, new_seed);
        if(!proc){
            error_out(proc.get_last_error());
            status_bar.set("Error!");
            return;
        }
        cur_seed = new_seed;
        seed_input.textcolor(FL_BLACK);
        seed_input.redraw();
        cout << "You need to restart the challenge to apply the changes." << endl;
        seed_changed = true;
    }

    if(goal_input.active() && str_to_float(cur_goal) != str_to_float(goal_input.value())){
        cout << endl;
        float distance = str_to_float(goal_input.value());
        proc.change_distance(distance);
        if(!proc){
            error_out(proc.get_last_error());
            status_bar.set("Error!");
            return;
        }
        type.distance = distance;
        cur_goal = goal_input.value();
        goal_input.textcolor(FL_BLACK);
        goal_input.redraw();
    }

    if(limit_input.active() && str_to_float(cur_limit) != str_to_float(limit_input.value())){
        cout << endl;
        float limit = str_to_float(limit_input.value());
        proc.change_limit(limit);
        if(!proc){
            error_out(proc.get_last_error());
            status_bar.set("Error!");
            return;
        }
        type.limit = limit;
        cur_limit = limit_input.value();
        limit_input.textcolor(FL_BLACK);
        limit_input.redraw();
    }

    if(seed_changed) status_bar.set("Changes saved! You need to restart the challenge.");
    else status_bar.set("Changes saved!");
    update_change_button();
}

void reset_changes_cb(Fl_Widget* widget, void*)
{
    seed_input.value(cur_seed.to_str().c_str());
    seed_input.textcolor(FL_BLACK);
    seed_input.redraw();

    if(goal_input.active()){
        goal_input.value(cur_goal.c_str());
        goal_input.textcolor(FL_BLACK);
        goal_input.redraw();
    }

    if(limit_input.active()){
        limit_input.value(cur_limit.c_str());
        limit_input.textcolor(FL_BLACK);
        limit_input.redraw();
    }

    update_change_button();
}

void seed_input_cb(Fl_Widget* widget, void*)
{
    string value;
    for(int i=0; i<strlen(seed_input.value()); i++)
        if((seed_input.value()[i] < '0' || seed_input.value()[i] > '9') && (toupper(seed_input.value()[i]) < 'A' || toupper(seed_input.value()[i]) > 'F'))
            continue;
        else value += toupper(seed_input.value()[i]);

    while(value.size() < 8) value = "0" + value;

    string seed_str;
    for(int i=0; i<8; i++){
        seed_str += value[i];
        if(i == 1 || i == 3 || i == 5) seed_str += ' ';
    }
    seed_input.value(seed_str.c_str());
    if(seed_str != cur_seed.to_str()) seed_input.textcolor(FL_RED);
    else seed_input.textcolor(FL_BLACK);
    update_change_button();
    status_bar.clear();
}

void random_seed_cb(Fl_Widget* widget, void*)
{
    uint8_t c[4];
    for(int i=0; i<4; i++) c[i] = rand() % 0x100;
    if(c[3] == 0) c[3]++;
    seed_input.value(seed(c[0], c[1], c[2], c[3]).to_str().c_str());
    if(seed_input.value() != cur_seed.to_str()) seed_input.textcolor(FL_RED);
    else seed_input.textcolor(FL_BLACK);
    update_change_button();
    status_bar.clear();
}

void goal_input_cb(Fl_Widget* widget, void*)
{
    if(str_to_float(cur_goal) != str_to_float(goal_input.value())) goal_input.textcolor(FL_RED);
    else goal_input.textcolor(FL_BLACK);
    goal_input.redraw();
    update_change_button();
    status_bar.clear();
}

void limit_input_cb(Fl_Widget* widget, void*)
{
    if(str_to_float(cur_limit) != str_to_float(limit_input.value())) limit_input.textcolor(FL_RED);
    else limit_input.textcolor(FL_BLACK);
    limit_input.redraw();
    update_change_button();
    status_bar.clear();
}


/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////

void deactive_all(void)
{
    level_box.deactivate();
    event_box.deactivate();
    seed_input.deactivate();
    random_button.deactivate();
    diff_box.deactivate();
    goal_input.deactivate();
    goal_type.deactivate();
    limit_input.deactivate();
    limit_type.deactivate();
    change_button.deactivate();
    reset_button.deactivate();

    level_box.label("Level:");
    event_box.label("Event:");
    seed_input.value("");
    diff_box.label("");
    goal_input.value("");
    goal_type.label("");
    limit_input.value("");
    limit_type.label("");
}

void active_all(void)
{
    level_box.activate();
    event_box.activate();
    seed_input.activate();
    random_button.activate();
    diff_box.activate();
    goal_input.activate();
    goal_type.activate();
    limit_input.activate();
    limit_type.activate();
    change_button.activate();
    reset_button.activate();
}

bool check_bundle(void)
{
    bund.open(bundle_path);
    if(!bund){
        if(bund.is_open) error_out(bund.get_last_error(), true);
        string error = bund.get_last_error();
        bundle_path = BUNDLE_NAME_2;
        bund.open(bundle_path);
        if(!bund){
            if(bund.is_open) error_out(bund.get_last_error(), true);

            bundle_path = "";
            ifstream ifs("Bundle_PC.path", ios::in|ios::binary);
            if(ifs){
                string path;
                ifs.seekg(0, ifs.end);
                int length = ifs.tellg();
                ifs.seekg(0, ifs.beg);
                for(int i=0; i<length; i++) path += ifs.get();
                if(path[0] == '\xB6' && path[1] == '\xA5' && path[2] == '\xC1'){
                    path.erase(0,3);
                    for(unsigned int i=0; i<path.size(); i++) path[i] = char(path[i]-PW[0]);
                    bundle_path = path;
                }
                ifs.close();
                bund.open(bundle_path);
                if(!bund) error_out(bund.get_last_error(), true);
            }

            if(!bundle_path.size()){
                cout << "Cannot found the Bundle_PC..." << endl << "You will need to specify the path of the bundle." << endl;
                const string pass = fl_password("Cannot found the Bundle_PC...\nYou will need to specify the path of the bundle.\nEnter password:");
                bool correct = false;

                if(pass.size() != PW.size()-1) correct = false;
                else for(unsigned int i=0; i<PW.size()-1; i++)
                    if(pass[i] != char(PW[i+1]-PW[0])){
                        correct = false;
                        break;
                    }
                    else correct = true;

                if(!correct) error_out("Wrong password!", true);


                char current_dir[FILENAME_MAX];
                getcwd(current_dir, sizeof(current_dir));

                bundle_path = get_bundle_path();
                if(!bundle_path.size()) return 0;
                bund.open(bundle_path);
                if(!bund) error_out(bund.get_last_error(), true);
                ofstream ofs((string(current_dir) + "\\Bundle_PC.path").c_str(), ios::binary);
                if(ofs){
                    ofs.put(0xB6);
                    ofs.put(0xA5);
                    ofs.put(0xC1);
                    for(unsigned int i=0; i<bundle_path.size(); i++) ofs.put(char(bundle_path[i]+PW[0]));
                    ofs.close();
                }
            }
        }
    }
    bool training_mode = bund.check_training();
    bund.close();

    return training_mode;
}

string get_bundle_path(void)
{
    Fl_Native_File_Chooser native;
    native.title("Where is Bundle_PC.ipk?");
    native.type(Fl_Native_File_Chooser::BROWSE_FILE);
    native.filter("Itsy Package\t*.ipk\nAll\t*.*}\n");
    native.preset_file("Bundle_PC.ipk");

    switch(native.show())
    {
        case -1:	// ERROR
        case  1: return ""; break;		// CANCEL
        default: 								// PICKED FILE
            if(native.filename()) return native.filename();
            else return "";
    }

    return "";
}

void error_out(const string& str, bool _exit)
{
    textcolor(LIGHTRED);
    cout << endl << "ERROR: " << str << endl;
    textcolor(LIGHTGRAY);
    MessageBox(0, str.c_str(), "Error", MB_ICONERROR);
    if(_exit) exit(0);
}

void warning_out(const string& str, bool _exit)
{
    textcolor(CYAN);
    cout << endl << "WARNING: " << str << endl;
    textcolor(LIGHTGRAY);
    MessageBox(0, str.c_str(), "Warning", MB_ICONWARNING);
    if(_exit) exit(0);
}
