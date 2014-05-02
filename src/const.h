#ifndef CONST_H_INCLUDED
#define CONST_H_INCLUDED

/****************************************/
/// INCLUDES
/****************************************/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include <windows.h>
#include <dirent.h>
#include <sys/stat.h>
#include <conio/conio2.h>
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

#define DEBUG 0 // DO NOT MODIFY

struct update_info
{
    bool available;
    std::string version;
    std::string url;
};

const std::string VERSION = "0.0.0.3";
const std::string VERSION_STR = "beta 1.0.3";
const std::string UPDATE_URL = "https://dl.dropboxusercontent.com/u/109130039/seed_manager/update?dl=1";

const bool DEFAULT = false;
const bool DOJO = true;

const bool NO = false;
const bool YES = true;

const std::string PROCESS_NAME = "Rayman Legends.exe";
const std::string BUNDLE_NAME = "C:/Program Files (x86)/Steam/SteamApps/common/Rayman Legends/Bundle_PC.ipk";
const std::string BUNDLE_NAME_2 = "C:/Program Files (x86)/Ubisoft/Rayman Legends/Bundle_PC.ipk";

// Password:
// p4v8zow9
const std::string PW = "\x3B\xAB\x6F\xB1\x73\xB5\xAA\xB2\x74";

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

/// NO LONGER NECESSARY:
/**
const int INPUT_MENU_X360_ADDRESS = 0x1C72B3FC;
const int CHALLENGE_ENDLESS_ADDRESS = 0x1CE6737A;
const int CHALLENGE_1_ADDRESS = 0x20A790F9;
const int CHALLENGE_2_ADDRESS = 0x20A9C328;
const int CHALLENGE_3_ADDRESS = 0x20ABAF35;
const int CHALLENGE_4_ADDRESS = 0x20ADB6FF;
const int CHALLENGE_5_ADDRESS = 0x20AFB255;
const int PAINTING_CHALLENGEENDLESS_A1_ADDRESS = 0x1F275CB3;
const int SUITCASE_A1_ADDRESS = 0x20D6EFD3;
*/

/// NO LONGER NECESSARY:
/**
const int ADDRESS[9] =
   {0x1C72B3FC, 0x1CE6737A, 0x20A790F9, 0x20A9C328,
    0x20ABAF35, 0x20ADB6FF, 0x20AFB255, 0x1F275CB3,
    0x20D6EFD3};
*/

#endif // CONST_H_INCLUDED
