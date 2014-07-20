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
void install_training_room_menu_cb(Fl_Widget* widget, void*);
void load_challenge_cb(Fl_Widget* widget, void*);
void apply_changes_cb(Fl_Widget* widget, void*);
void reset_changes_cb(Fl_Widget* widget, void*);
void seed_input_cb(Fl_Widget* widget, void*);
void random_seed_cb(Fl_Widget* widget, void*);
void goal_input_cb(Fl_Widget* widget, void*);
void limit_input_cb(Fl_Widget* widget, void*);
void quit_cb(Fl_Widget* widget, void*){ exit(0); }
void null_cb(Fl_Widget* widget, void*);
void save_state_cb(Fl_Widget* widget, void*);
void open_state_cb(Fl_Widget* widget, void*);
void copy_state_cb(Fl_Widget* widget, void*);
void paste_state_cb(Fl_Widget* widget, void*);
void about_cb(Fl_Widget* widget, void*);
void help_cb(Fl_Widget* widget, void*);
#include "state_list.h"

Fl_Window window(WIDTH, HEIGHT, "RL Offline Challenge Manager");

Fl_Menu_Bar menu(0,0,WIDTH+1,MENU_Y);

Fl_Button load_button(10,10+MENU_Y,120,30,"Load challenge");
Fl_Check_Button training_check(150,12+MENU_Y,160,25,"Enable training room");

Fl_Box level_box(10,44+MENU_Y,200,30,"Level:");
Fl_Box event_box(10,70+MENU_Y,200,30,"Event:");
Fl_Box diff_box(215,70+MENU_Y,200,30,"Difficulty:");

Fl_Input seed_input(53,104+MENU_Y,100,30,"Seed:");
Fl_Button random_button(180,104+MENU_Y,100,30,"Random seed");

Fl_Float_Input goal_input(53,140+MENU_Y,80,30,"Goal: ");
Fl_Box goal_type(134,140+MENU_Y,60,30);
Fl_Float_Input  limit_input(258,140+MENU_Y,80,30,"Score limit:");
Fl_Box limit_type(340,140+MENU_Y,60,30);

Fl_Button change_button(70,190+MENU_Y,120,34,"Apply changes");
Fl_Button reset_button(210,190+MENU_Y,80,34,"Reset");

StatusBar status_bar;

#if DEV_MODE
Fl_Box dev_box(10,234+MENU_Y,60,30,"Dev mode enabled");
Fl_Check_Button console_check(170,234+MENU_Y,120,30,"Show console");
void console_show_cb(Fl_Widget* widget, void*)
{
    if(console_check.value()){
        ShowWindow(GetConsoleWindow(), SW_SHOWNA);
        BringWindowToTop(GetActiveWindow());
    }
    else ShowWindow(GetConsoleWindow(), SW_HIDE);
}
#endif

Fl_Window state_list(180,250,"Open");
Fl_Box state_title(0,0,180,20,"Rules for this level type:");
Fl_Hold_Browser browser(0,20,180,200);
Fl_Button open_button(0,220,60,30,"Open");
Fl_Button delete_button(60,220,60,30,"Delete");
Fl_Button close_button(120,220,60,30,"Close");

string bundle_path = BUNDLE_NAME;
bundle bund;
challenge_type type;

int main(int argc, char **argv)
{
    cout << "= SEED MANAGER =" << endl << "by Olybri (v" << VERSION_STR << ")" << endl;
    #if DEV_MODE
    textcolor(BROWN);
    cout << "Build number: #" << setw(4) << setfill('0') << BUILD << " (Developer mode)" << endl;
    textcolor(LIGHTGRAY);
    #endif
    cout << endl;

    init_state_list();

    // Window creation

    window.color(FL_DARK2);

    string str = "RLOCM version " + VERSION_STR;
    status_bar.set(str.c_str());

    bool training_mode = check_bundle();

    menu.add("_&File/&Open rules...\t",FL_CTRL+'o',open_state_cb,0,FL_MENU_INACTIVE);
    menu.add("&File/_&Save rules...\t",FL_CTRL+'s',save_state_cb,0,FL_MENU_INACTIVE);
    menu.add("&File/&Quit\t",FL_CTRL+'q',quit_cb);

    if(!training_mode) menu.add("_&Challenge/_Install &training room\t",FL_CTRL+'t',install_training_room_menu_cb);
    else menu.add("_&Challenge/_Uninstall &training room\t",FL_CTRL+'t',install_training_room_menu_cb);
    menu.add("&Challenge/&Load current challenge\t",FL_CTRL+'l',load_challenge_cb);
    menu.add("&Challenge/&Apply changes\t",FL_CTRL+'a',apply_changes_cb,0,FL_MENU_INACTIVE);
    menu.add("&Challenge/_&Reset changes\t",0,reset_changes_cb,0,FL_MENU_INACTIVE);
    menu.add("&Challenge/&Copy current rules\t",FL_CTRL+'c',copy_state_cb,0,FL_MENU_INACTIVE);
    menu.add("&Challenge/&Paste rules\t",FL_CTRL+'v',paste_state_cb,0,FL_MENU_INACTIVE);

    menu.add("&Help/&Help\t",FL_F+1,help_cb);
    menu.add("&Help/&About...\t",0,about_cb);

    load_button.callback(load_challenge_cb);
    load_button.tooltip("Ctrl+L");

    training_check.value(training_mode);
    training_check.callback(install_training_room_cb);
    training_check.tooltip("Ctrl+T");

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
    reset_button.tooltip("Reset to last changes");

    #if DEV_MODE
    dev_box.align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
    dev_box.labelcolor(84);
    dev_box.labelfont(FL_BOLD);
    console_check.labelcolor(84);
    console_check.labelfont(FL_BOLD);
    console_check.callback(console_show_cb);
    ShowWindow(GetConsoleWindow(), SW_HIDE);
    #endif

    deactive_all();

    load_button.activate();
    training_check.activate();

    window.end();
    window.show(argc, argv);

    Fl::scheme(NULL);
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
    bool change = false;
    if(seed_input.value() != cur_seed.to_str()) change = true;

    if(goal_input.active())
        if(str_to_float(cur_goal) != str_to_float(goal_input.value())) change = true;

    if(limit_input.active())
        if(str_to_float(cur_limit) != str_to_float(limit_input.value())) change = true;

    if(change){
        change_button.activate();
        reset_button.activate();
        menu.mode(menu.find_index(apply_changes_cb),0);
        menu.mode(menu.find_index(reset_changes_cb),FL_MENU_DIVIDER);
    }
    else{
        change_button.deactivate();
        reset_button.deactivate();
        menu.mode(menu.find_index(apply_changes_cb),FL_MENU_INACTIVE);
        menu.mode(menu.find_index(reset_changes_cb),FL_MENU_INACTIVE|FL_MENU_DIVIDER);
    }
}

void load_challenge_cb(Fl_Widget* widget, void*)
{
#if !INIT
    seed_input.textcolor(FL_BLACK);
    goal_input.textcolor(FL_BLACK);
    limit_input.textcolor(FL_BLACK);

    deactive_all();

    cout << endl;

    proc.open(PROCESS_NAME);
    if(!proc){
        status_bar.set("Error!");
        load_button.activate();
        training_check.activate();
        error_out(proc.get_last_error());
        proc.reset();
        return;
    }

    cur_seed = proc.get_seed();
    if(!proc){
        status_bar.set("Error!");
        load_button.activate();
        training_check.activate();
        error_out(proc.get_last_error());
        proc.reset();
        return;
    }
    seed_input.value(cur_seed.to_str().c_str());

    type = proc.get_type(cur_seed);
    if(!proc){
        status_bar.set("Error!");
        load_button.activate();
        training_check.activate();
        error_out(proc.get_last_error());
        proc.reset();
        return;
    }

    status_bar.set("Success!");
    Fl::flush();

#else
    cur_seed = seed(SEED);
    seed_input.value(cur_seed.to_str().c_str());
    type.distance = GOAL;
    type.limit = LIMIT;
    type.level = LEVEL;
    type.event = EVENT;
    type.difficulty = DIFFICULTY;
#endif

    active_all();
    menu.mode(menu.find_index(open_state_cb),0);
    menu.mode(menu.find_index(save_state_cb),FL_MENU_DIVIDER);
    menu.mode(menu.find_index(copy_state_cb),0);
    menu.mode(menu.find_index(paste_state_cb),0);

    level_box.label(LEVEL_LIST[type.level].c_str());

    event_box.label(EVENT_LIST[type.event].c_str());
    diff_box.label(DIFFICULTY_LIST[type.difficulty].c_str());

    if(type.event == DISTANCE) goal_input.deactivate();
    else{
        if(type.distance == UNKNOWN_DISTANCE) goal_input.value("Unknown");
        else{
                ostringstream goal;
                goal << type.distance;
                goal_input.value(goal.str().c_str());
                cur_goal = goal.str();
                if(type.event == SPEED) goal_type.label("m");
                else if(type.event == LUMS_SPEED) goal_type.label("sec");
                else goal_type.label("lums");
                Fl::flush();
            }
    }

    if(type.limit == UNKNOWN_LIMIT) limit_input.value("Unknown");
    else{
        ostringstream limit;
        limit << type.limit;
        limit_input.value(limit.str().c_str());
        cur_limit = limit.str();
        if(type.event == DISTANCE) limit_type.label("m");
        else if(type.event == LUMS_SPEED) limit_type.label("lums");
        else limit_type.label("sec");
        Fl::flush();
    }

    update_change_button();
}

void install_training_room(bool install)
{
    cout << endl;

    bund.open(bundle_path);
    if(!bund){
        status_bar.set("Error!");
        string error = bund.get_last_error();
        error_out(error);
        return;
    }

    //if(training_check.value()){
    if(install){
        status_bar.set("Installing the training room...");
        Fl::flush();
        bund.install_training_room();
    }
    else{
        status_bar.set("Uninstalling the training room...");
        Fl::flush();
        bund.uninstall_training_room();
    }

    bool training_enabled = bund.check_training();
    bund.close();

    training_check.value(training_enabled);
    Fl_Menu_Item *item = (Fl_Menu_Item*)menu.find_item(install_training_room_menu_cb);
    if(!training_enabled) item->label("Install &training room");
    else item->label("Uninstall &training room");

    if(!bund){
        status_bar.set("Error!");
        error_out(bund.get_last_error());
    }
    else status_bar.set("Success!");
}

void install_training_room_menu_cb(Fl_Widget* widget, void*)
{
    Fl_Menu_Item *item = (Fl_Menu_Item*)menu.find_item(install_training_room_menu_cb);
    install_training_room(item->label()[0] == 'I');
}

void install_training_room_cb(Fl_Widget* widget, void*)
{
    install_training_room(training_check.value());
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
        /*if(type.level != DOJO)*/ proc.change_seed(new_seed);
        //else proc.change_seed_dojo(cur_seed, new_seed);
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
    status_bar.clear();
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
    if(!strlen(goal_input.value())) goal_input.value("0");
    if(str_to_float(cur_goal) != str_to_float(goal_input.value())) goal_input.textcolor(FL_RED);
    else goal_input.textcolor(FL_BLACK);
    goal_input.redraw();
    update_change_button();
    status_bar.clear();
}

void limit_input_cb(Fl_Widget* widget, void*)
{
    if(!strlen(limit_input.value())) limit_input.value("0");
    if(str_to_float(cur_limit) != str_to_float(limit_input.value())) limit_input.textcolor(FL_RED);
    else limit_input.textcolor(FL_BLACK);
    limit_input.redraw();
    update_change_button();
    status_bar.clear();
}

void null_cb(Fl_Widget* widget, void*)
{
    Fl_Menu_Item *item = (Fl_Menu_Item*)menu.mvalue();
    MessageBox(0,item->label(),"NULL",0);
}

void save_state_cb(Fl_Widget* widget, void*)
{
    const char* input = fl_input("Save as:");
    if(input == NULL) return;
    string name = input;
    if(!name.size()) return;

    const string key = "RLOCM/" + ALT_LEVEL_LIST[type.level] + "_" + ALT_EVENT_LIST[type.event] + "_" + ALT_DIFFICULTY_LIST[type.difficulty];
    Fl_Preferences app(Fl_Preferences::USER, "RLM Team", key.c_str());

    if(app.groupExists(name.c_str())) if(MessageBox(0, string("Overwrite '" + name + "'?").c_str(), "Rules name already exists", MB_YESNO) == IDNO) return;
    Fl_Preferences group(app, name.c_str());
    group.set("seed", seed_input.value());
    group.set("goal", goal_input.value());
    group.set("limit", limit_input.value());

    status_bar.set("Rules saved!");
}

void open_state_cb(Fl_Widget* widget, void*)
{
    const string key = "RLOCM/" + ALT_LEVEL_LIST[type.level] + "_" + ALT_EVENT_LIST[type.event] + "_" + ALT_DIFFICULTY_LIST[type.difficulty];
    Fl_Preferences app(Fl_Preferences::USER, "RLM Team", key.c_str());

    browser.clear();
    for(int i=0; i<app.groups(); i++)
        browser.add(app.group(i));

    browser.sort(FL_SORT_ASCENDING);

    state_list.show();
}

void copy_state_cb(Fl_Widget* widget, void*)
{
    seed_input.do_callback();

    ostringstream ostr;
    ostr << "{" << seed_input.value()
        << ";" << hex << uppercase << setw(8) << setfill('0') << process::float_to_uint32(str_to_float(goal_input.value()))
        << ";" << process::float_to_uint32(str_to_float(limit_input.value())) << "}";
    string str = ostr.str();
    for(unsigned int i=0; i<str.size(); i++)
        if(str[i] == ' ') str.erase(i,1);
    const char *text = str.c_str();

    HANDLE hGlobalMem = GlobalAlloc(GHND, lstrlen(text)+1);
    char *lpGlobalMem = NULL;
    if(hGlobalMem == NULL) return;

    lpGlobalMem = (char*)GlobalLock(hGlobalMem);
    if(lpGlobalMem == NULL) return;

    lstrcpy(lpGlobalMem, text);
    GlobalUnlock(hGlobalMem);
    if(!OpenClipboard(NULL)) return;

    EmptyClipboard();
    SetClipboardData(CF_TEXT, hGlobalMem);
    CloseClipboard();
}

void paste_state_cb(Fl_Widget* widget, void*)
{
    if(!IsClipboardFormatAvailable(CF_TEXT)) return;
    if(!OpenClipboard(NULL)) return;
    HGLOBAL hglb = GetClipboardData(CF_TEXT);
    if (hglb != NULL)
    {
        string str = static_cast<char*>(GlobalLock(hglb));
        if(str.size())
        {
            if(str[0] != '{' ||
                str[9] != ';' ||
                str[18] != ';' ||
                str[27] != '}' ||
                str.size() != 28){
                    error_out("Invalid clipboard...");
                    GlobalUnlock(hglb);
                    CloseClipboard();
                    return;
                }

            string seed_str = str;
            seed_str.erase(0,1);
            seed_str.erase(8,str.size()-9);

            string goal_str = str;
            goal_str.erase(0,10);
            goal_str.erase(8,str.size()-9);

            string limit_str = str;
            limit_str.erase(0,19);
            limit_str.erase(8,str.size()-9);

            seed_input.value(seed(seed_str).to_str().c_str());
            seed_input.do_callback();

            istringstream goal_isstr(goal_str);
            uint32_t uint;
            goal_isstr >> hex >> uint;
            ostringstream goal_osstr;
            goal_osstr << process::uint32_to_float(uint);
            goal_input.value(goal_osstr.str().c_str());
            goal_input.do_callback();

            istringstream limit_isstr(limit_str);
            limit_isstr >> hex >> uint;
            ostringstream limit_osstr;
            limit_osstr << process::uint32_to_float(uint);
            limit_input.value(limit_osstr.str().c_str());
            limit_input.do_callback();

            GlobalUnlock(hglb);
        }
    }
    CloseClipboard();
}

void about_cb(Fl_Widget* widget, void*)
{
    MessageBox(0,string("Rayman Legends Offline Challenge Manager:\n"
                "Version " + VERSION_STR + "\n"
                "RLOCM will allow you to change the rules of the\nchallenges used in Rayman® Legends.\n\n"
                "Credit:\n"
                "Olybri: Author of this program\n"
                "DJTHED: Discovered a way to modify the seed\n"
                "CaneofPacci: Unlocked the training room on PC\n"
                "UsWar: Discovered nice things in the RAM.\n").c_str(),
                "About the program",0);
}

void help_cb(Fl_Widget* widget, void*)
{
    system("start readme.txt");
}

/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////


void deactive_all(void)
{
    load_button.deactivate();
    training_check.deactivate();

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
    load_button.activate();
    training_check.activate();

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
                if(!bundle_path.size()) exit(0);
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
