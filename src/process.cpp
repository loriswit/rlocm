#include "process.h"

using namespace std;

/****************************************/
/// class process
/****************************************/

process::process(const std::string& process_name)
{
    success = true;
    last_location = 0;

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
        last_error = "Process not found!";
        success = false;
        return;
    }

    cout << "Success!" << endl;
}

seed process::get_seed(bool type)
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
    if(type == DEFAULT) str = "countdown.act";
    if(type == DOJO) str = "countdown_shaolin.act";

    bool located = false; // true if the correct string has been found
    unsigned int address = 0;
    unsigned char buffer[0x1000];

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
                if(type == DEFAULT)
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
    }

    // we finalize the progress bar
    gotoxy(18,wherey());
    for(int i=0; i<16; i++) cout << char(0xB2);
    _setcursortype(_NORMALCURSOR);

    // now it's time to read the seed, which is located some bytes before the string that has been found
    seed s;
    int pos;
    if(type == DEFAULT) pos = 12;
    else pos = 116;

    if(located){
        cout << " Success!" << endl << "  SEED: ";
        for(int i=0; i<4; i++){
            char byte;
            ReadProcessMemory(phandle,(void*)address-pos+i,&byte,sizeof(char),NULL);
            s.set(i, byte);
            cout <<  hex << uppercase << setw(2) << setfill('0') << s[i] << " ";
        }
        last_location = address-pos;
        cout << "(location: 0x" << hex << uppercase << last_location << ")" << endl;
    }
    else{
        last_error = "Seed not found!";
        success = false;
    }

    return s;
}

bool process::change_seed(seed cur_seed, seed new_seed)
{
    cout << "Writing seed to process memory";

    /*if(!bund.check_training()){
        last_error = "You must be playing in the training room to change the seed!";
        success = false;
        return false;
    }*/

    cout << endl << "Please wait...";

    bool located = false; // true if the correct string has been found
    unsigned int address = 0;
    unsigned char buffer[0x1000];

    int occur_cnt = 0;

    #if DEBUG
        ofstream ofs("temp.txt", ios::out|ios::trunc);
        ofs << "start" << endl << endl;
    #endif

    for(int i=0; i<0x100000; i++)
    {
        address = i*0x1000;
        #if DEBUG
            ofs << "  i = 0x" << hex << i << endl;
            ofs << "  address = 0x" << hex << address << endl;
            ofs << "  ReadProcessMemory..." << endl;
        #endif
        if(!ReadProcessMemory(phandle,(void*)address,&buffer,sizeof(buffer),NULL)) continue; // continue if the area is inaccessible

        #if DEBUG
            ofs << "  sizeof(buffer) = 0x" << hex << sizeof(buffer) << endl;
        #endif

        for(unsigned int j=0; j<sizeof(buffer); j++)
        {
            #if DEBUG
                ofs << "    j = 0x" << hex << j << endl;
                ofs << "    buffer[j] = 0x" << hex << int(buffer[j]) << endl;
                ofs << "    cur_seed[0] = 0x" << hex << cur_seed[0] << endl;
            #endif
            if(buffer[j] == cur_seed[0]){
                #if DEBUG
                        ofs << "      buffer[j] = cur_seed[0]" << endl;
                #endif
                for(int k=1; k<4; k++){ // we check if we found the current seed
                    #if DEBUG
                        ofs << "        k = 0x" << hex << k << endl;
                        ofs << "        buffer[j+k] = 0x" << hex << int(buffer[j+k]) << endl;
                        ofs << "        cur_seed[k] = 0x" << hex << cur_seed[k] << endl;
                    #endif
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
                #if DEBUG
                    ofs << "      located = true" << endl;
                    ofs << "      occur_cnt = " << occur_cnt << endl;
                    ofs << "      cout: at 0x" << hex << uppercase << setw(8) << setfill('0') << address << "... " << endl;
                #endif
                // we write the seed at the address
                for(int k=0; k<4; k++){
                    int byte = new_seed[k];
                    #if DEBUG
                        ofs << "        k = 0x" << hex << k << endl;
                        ofs << "        new_seed[k] = 0x" << hex << new_seed[k] << endl;
                        ofs << "        WriteProcessMemory..." << endl;
                    #endif
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
            if(address > last_location+10) break;
            address++;
        }
        if(address > last_location+10) break;
    }

    #if DEBUG
        ofs << "end" << endl;
        ofs.close();
    #endif

    if(!occur_cnt){
        last_error = "Could not change the seed!";
        success = false;
        return false;
    }

    gotoxy(1,wherey());
    cout << "Seed has been written successfully!" << endl;
    return true;
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
