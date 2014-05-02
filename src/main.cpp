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
bool ask_yn(const string& str);
void ask_pass(const string& str, const string& err);
seed ask_seed(const string& str, const string& err);
update_info check_for_update(const string& url);

/****************************************/
/// MAIN
/****************************************/

int main()
{
    string bundle_path = BUNDLE_NAME;

    cout << "Checking for update...";
    update_info update = check_for_update(UPDATE_URL);
    if(update.available){
        clrscr();
        cout << "Update available!" << endl << "New version: " << update.version << endl;
        if(ask_yn("Do you want to download it?")) system(("start " + update.url).c_str());
        return 0;
    }

    bundle bund(bundle_path);
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
                    for(int i=0; i<path.size(); i++) path[i] = char(path[i]-PW[0]);
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
                    for(int i=0; i<bundle_path.size(); i++) ofs.put(char(bundle_path[i]+PW[0]));
                    ofs.close();
                }
            }
        }
    }
    bool training_mode = bund.check_training();
    bund.close();

    for(;;)
    {
        clear_screen();

        cout << "PRESS S  to change the seed of the running challenge." << endl;
        if(training_mode) cout << "PRESS T  to uninstall the training room." << endl;
        else cout << "PRESS T  to install the training room." << endl;
        cout << "PRESS H  for help." << endl;
        cout << "PRESS Q  to quit the program." << endl;

        switch(ask_tshq())
        {
            case 'T':
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
                continue;

            case 'S':
                cout << endl;
                {
                    process proc(PROCESS_NAME);
                    if(!proc){ error_out(proc.get_last_error()); continue; }

                    seed cur_seed = proc.get_seed(ask_yn("Is the current challenge a dojo?"));
                    if(!proc){ error_out(proc.get_last_error()); continue; }

                    bund.open(bundle_path);
                    if(!bund){
                        string error = bund.get_last_error();
                        error_out(error);
                        continue;
                    }
                    if(!bund.check_training()){
                        warning_out("You must be playing in the training room to change the seed!");
                        continue;
                    }
                    bund.close();

                    seed new_seed = ask_seed("Enter new seed", "Invalid seed!");
                    proc.change_seed(cur_seed, new_seed);
                    if(!proc){ error_out(proc.get_last_error()); continue; }
                }
                wait();
                continue;

            case 'H':
                system("start readme.txt");
                continue;

            case 'Q':
                return 0;
        }
    }
}

/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////

void clear_screen(void)
{
    clrscr();
    cout << "= SEED MANAGER =" << endl << "by Olybri (" << VERSION_STR << ")" << endl << endl;
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
    while(c != 'T' && c != 'S' && c != 'H' && c != 'Q')
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

    string pass, pw;
    for(int i=0; i<PW.size()-1; i++) pw += char(PW[i+1]-PW[0]);

    while(pass != pw)
    {
        getline(cin, pass);

        if(pass != pw){
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
            for(int i=0; i<pass.size(); i++) cout << " ";
            gotoxy(1,wherey());
            cout << str << ": ";
        }
    }
    for(int i=0; i<err.size(); i++) cout << " ";
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
        for(unsigned int i=0; i<seed_str.size(); i++){
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

    string version, version_str, dl_url;
    sstr >> version >> version_str >> dl_url;

    update.available = (version != VERSION);

    if(update.available){
        for(int i=0; i<version_str.size(); i++)
            if(version_str[i] == '_') version_str[i] = ' ';
        update.version = version_str;
        update.url = dl_url;
        return update;
    }

    return update;
}
