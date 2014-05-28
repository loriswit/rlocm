#include "bundle.h"

using namespace std;

bundle::bundle(const string& bundle_name)
{
    is_open = false;
    success = false;
    open(bundle_name);
}

bundle::operator bool() const
{
    return success;
}

void bundle::open(const string& bundle_name)
{
    //cout << "Opening " << bundle_name << "... ";
    filename = bundle_name;
    if(is_open) file.close();
    file.open(filename.c_str(), ios::in|ios::out|ios::binary);
    if(!file){
        last_error = "Could not open '" + filename +"'!";
        is_open = false;
        success = false;
    }
    else{
        is_open = true;
        file.seekg (0, file.end);
        streampos length = file.tellg();
        file.seekg (0, file.beg);
        if(length < 0x30){
            last_error = "File '" + filename + "' is an invalid bundle!";
            success = false;
            return;
        }

        success = true;
        //cout << "Success!" << endl;
        base_off = get_long(0x0C); // we read the base off
        files = get_long(0x2C); // we read the number of files

        if(length < base_off + 0x30){
            last_error = "File '" + filename + "' might be corrupted!";
            success = false;
            return;
        }
    }
}

file_info bundle::seek(const string& filepath, bool out)
{
    file.seekg(0x30);

    file_info fi;

    // this is based on 'rayman_legends.bms' script
    for(unsigned int i=0; i<files; i++)
    {
        uint32_t dummy1 = get_long();
        uint32_t size = get_long();
        uint32_t zsize = get_long();
        //uint64_t tstamp = get_longlong();
                            get_longlong();
        uint64_t offset = get_longlong();

        if(dummy1 == 2){
            //uint32_t dummy4 = get_long();
                                get_long();
            //uint32_t dummy5 = get_long();
                                get_long();
        }

        uint32_t foldersz = get_long();
        string folder = get_string(foldersz);
        uint32_t namesz = get_long();
        string name = get_string(namesz);

        folder += name;
        name = folder;
        offset += base_off;

        if(!file) success = false;

        if(name == filepath){
            if(out){
                if(zsize == 0) cout << name << endl << " at 0x" << hex << uppercase << setw(16) << setfill('0') << offset << " size: 0x" << size << " bytes" << endl << endl;
                          else cout << name << endl << " at 0x" << hex << uppercase << setw(16) << setfill('0') << offset << " size: 0x" << size << " compressed_size: 0x" << zsize << endl << endl;
            }
            fi.name = name;
            fi.offset = offset;
            fi.size = size;
            fi.zsize = zsize;
            fi.ok = true;
            return fi;
        }

        //uint32_t dummy6 = get_long();
                            get_long();
        //uint32_t dummy7 = get_long();
                            get_long();
    }

    // if failed
    fi.offset = 0;
    fi.size = 0;
    fi.zsize = 0;
    fi.ok = false;
    return fi;
}

bool bundle::check_training(void)
{
    #if DEV_MODE
    return true;
    #else
    vector<uint8_t> data;
    data = load_resource(INPUT_MENU_X360);
    file.seekg(seek(FILE_LIST[0]).offset);

    for(unsigned int i=0; i<data.size(); i++)
        if(file.get() != data[i]) return false;

    data = load_resource(CHALLENGE_ENDLESS);
    file.seekg(seek(FILE_LIST[1]).offset);
    for(unsigned int i=0; i<data.size(); i++)
        if(file.get() != data[i]) return false;

    return true;
    #endif
}

bool bundle::check_dojo_mod(void)
{
    vector<uint8_t> data;
    data = load_resource(CHALLENGE_SHAOLIN_EXPERT);
    file.seekg(seek(SHAOLIN_PATH).offset);

    for(unsigned int i=0; i<data.size(); i++)
        if(file.get() != data[i]) return false;

    return true;
}

bool bundle::install_training_room(int resource)
{
    if(resource == 101) cout << "Installing the training room...";
    else cout << "Uninstalling the training room...";

	if(process::get_handle(PROCESS_NAME) != NULL){
	    if(resource == 101) last_error = "Could not install the training room while '" + PROCESS_NAME + "' is running!";
	    else last_error = "Could not uninstall the training room while '" + PROCESS_NAME + "' is running!";
	    success = false;
	    return false;
	}

    vector<uint8_t> vec;

    file.seekg(0, file.end);
    streampos length = file.tellg();
    file.seekg(0, file.beg);

    for(int i=0; i<9; i++)
    {
        vec = load_resource(resource+i);
        uint64_t address = seek(FILE_LIST[i]).offset;
        cout << endl << "File " << i+1 << "/9 | size: 0x" << hex << uppercase << setw(6) << setfill('0') << vec.size() << " bytes | address: 0x" << address;

        if(length < address + vec.size()){
            last_error = "File '" + filename + "' might be corrupted!";
            success = false;
            return false;
        }

        file.seekg(address);
        for(unsigned int j=0; j<vec.size(); j++) file.put(vec[j]);
        if(!file) break;
    }

    cout << endl;

    if(!file){
        last_error = "Could not write into file 'Bundle_PC.ipk'!";
	    success = false;
	    return false;
    }

    if(resource == 101) cout << "Training room has been installed successfully!" << endl;
    else cout << "Training room has been uninstalled successfully!" << endl;
    return true;
}

bool bundle::uninstall_training_room(void)
{
    return install_training_room(111);
}

bool bundle::install_dojo_mod(int resource)
{
    if(resource == 121) cout << "Installing the Dojo mod for the tournament...";
    else cout << "Uninstalling the Dojo mod for the tournament...";

    if(!check_training() && resource == 121){
	    last_error = "Could not install the Dojo mod if the training room is not installed!";
	    success = false;
	    return false;
	}

    vector<uint8_t> vec;

    file.seekg(0, file.end);
    streampos length = file.tellg();
    file.seekg(0, file.beg);

    vec = load_resource(resource);
    uint64_t address = seek(SHAOLIN_PATH).offset;
    cout << endl << "File 1/1 | size: 0x" << hex << uppercase << setw(6) << setfill('0') << vec.size() << " bytes | address: 0x" << address;

    if(length < address + vec.size()){
        last_error = "File '" + filename + "' might be corrupted!";
        success = false;
        return false;
    }

    file.seekg(address);
    for(unsigned int j=0; j<vec.size(); j++) file.put(vec[j]);

    cout << endl;

    if(!file){
        last_error = "Could not write into file 'Bundle_PC.ipk'!";
	    success = false;
	    return false;
    }

    if(resource == 121) cout << "The Dojo mod for the tournament has been installed successfully!" << endl;
    else cout << "The Dojo mod for the tournament has been uninstalled successfully!" << endl;
    return true;
}

bool bundle::uninstall_dojo_mod(void)
{
    return install_dojo_mod(122);
}


vector<uint8_t> bundle::load_resource(int name)
{
    HMODULE handle = ::GetModuleHandle(NULL);
    HRSRC rc = ::FindResource(handle, MAKEINTRESOURCE(name),MAKEINTRESOURCE(10));
    HGLOBAL rcData = ::LoadResource(handle, rc);
    DWORD size = ::SizeofResource(handle, rc);
    const char *data = static_cast<char*>(::LockResource(rcData));

    vector<uint8_t> vec;
    for(unsigned int i=0; i<size; i++) vec.push_back(data[i]);
    return vec;
}

void bundle::close(void)
{
    is_open = false;
    file.close();
}

string bundle::get_last_error(void)
{
    return last_error;
}

/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////


// reads a 32 bits number and swap to Big Endian
uint32_t bundle::get_long(void)
{
    uint32_t i;
    file.read((char*)&i, sizeof(uint32_t));
    i = swap(i);
    return i;
}

uint32_t bundle::get_long(streampos pos)
{
    file.seekg(pos);
    return get_long();
}

// reads a 64 bits number and swap to Big Endian
uint64_t bundle::get_longlong(void)
{
    uint64_t i = get_long() * 0x100000000 + get_long();
    return i;
}

uint64_t bundle::get_longlong(streampos pos)
{
    file.seekg(pos);
    return get_longlong();
}

string bundle::get_string(streamsize n)
{
    char *buffer = new char[n];
    file.read(buffer, n);
    string str(buffer);
    str.erase(n,str.size()-n);
    delete[] buffer;
    return str;
}

string bundle::get_string(streamsize n, streampos pos)
{
    file.seekg(pos);
    return get_string(n);
}

// converts Big Endian to Little Endian and vice versa
unsigned int bundle::swap(unsigned  int i)
{
    unsigned int j = ((i>>24) % 0x100)
        + ((i>>16) % 0x100 * 0x100)
        + ((i>>8) % 0x100 * 0x10000)
        + (i % 0x100 * 0x1000000);

    return j;
}

/// /////////////////////////////////////////////////////////////////////////////////////////
/// /////////////////////////////////////////////////////////////////////////////////////////

/// CURRENTLY UNUSED METHODS

/**
void bundle::patch(const string& patch_folder)
{
    pathlist pl(patch_folder);
    vector<file_info> finf;
    for(unsigned int i=0; i<pl.size(); i++){
        file_info fi = seek(pl[i]);
        if(fi.ok) finf.push_back(fi);
    }

    cout << finf.size() << " files to read..." << endl;

    ofstream fout("fout.txt",ios::out|ios::trunc);

    for(unsigned int i=0; i<finf.size(); i++){
        fout << finf[i].name << endl << endl;
        vector<uint8_t> data;
        if(finf[i].zsize == 0) data = read_data(finf[i].offset, finf[i].size);
        else {
            cout << "a";
            cout << hex << finf[i].size << endl;
            data = decompress_data(finf[i].offset, finf[i].zsize, finf[i].size);
            cout << "a";
        }

        cout << "size: 0x" << hex << data.size() << " bytes" <<endl;
        write_file("Mod_2/"+finf[i].name, patch_folder+"/../", data);
        //else write_file("Mod_2"+finf[i].name, patch_folder+"/../", decompress_data(0x0000000020A790F9 , 0x2322F , 0x55624));

    }
}

vector<uint8_t> bundle::compress_file(const string& filename)
{
    ifstream ifs(filename.c_str());
    ifs.seekg(0, ifs.end);
    int in_data_size = ifs.tellg();
    ifs.seekg(0, ifs.beg);
    char in_data[in_data_size];
    ifs.read(in_data, in_data_size);
    ifs.close();

    vector<uint8_t> buffer;

    const size_t BUFSIZE = 128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = 0;
    strm.zfree = 0;
    strm.next_in = reinterpret_cast<uint8_t *>(in_data);
    strm.avail_in = in_data_size;
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    deflateInit(&strm, Z_DEFAULT_COMPRESSION);

    while(strm.avail_in != 0)
    {
        int res = deflate(&strm, Z_NO_FLUSH);
        assert(res == Z_OK);
        if(strm.avail_out == 0)
        {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    }

    int deflate_res = Z_OK;
    while(deflate_res == Z_OK)
    {
        if(strm.avail_out == 0)
        {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        deflate_res = deflate(&strm, Z_FINISH);
    }

    assert(deflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    deflateEnd(&strm);

    vector<uint8_t> out_data;
    out_data.swap(buffer);
    return out_data;
}

vector<uint8_t> bundle::decompress_data(streampos offset, size_t in_data_size, size_t out_data_size)
{
    char in_data[in_data_size];
    file.seekg(offset);
    file.read(in_data, in_data_size);

    vector<uint8_t> buffer;

    const size_t BUFSIZE = 16384;//128 * 1024;
    uint8_t temp_buffer[BUFSIZE];

    z_stream strm;
    strm.zalloc = 0;
    strm.zfree = 0;
    strm.next_in = reinterpret_cast<uint8_t *>(in_data);
    strm.avail_in = in_data_size;
    strm.next_out = temp_buffer;
    strm.avail_out = BUFSIZE;

    inflateInit(&strm);

    //while(strm.avail_in != 0)
    do
    {
        //cout << strm.avail_in << endl;
        int res = inflate(&strm, Z_NO_FLUSH);
        assert(res != Z_STREAM_ERROR);
        if(strm.avail_out == 0)
        {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
    //}
    }while(strm.avail_out == 0);


    int inflate_res = Z_OK;
    while(inflate_res == Z_OK)
    {
        if(strm.avail_out == 0)
        {
            buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE);
            strm.next_out = temp_buffer;
            strm.avail_out = BUFSIZE;
        }
        inflate_res = inflate(&strm, Z_FINISH);
    }

    //assert(inflate_res == Z_STREAM_END);
    buffer.insert(buffer.end(), temp_buffer, temp_buffer + BUFSIZE - strm.avail_out);
    inflateEnd(&strm);

    vector<uint8_t> out_data;
    out_data.swap(buffer);
    while(out_data.size() != out_data_size) out_data.pop_back();
    if(!file) success = false;
    return out_data;
    return buffer;
}

vector<uint8_t> bundle::read_data(streampos offset, size_t in_data_size)
{
    char in_data[in_data_size];
    file.seekg(offset);
    file.read(in_data, in_data_size);

    vector<uint8_t> out_data;
    for(unsigned int i=0; i<in_data_size; i++) out_data.push_back(in_data[i]);
    if(!file) success = false;
    return out_data;
}

void bundle::write_file(const string& filepath, const string& dir, const vector<uint8_t>& data)
{
    chdir(dir.c_str());

    string path = filepath;
    for(unsigned int i=0; i<path.size(); i++) if(path[i] == '/') path[i] = '\\';

    string folder;
    for(unsigned int i=0; i<path.size(); i++)
    {
        if(path[i] != '\\') folder += path[i];
        else{
            mkdir(folder.c_str());
            chdir(folder.c_str());
            folder = "";
        }
    }

    chdir(dir.c_str());

    ofstream ofs(filepath.c_str(), ios::out|ios::binary|ios::trunc);
    if(!ofs){
        cout << "SHIEAT" << endl;
        success = false;
        return;
    }

    for(unsigned int i=0; i<data.size(); i++){
        if(ofs.rdstate() & ofstream::failbit){
            cout << "HERE";
            system("pause");
        }
        ofs.put(data[i]);
    }
    if(!ofs) success = false;
    ofs.close();
}
*/
