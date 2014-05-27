#include "seed_manager.h"
#include "jos/download.h"

using namespace std;

// HELPERS:
void clear_screen(void);
void wait(void);
string get_bundle_path(void);
void error_out(const string& str, bool _exit = false);
void warning_out(const string& str, bool _exit = false);
char ask_tshq(void);
char ask_sgbhq(void);
bool ask_yn(const string& str);
void ask_pass(const string& str, const string& err);
seed ask_seed(const string& str, const string& err);
float ask_float(const string& str, const string& err);
update_info check_for_update(const string& url);

/****************************************/
/// MAIN
/****************************************/

int main()
{
    string bundle_path = BUNDLE_NAME;

    #if DEV_MODE == 0
    cout << "Checking for update...";
    update_info update = check_for_update(UPDATE_URL);
    if(update.available){
        clrscr();
        cout << "Update available!" << endl << "New version: " << update.version << endl;
        if(ask_yn("Do you want to download it?")) system(("start " + update.url).c_str());
        return 0;
    }
    #endif

    bundle bund(bundle_path);

    /*cout << endl;
    bund.seek("cache/itf_cooked/pc/world/home/level/home.isc.ckd", true);
    bund.seek("cache/itf_cooked/pc/world/challenge/shaolinplaza/brick/challenge_shaolin_default_expert.isg.ckd", true);
    bund.seek("cache/itf_cooked/pc/world/jungle/level/ju_rl_2_movingroots_inv/ju_rl_2_movingroots_inv.isc.ckd", true);
    bund.close();
    wait(); exit(0);*/


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
                clear_screen();
                cout << "Cannot found the Bundle_PC..." << endl << "You will need to specify the path of the bundle." << endl;
                ask_pass("Enter password", "Wrong password!");

                char current_dir[FILENAME_MAX];
                getcwd(current_dir, sizeof(current_dir));

                bundle_path = get_bundle_path();
                if(!bundle_path.size()) warning_out("No path specified...", true);
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

    int lol = 0;
    for(;;)
    {
        clear_screen();

        cout << "PRESS C  to load the current challenge seed." << endl;
        #if DEV_MODE
        textcolor(RED);
        cout << "PRESS T  to install the training room." << endl;
        textcolor(LIGHTGRAY);
        #else
        if(training_mode) cout << "PRESS T  to uninstall the training room." << endl;
        else cout << "PRESS T  to install the training room." << endl;
        #endif
        /*cout << "PRESS U  to install ";
        cout << "Ubi Ray";
        cout << "." << endl;*/

        cout << endl;

        cout << "PRESS H  for help." << endl;
        cout << "PRESS Q  to quit the program." << endl;

        switch(ask_tshq())
        {
            #if DEV_MODE == 0
            case 'T':{
                cout << endl;

                bund.open(bundle_path);
                if(!bund){
                    string error = bund.get_last_error();
                    error_out(error);
                    continue;
                }

                if(training_mode) bund.uninstall_training_room();
                else bund.install_training_room();
                training_mode = bund.check_training();
                bund.close();

                if(!bund){ error_out(bund.get_last_error()); continue; }

                wait();
                continue;}
            #endif

            case 'C':
                cout << endl;
                {
                    process proc(PROCESS_NAME);
                    if(!proc){ error_out(proc.get_last_error()); continue; }

                    int level;
                    if(ask_yn("Is the current challenge a dojo?") == YES) level = DOJO;
                    else level = UNKNOWN_LEVEL;
                    seed cur_seed = proc.get_seed(level);
                    if(!proc){ error_out(proc.get_last_error()); continue; }

                    challenge_type type;
                    if(level != DOJO){
                        type = proc.get_type(cur_seed);
                        if(!proc){ error_out(proc.get_last_error()); continue; }
                    }
                    else{
                        type.level = DOJO;
                        type.event = UNKNOWN_EVENT;
                        type.difficulty = UNKNOWN_DIFFICULTY;
                        type.distance = UNKNOWN_DISTANCE;
                        type.limit = UNKNOWN_LIMIT;
                    }

                    Sleep(1400);
                    //wait();

                    //////////////////////////

                    bool loop = true;

                    while(loop)
                    {
                        clear_screen();

                        cout << "Challenge loaded: " << endl << LEVEL_LIST[type.level] << endl;
                        cout << " Event: " << EVENT_LIST[type.event] << endl;
                        cout << " Seed: " << cur_seed.to_str() << endl;
                        if(type.distance == UNKNOWN_DISTANCE) cout << " Goal: Unknow" << endl;
                        else if(type.event == SPEED && type.distance < 10000) cout << " Goal: " << type.distance << " m" << endl;
                        else if(type.event == SPEED) cout << " Goal: " << type.distance/1000 << " km" << endl;
                        else if(type.event == LUMS) cout << " Goal: " << type.distance << " lums" << endl;
                        cout << " Difficulty: " << DIFFICULTY_LIST[type.difficulty] << endl;
                        if(type.limit == UNKNOWN_LIMIT) cout << " Time limit: Unknow" << endl;
                        else if(type.event == DISTANCE) cout << " Time limit: Unlimited" << endl;
                        else cout << " Time limit: " << dec << int(type.limit/60.0) << "'" << setw(2) << setfill('0') << int(type.limit)%60 << "\"" << setw(2) << setfill('0') << int(type.limit*100.0)%100 << endl;

                        cout << endl;

                        bund.open(bundle_path);
                        if(!bund){
                            string error = bund.get_last_error();
                            error_out(error);
                            continue;
                        }
                        bool training = bund.check_training();
                        bund.close();

                        if(!training) textcolor(RED);
                        cout << "PRESS S  to change the seed." << endl;
                        if((type.event == SPEED || type.event == LUMS) && (type.level != DOJO && type.level != UNKNOWN_LEVEL)){
                            cout << "PRESS G  to change the goal." << endl;
                            cout << "PRESS L  to change the time limit." << endl;
                        }
                        if(!training) textcolor(LIGHTGRAY);
                        cout << "PRESS B  to go back to menu." << endl;
                        cout << endl;
                        cout << "PRESS H  for help." << endl;
                        cout << "PRESS Q  to quit the program." << endl;

                        switch(ask_sgbhq())
                        {
                            case 'S':
                                if(type.level == UNKNOWN_LEVEL) continue;
                                cout << endl;
                                {
                                    if(!training){
                                        warning_out("You must be playing in the training room to change the seed!");
                                        continue;
                                    }
                                    seed new_seed = ask_seed("Enter new seed", "Invalid seed!");
                                    if(type.level != DOJO) proc.change_seed(new_seed);
                                    else proc.change_seed_dojo(cur_seed, new_seed);
                                    if(!proc){ error_out(proc.get_last_error()); continue; }
                                    cur_seed = new_seed;
                                    cout << "You need to restart the challenge to apply the changes." << endl;
                                }
                                wait();
                                continue;

                            case 'L':
                                if((type.event != SPEED && type.event != LUMS) || type.level == DOJO || type.level == UNKNOWN_LEVEL) continue;
                                cout << endl;
                                {
                                    if(!training){
                                        warning_out("You must be playing in the training room to change the time limit!");
                                        continue;
                                    }
                                    float limit = ask_float("Enter new time limit (seconds)", "Invalid number!");
                                    proc.change_limit(limit);
                                    if(!proc){ error_out(proc.get_last_error()); continue; }
                                    type.limit = limit;
                                }
                                wait();
                                continue;

                            case 'G':
                                if((type.event != SPEED && type.event != LUMS) || type.level == DOJO || type.level == UNKNOWN_LEVEL) continue;
                                cout << endl;
                                {
                                    if(!training){
                                        warning_out("You must be playing in the training room to change the goal!");
                                        continue;
                                    }
                                    float distance;
                                    if(type.event == SPEED) distance = ask_float("Enter new goal (meters)", "Invalid number!");
                                    else distance = ask_float("Enter new goal (lums)", "Invalid number!");
                                    proc.change_distance(distance);
                                    if(!proc){ error_out(proc.get_last_error()); continue; }
                                    type.distance = distance;
                                }
                                wait();
                                continue;

                            case 'B':
                                loop = false;
                                continue;

                            case 'H':
                                system("start readme.txt");
                                continue;

                            case 'Q':
                                return 0;
                        }
                    }
                }
                continue;

            case 'H':
                system("start readme.txt");
                continue;

            case 'Q':
                return 0;

            case 'U':
                switch(lol){
                    case 0: MessageBoxW(0,L"lol nope ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_OK); break;
                    case 1: MessageBoxW(0,L"I said: \"lol nope\" ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_OK); break;
                    case 2: MessageBoxW(0,L"Again: \"lol nope\" ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_OK); break;
                    case 3: MessageBoxW(0,L"Stop that, or the program will... crash ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_OK); break;
                    case 4: MessageBoxW(0,L"CRASH ( ͡° ͜ʖ ͡°) ( ͡° ͜ʖ ͡°) ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_OK); *((unsigned int*)0) = 0xdead; break;
                }

                ++lol;
                continue;

                /*
                if(MessageBoxW(0,L"You sure wanna install Ubi Ray? ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"Really? ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"Really sure? ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"Is that a good idea? ( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"Really? ( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"Really sure? ( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°) ?",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"Is the program going to crash? ( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",MB_YESNO) == IDNO) continue;
                if(MessageBoxW(0,L"The program is going to crash. Cheers.\n( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)\n( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)( ͡° ͜ʖ ͡°)",L"( ͡° ͜ʖ ͡°)",0) == IDNO) continue;
                *((unsigned int*)0) = 0xDEAD;
                */
        }
    }
}

/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////

void clear_screen(void)
{
    clrscr();
    cout << "= SEED MANAGER =" << endl << "by Olybri (" << VERSION_STR << ")" << endl;
    #if DEV_MODE
    textcolor(BROWN);
    cout << "Build number: #" << setw(4) << setfill('0') << BUILD << " (Developer mode)" << endl;
    textcolor(LIGHTGRAY);
    #endif
    cout << endl;
}

void wait(void)
{
    cout << "Press any key...";
    getch();
}

string get_bundle_path(void)
{
    OPENFILENAMEA ofn;

    char file[1000] = {"Bundle_PC.ipk"};
    ofn.lStructSize = sizeof(OPENFILENAME);
    ofn.hwndOwner = GetActiveWindow();
    ofn.hInstance = 0;
    ofn.lpstrFilter = "Itsy Package (*.ipk)\0*.ipk\0All Files (*.*)\0*.*\0\0";
    ofn.lpstrCustomFilter = 0;
    ofn.nMaxCustFilter = 0;
    ofn.nFilterIndex = 1;
    ofn.lpstrFile = file;
    ofn.nMaxFile = 1000;
    ofn.lpstrTitle = "Where is Bundle_PC.ipk?";
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = 0;
    ofn.Flags = OFN_FILEMUSTEXIST | OFN_LONGNAMES | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY;
    ofn.nFileOffset = 0;
    ofn.nFileExtension = 0;
    ofn.lpstrDefExt = "ipk";
    ofn.lCustData = 0;
    ofn.lpfnHook = 0;
    ofn.lpTemplateName = 0;


    if(!GetOpenFileName(&ofn))
        return "";
    return ofn.lpstrFile;
}

void error_out(const string& str, bool _exit)
{
    textcolor(LIGHTRED);
    cout << endl << "ERROR: " << str << endl;
    textcolor(LIGHTGRAY);
    wait();
    if(_exit) exit(0);
}

void warning_out(const string& str, bool _exit)
{
    textcolor(CYAN);
    cout << endl << "WARNING: " << str << endl;
    textcolor(LIGHTGRAY);
    wait();
    if(_exit) exit(0);
}

char ask_tshq(void)
{
    char c = 0;
    while(c != 'T' && c != 'C' && c != 'H' && c != 'Q'/* && c != 'U'*/)
        c = toupper(getch());

    return c;
}

char ask_sgbhq(void)
{
    char c = 0;
    while(c != 'S' && c != 'G' && c != 'L' && c != 'H' && c != 'Q' && c != 'B')
        c = toupper(getch());

    return c;
}

bool ask_yn(const string& str)
{
    cout << str << " (Y/N): ";

    char c = 0;
    while(c != 'Y' && c != 'N')
        c = toupper(getch());

    cout << c << endl;

    if(c == 'Y' || c == 'y') return YES;
    return NO;
}

void ask_pass(const string& str, const string& err)
{
    cout << str << ": ";

    string pass;
    bool correct = false;

    while(!correct)
    {
        getline(cin, pass);
        for(unsigned int i=0; i<PW.size()-1; i++)
        if(pass[i] != char(PW[i+1]-PW[0])){
            correct = false;
            break;
        }
        else correct = true;

        if(!correct){
            textcolor(LIGHTRED);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(YELLOW);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(LIGHTRED);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(YELLOW);
            cout << err;

            textcolor(LIGHTGRAY);
            gotoxy(1,wherey()-1);
            cout << str << ": ";
            for(unsigned int i=0; i<pass.size(); i++) cout << " ";
            gotoxy(1,wherey());
            cout << str << ": ";
        }
    }
    for(unsigned int i=0; i<err.size(); i++) cout << " ";
    gotoxy(1,wherey());
}

seed ask_seed(const string& str, const string& err)
{
    cout << str << ": ";

    string seed_str;
    bool correct = false;

    while(!correct)
    {
        getline(cin, seed_str);
        correct = true;
        int size = seed_str.size();
        if(!size) correct = false;
        else for(unsigned int i=0; i<seed_str.size(); i++){
            if(seed_str[i] == ' ') seed_str.erase(i,1);
            if(seed_str[i] < '0' || seed_str[i] > '9')
                if(seed_str[i] < 'A' || seed_str[i] > 'F')
                    if(seed_str[i] < 'a' || seed_str[i] > 'f'){
                        correct = false;
                        break;
                    }
        }
        if(seed_str.size() != 8) correct = false;
        if(!correct){
            textcolor(LIGHTRED);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(YELLOW);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(LIGHTRED);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(YELLOW);
            cout << err;

            textcolor(LIGHTGRAY);
            gotoxy(1,wherey()-1);
            cout << str << ": ";
            for(int i=0; i<size; i++) cout << " ";
            gotoxy(1,wherey());
            cout << str << ": ";
        }
    }

    return seed(seed_str);
}

float ask_float(const string& str, const string& err)
{
    cout << str << ": ";

    string float_str;
    bool correct = false;

    while(!correct)
    {
        getline(cin, float_str);
        correct = true;
        int size = float_str.size();
        if(!size) correct = false;
        else for(unsigned int i=0; i<float_str.size(); i++){
            if(float_str[i] == ' ') float_str.erase(i,1);
            if(float_str[i] < '.' || float_str[i] > '9' || float_str[i] == '/'){
                correct = false;
                break;
            }
        }
        if(!correct){
            textcolor(LIGHTRED);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(YELLOW);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(LIGHTRED);
            cout << err;
            Sleep(120);
            gotoxy(1,wherey());
            textcolor(YELLOW);
            cout << err;

            textcolor(LIGHTGRAY);
            gotoxy(1,wherey()-1);
            cout << str << ": ";
            for(int i=0; i<size; i++) cout << " ";
            gotoxy(1,wherey());
            cout << str << ": ";
        }
    }

    istringstream istr(float_str);
    float f;
    istr >> f;

    return f;
}


update_info check_for_update(const string& url)
{
    update_info update;
    update.available = false;

    vector<uint8_t> vec;
    try{ vec = Download::download(url.c_str(), true, NULL); }
    catch(DLExc exc){
        cout << endl << "Could not get update informations!";
        Sleep(1400);
        return update;
    }

    if(!vec.size()) return update;

    stringstream sstr;
    for(unsigned int i=0; i<vec.size(); i++) sstr << vec[i];

    int version;
    string version_str, dl_url;
    sstr >> version >> version_str >> dl_url;

    update.available = (BUILD < version);

    if(update.available){
        for(unsigned int i=0; i<version_str.size(); i++)
            if(version_str[i] == '_') version_str[i] = ' ';
        update.version = version_str;
        update.url = dl_url;
        return update;
    }

    return update;
}
