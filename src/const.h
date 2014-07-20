#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

/****************************************/
/// DEVELOPPER MODE
/****************************************/

#define DEV_MODE TRUE    // DO NOT MODIFY

#define INIT FALSE
#define SEED "DEADBEEF"
#define GOAL 300
#define LIMIT 180
#define LEVEL 0
#define EVENT 0
#define DIFFICULTY 0

/****************************************/
/// INCLUDES
/****************************************/

#define _WIN32_WINNT 0x0500

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Menu_Window.H>
#include <Fl/Fl_Check_Button.H>
#include <FL/Fl_Hold_Browser.H>
#include <Fl/Fl_Box.H>
#include <FL/fl_ask.H>
#include <FL/Fl_Input.H>
#include <FL/Fl_Float_Input.H>
#include <FL/Fl_Native_File_Chooser.H>
#include <FL/Fl_Menu_Bar.H>
#include <FL/Fl_Preferences.H>

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>
#include <time.h>
#include <dirent.h>
//#include <sys/stat.h>
#include <conio2.h>
#include <wininet.h>
#include <tlhelp32.h>
//#include <zlib/zlib.h>
//#include <assert.h>

/** LIBS:
    -lconio
    -lzlibstatic
    -lwininet
    -lcomdlg32
*/

/****************************************/
/// CONSTANTS
/****************************************/

#define INPUT_MENU_X360                 101
#define CHALLENGE_ENDLESS               102
#define CHALLENGE_1                     103
#define CHALLENGE_2                     104
#define CHALLENGE_3                     105
#define CHALLENGE_4                     106
#define CHALLENGE_5                     107
#define PAINTING_CHALLENGEENDLESS_A1    108
#define SUITCASE_A1                     109

#define CHALLENGE_SHAOLIN_EXPERT        121

#define MENU_Y 26

// Window size
const int WIDTH = 390;
#if !DEV_MODE
const int HEIGHT = 260 + MENU_Y;
#else
const int HEIGHT = 290 + MENU_Y;
#endif

class StatusBar : public Fl_Box
{
    public:
        StatusBar(void) : Fl_Box(-1, HEIGHT-25, WIDTH+2, 26){
            color(FL_DARK1);
            align(FL_ALIGN_LEFT|FL_ALIGN_INSIDE);
            box(FL_BORDER_BOX);
        }
        void set(const char* text){ label(text); }
        void clear(void){ label(""); }
};

struct update_info
{
    bool available;
    std::string version;
    std::string url;
};

const int BUILD = 11;
const std::string VERSION_STR = "1.0.0";
const std::string UPDATE_URL = "https://dl.dropboxusercontent.com/u/109130039/seed-manager.com/update?dl=1";

const bool NO = false;
const bool YES = true;

const std::string PROCESS_NAME = "Rayman Legends.exe";
const std::string BUNDLE_NAME = "C:/Program Files (x86)/Steam/SteamApps/common/Rayman Legends/Bundle_PC.ipk";
const std::string BUNDLE_NAME_2 = "C:/Program Files (x86)/Ubisoft/Rayman Legends/Bundle_PC.ipk";

// Password:
// p4v8zow9
const std::string PW = "\x3B\xAB\x6F\xB1\x73\xB5\xAA\xB2\x74";
// u3nqp2ri
//const std::string PW = "\x82\xF7\xB5\xF0\xF3\xF2\xB4\xF4\xEB";

const std::string FILE_LIST[9] = {
    "cache/itf_cooked/pc/enginedata/inputs/menu/input_menu_x360.isg.ckd",
    "cache/itf_cooked/pc/world/home/brick/challenge/challenge_endless.isc.ckd",
    "cache/itf_cooked/pc/world/home/paintings_and_notifications/painting_levels/textures/challenge/challenge_1.tga.ckd",
    "cache/itf_cooked/pc/world/home/paintings_and_notifications/painting_levels/textures/challenge/challenge_2.tga.ckd",
    "cache/itf_cooked/pc/world/home/paintings_and_notifications/painting_levels/textures/challenge/challenge_3.tga.ckd",
    "cache/itf_cooked/pc/world/home/paintings_and_notifications/painting_levels/textures/challenge/challenge_4.tga.ckd",
    "cache/itf_cooked/pc/world/home/paintings_and_notifications/painting_levels/textures/challenge/challenge_5.tga.ckd",
    "cache/itf_cooked/pc/world/home/paintings_and_notifications/painting_challengeendless/animation/painting_challengeendless_a1.tga.ckd",
    "cache/itf_cooked/pc/world/common/ui/suitcase/animation/suitcase_a1.tga.ckd"};

const std::string SHAOLIN_PATH = "cache/itf_cooked/pc/world/challenge/shaolinplaza/brick/challenge_shaolin_default_expert.isg.ckd";

#endif // CONST_H_INCLUDED
