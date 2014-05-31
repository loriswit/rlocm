#include "process.h"

using namespace std;

/****************************************/
/// class process
/****************************************/

process::process(const string& process_name)
{
    success = true;
    location = 0;
    location_2 = 0;

    open(process_name);
}

process::process(void)
{
    success = true;
    location = 0;
    location_2 = 0;
}

bool process::open(const string& process_name)
{
    /// NO LONGER NECESSARY:
    /**
	const char* window_name = process_name.c_str();
    // we look for a window named as 'process_name'
	cout << "Finding process: " << window_name << "... ";
    HWND hwnd = FindWindow(NULL, window_name);
	if(!hwnd){
	    last_error = "Process not found!";
	    success = false;
	    return;
	}
    cout << "Success!" << endl;

    // we get the ID of the process we have found
    DWORD pid;
    cout << "Getting thread process ID... ";
    GetWindowThreadProcessId(hwnd,&pid);
    cout << "Sucess!" << endl;

    // we open the process with read/write access
    cout << "Opening process... ";
    phandle = OpenProcess(PROCESS_VM_READ|PROCESS_VM_WRITE|PROCESS_VM_OPERATION,0,pid);
    if(!phandle){
        last_error = "Could not get handle!";
        success = false;
        return;
    }
    */

    cout << "Opening process " << process_name << "... ";
    phandle = get_handle(process_name);
    if(phandle == NULL){
        last_error = "Process \"" + process_name + "\" not found!";
        success = false;
        return false;
    }

    cout << "Success!" << endl;
    return true;
}

extern StatusBar status_bar;

seed process::get_seed(int level)
{
    cout << "Locating seed... ";

    // prorgess bar init
    textcolor(DARKGRAY);
    for(int i=0; i<16; i++) cout << char(0xB0);
    textcolor(LIGHTGRAY);
    int progression = 0;
    gotoxy(18,wherey());
    _setcursortype(1);

    string str; // we're looking for this string in the RAM
    if(level != DOJO) str = "countdown.act";
    else str = "countdown_shaolin.act";

    bool located = false; // true if the correct string has been found
    unsigned int address = 0;
    unsigned char buffer[0x1000];

    int percent = 0;
    for(int i=0; i<0x100000; i++)
    {
        // we read the RAM by brick of 0x1000 bytes
        address = i*0x1000;

        if(!ReadProcessMemory(phandle,(void*)address,&buffer,sizeof(buffer),NULL)) continue; // continue if the area is inaccessible

        for(unsigned int j=0; j<sizeof(buffer); j++)
        {
            if(buffer[j] == str[0])
                for(unsigned int k=1; k<str.size(); k++){ // we check if we found the string
                    if(buffer[j+k] == str[k]) located = true;
                    else{
                        located = false;
                        break;
                    }
                }

            if(located)
            {
                // if the string has been found, we need to be sure it's preceded by some specific data
                if(level != DOJO)
                {
                    // 01 00 00 00   XX XX XX XX   00 00 00 00   00 00 00 00 'str'
                    // (assuming XX is one byte of the seed and 'str' is the string that has been found)

                    for(int k=0; k<8; k++){
                        if(buffer[j-1-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located) for(int k=0; k<3; k++){
                        if(buffer[j-13-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located)
                        if(buffer[j-16]!=0x01){
                            located=false;
                            break;
                        }
                }

                else
                {
                    // XX XX XX XX   ?? ?? ?? ??   ?? ?? ?? ??   ?? ?? ?? ??
                    // ?? ?? ?? ??   ?? ?? ?? ??   ?? ?? ?? ??   ?? ?? ?? ??
                    // ?? ?? ?? ??   ?? ?? ?? ??   ?? ?? ?? ??   ?? ?? ?? ??
                    // 02 00 00 00   02 00 00 00   ?? ?? ?? ??   ?? ?? ?? ??
                    // 00 00 00 00   01 00 00 00   ?? ?? ?? ??   ?? ?? ?? ??
                    // ?? ?? ?? ??   00 00 00 00   ?? ?? ?? ??   00 00 00 00
                    // 00 00 00 00   ?? ?? ?? ??   ?? ?? ?? ??   00 00 00 00
                    // ?? ?? ?? ??   'str'
                    // (assuming XX is one byte of the seed, ?? is an unknown byte and 'str' is the string that has been found)

                    for(int k=0; k<4; k++){
                        if(buffer[j-5-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    /*if(located) for(int k=0; k<3; k++){
                        if(buffer[j-13-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located)
                        if(buffer[j-16]!=0x01)
                            located=false;*/

                    if(located) for(int k=0; k<8; k++){
                        if(buffer[j-17-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located) for(int k=0; k<4; k++){
                        if(buffer[j-18-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located) for(int k=0; k<3; k++){
                        if(buffer[j-45-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located)
                        if(buffer[j-48]!=0x01)
                            located=false;

                    if(located) for(int k=0; k<4; k++){
                        if(buffer[j-49-k]!=0x00){
                            located = false;
                            break;
                        }
                    }

                    if(located) for(int k=0; k<3; k++){
                        if(buffer[j-61-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located)
                        if(buffer[j-64]!=0x02)
                            located=false;

                    if(located) for(int k=0; k<3; k++){
                        if(buffer[j-65-k]!=0x00){
                            located = false;
                            break;
                        }
                    }
                    if(located)
                        if(buffer[j-68]!=0x02)
                            located = false;
                }
            }

            if(located) break;
            else address++;
        }

        if(located) break;

        // we update the progress bar
        if(progression != i/0x1000 && progression < 16*4){
            progression++;
            gotoxy(18+progression/4,wherey());
            if(progression%4 == 1) cout << char(0xB0);
            if(progression%4 == 2) cout << char(0xB1);
            if(progression%4 == 3) cout << char(0xB2);
        }
        if(percent == -1) continue;
        if(i/2000 != percent && percent < 100){
            percent = i/2000;
            ostringstream str;
            if(level != DOJO) str << "Loading challenge " << percent/2 << "%";
            else str << "Loading challenge " << percent << "%";
            status_bar.set(str.str().c_str());
            Fl::flush();
        }
        else if(percent > 99){
            status_bar.set("Please wait...");
            Fl::flush();
            percent = -1;
        }
    }

    // we finalize the progress bar
    gotoxy(18,wherey());
    for(int i=0; i<16; i++) cout << char(0xB2);
    _setcursortype(_NORMALCURSOR);

    // now it's time to read the seed, which is located some bytes before the string that has been found
    seed s;
    int pos;
    if(level != DOJO) pos = 12;
    else pos = 116;

    if(located){
        cout << " Success!" << endl << "  Seed: ";
        for(int i=0; i<4; i++){
            char byte;
            ReadProcessMemory(phandle,(void*)address-pos+i,&byte,sizeof(char),NULL);
            s.set(i, byte);
            cout <<  hex << uppercase << setw(2) << setfill('0') << s[i] << " ";
        }
        location = address-pos;
        cout << "(location: 0x" << hex << uppercase << location << ")" << endl;
    }
    else{
        last_error = "Seed not found!";
        success = false;
    }

    return s;
}

challenge_type process::get_type(seed cur_seed)
{
    cout << "Locating challenge type... ";
    challenge_type type;
    type.level = UNKNOWN_LEVEL;
    type.event = UNKNOWN_EVENT;
    type.difficulty = UNKNOWN_DIFFICULTY;
    type.distance = UNKNOWN_DISTANCE;
    type.limit = UNKNOWN_LIMIT;

    if(!location){
        last_error = "'location' is undefined";
        success = false;
        return type;
    }

    uint8_t byte;
    seed s;
    for(int i=0; i<4; i++){
        ReadProcessMemory(phandle,(void*)location+i-0x5F8,&byte,sizeof(char),NULL);
        s.set(i, byte);
    }
    if(s.to_str() != cur_seed.to_str()){
        cout << endl << "CORRUPTED at 0x" << hex << location-0x5F8 << ": " << s.to_str() << endl << "cur_seed: " << cur_seed.to_str() << endl;
        last_error = "Seed might be corrupted...";
        success = false;
        return type;
    }

    string type_str;
    int i = 0;
    while(byte){
        ReadProcessMemory(phandle,(void*)location+i-0x5F8+0x34,&byte,sizeof(char),NULL);
        if(byte) type_str += byte;
        i++;
    }

    /// SEARCH ALGORITHM
    bool located = false;
    i = 0;
    int progression = 0;
    uint32_t address = 0x10000000;
    while(!located){
        ReadProcessMemory(phandle,(void*)address-i,&byte,sizeof(char),NULL);
        bool skip = true;
        if(byte == type_str[0]){
            located = true;
            for(unsigned int j=0; j<type_str.size(); j++){
                ReadProcessMemory(phandle,(void*)address-i+j,&byte,sizeof(char),NULL);
                //if(address-i == 0xd790750) cout << byte << " " << type_str[type_str.size()-1-j] << endl;
                if(byte != type_str[j]){
                    located = false;
                    break;
                }
            }
        }

        for(unsigned int j=0; j<type_str.size(); j++)
            if(byte == type_str[j]) skip = false;

        //if(address-i < 0xd790800 && address-i > 0xd790600) cout << "LAC: " << address-i << endl;

        if(located){
            skip = false;
            //cout << "loc: " << address-i << endl;
            for(int j=0; j<4; j++){
                ReadProcessMemory(phandle,(void*)address-i+j-0x34,&byte,sizeof(char),NULL);
                if(byte != cur_seed[j]) located = false;
                //cout << "   byte: " << hex << int(byte) << "   seed[i]: " << int(cur_seed[j]) << endl;
            }
            if(located) location_2 = address-i;
        }
        //if((address-i)%0x100000 == 0) cout << "0x" << hex << address-i << endl;
        i++;
        if(skip) i += type_str.size() - 1;
        if(address-i < 0x100000){
            last_error = "Challenge type not found!";
            success = false;
            return type;
        }

        if(i/0x290000 != progression && progression < 50){
            progression++;
            ostringstream str;
            str << "Loading challenge " << progression+50 << "%";
            status_bar.set(str.str().c_str());
            Fl::flush();
        }
        else if(progression == 50){
            progression++;
            status_bar.set("Please wait...");
            Fl::flush();
        }
    }

    location_2 -= 0x34;

    type.distance = get_float(location_2 + 0xC);
    type.limit = get_float(location_2 + 0x10);

    cout << "Success!" << endl << "  (location: 0x" << hex << uppercase << location_2 << ")" << endl;

    /// PIT
    if(type_str == "challenge_spikyroad_default_normal.isg"){
        type.level = PIT;
        if(type.distance == -1) type.event = DISTANCE;
        else type.event = SPEED;
        type.difficulty = NORMAL;
    }
    else if(type_str == "challenge_spikyroad_default_expert.isg"){
        type.level = PIT;
        if(type.distance == -1) type.event = DISTANCE;
        else type.event = SPEED;
        type.difficulty = EXPERT;
    }
    else if(type_str == "challenge_spikyroad_lumsattack_normal.isg"){
        type.level = PIT;
        type.event = LUMS;
        type.difficulty = NORMAL;
    }
    else if(type_str == "challenge_spikyroad_lumsattack_expert.isg"){
        type.level = PIT;
        type.event = LUMS;
        type.difficulty = EXPERT;
    }

    /// LOTLD
    else if(type_str == "challenge_run_default_normal.isg"){
        type.level = LOTLD;
        type.event = DISTANCE;
        type.difficulty = NORMAL;
    }
    else if(type_str == "challenge_run_default_expert.isg"){
        type.level = LOTLD;
        type.event = DISTANCE;
        type.difficulty = EXPERT;
    }
    else if(type_str == "challenge_run_lumsattack_normal.isg"){
        type.level = LOTLD;
        type.event = LUMS;
        type.difficulty = NORMAL;
    }
    else if(type_str == "challenge_run_lumsattack_expert.isg"){
        type.level = LOTLD;
        type.event = LUMS;
        type.difficulty = EXPERT;
    }
    else if(type_str == "challenge_run_timeattack_normal.isg"){
        type.level = LOTLD;
        type.event = SPEED;
        type.difficulty = NORMAL;
    }
    else if(type_str == "challenge_run_timeattack_expert.isg"){
        type.level = LOTLD;
        type.event = SPEED;
        type.difficulty = EXPERT;
    }

    /// TOWER
    else if(type_str == "challenge_goingup_default_expert.isg"){
        type.level = TOWER;
        if(type.distance == -1) type.event = DISTANCE;
        else type.event = SPEED;
        type.difficulty = EXPERT;
    }

    /// MURPHY
    else if(type_str == "challenge_drc_castle_default_normal.isg"){
        type.level = MURPHY;
        type.event = DISTANCE;
        type.difficulty = NORMAL;
    }
    else if(type_str == "challenge_drc_castle_default_expert.isg"){
        type.level = MURPHY;
        type.event = DISTANCE;
        type.difficulty = EXPERT;
    }
    else if(type_str == "challenge_drc_castle_lumsattack_expert.isg"){
        type.level = MURPHY;
        type.event = LUMS;
        type.difficulty = EXPERT;
    }

    return type;
}

float process::get_float(uint32_t address)
{
    uint32_t num = 0;
    for(int i=0; i<4; i++){
        num *= 0x100;
        uint8_t byte;
        ReadProcessMemory(phandle,(void*)address+i,&byte,sizeof(char),NULL);
        num += byte;
    }
    num = bundle::swap(num);
    float distance = *((float*)&num);
    return distance;
}

uint32_t process::float_to_uint32(float f)
{
    uint32_t num = *((uint32_t*)&f);
    return num;
}

bool process::change_seed(seed new_seed)
{
    cout << "Writing seed to process memory" << endl;

    unsigned int address = location;

    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    for(int i=0; i<4; i++){
        int byte = new_seed[i];
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
    }
    cout << "Success!" << endl;

    address -= 0x5F8;
    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    for(int i=0; i<4; i++){
        int byte = new_seed[i];
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
    }
    cout << "Success!" << endl;

    address = location_2;
    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    for(int i=0; i<4; i++){
        int byte = new_seed[i];
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
    }
    cout << "Success!" << endl;
    cout << "Seed has been changed successfully!" << endl;
    return true;
}


bool process::change_seed_dojo(seed cur_seed, seed new_seed)
{
    cout << "Writing seed to process memory";

    cout << endl << "Please wait...";

    bool located = false; // true if the correct string has been found
    unsigned int address = 0;
    unsigned char buffer[0x1000];

    int occur_cnt = 0;

    for(int i=0; i<0x100000; i++)
    {
        address = i*0x1000;
        if(!ReadProcessMemory(phandle,(void*)address,&buffer,sizeof(buffer),NULL)) continue; // continue if the area is inaccessible
        for(unsigned int j=0; j<sizeof(buffer); j++)
        {
            if(buffer[j] == cur_seed[0]){
                for(int k=1; k<4; k++){ // we check if we found the current seed
                    if(buffer[j+k] == cur_seed[k]) located=true;
                    else{
                        located = false;
                        break;
                    }
                }
            }

            if(located)
            {
                occur_cnt++;
                gotoxy(1,wherey());
                cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
                // we write the seed at the address
                for(int k=0; k<4; k++){
                    int byte = new_seed[k];
                    if(!WriteProcessMemory(phandle,(void*)address+k,&byte,1,NULL)){
                        last_error = "Could not write to process memory";
                        success = false;
                        return false;
                    }
                    else success = true;
                }
                if(success) cout << "Success!" << endl << "Please wait...";
                located = false;
            }
            if(address > location+10) break;
            address++;
        }
        if(address > location+10) break;
    }

    if(!occur_cnt){
        last_error = "Could not change the seed!";
        success = false;
        return false;
    }

    gotoxy(1,wherey());
    cout << "Seed has been changed successfully!" << endl;
    return true;
}

bool process::change_distance(float distance)
{
    uint32_t num = float_to_uint32(distance);

    cout << "Writing goal to process memory" << endl;

    unsigned int address = location - 0x5F8 + 0xC;

    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    uint32_t u = num;
    for(int i=0; i<4; i++){
        uint8_t byte = u % 0x100;
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
        u /= 0x100;
    }
    cout << "Success!" << endl;

    address = location_2 + 0xC;
    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    u = num;
    for(int i=0; i<4; i++){
        uint8_t byte = u % 0x100;
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
        u /= 0x100;
    }
    cout << "Success!" << endl;
    cout << "Goal has been changed successfully!" << endl;
    return true;
}

bool process::change_limit(float limit)
{
    uint32_t num = float_to_uint32(limit);

    cout << "Writing time limit to process memory" << endl;

    unsigned int address = location - 0x5F8 + 0x10;

    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    uint32_t u = num;
    for(int i=0; i<4; i++){
        uint8_t byte = u % 0x100;
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
        u /= 0x100;
    }
    cout << "Success!" << endl;

    address = location_2 + 0x10;
    cout << "at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... ";
    u = num;
    for(int i=0; i<4; i++){
        uint8_t byte = u % 0x100;
        if(!WriteProcessMemory(phandle,(void*)address+i,&byte,1,NULL)){
            last_error = "Could not write to process memory";
            success = false;
            return false;
        }
        else success = true;
        u /= 0x100;
    }
    cout << "Success!" << endl;
    cout << "Time limit has been changed successfully!" << endl;
    return true;
}

void process::reset(void)
{
    success = true;
    location = 0;
    location_2 = 0;
}


process::operator bool() const
{
    return success;
}

string process::get_last_error(void)
{
    return last_error;
}

HANDLE process::get_handle(const std::string& process_name)
{
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);

    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);

    if (Process32First(snapshot, &entry) == TRUE)
    {
        while (Process32Next(snapshot, &entry) == TRUE)
        {
            if (stricmp(entry.szExeFile, process_name.c_str()) == 0)
                return OpenProcess(PROCESS_ALL_ACCESS, FALSE, entry.th32ProcessID);
        }
    }
    return NULL;
}
