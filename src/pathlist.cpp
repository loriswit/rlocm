#include "pathlist.h"

using namespace std;

pathlist::pathlist(const string& folder)
{
    vector<string> folders;
    folders.push_back(folder);

    // we read all filepaths located in the specified 'folder'
    while(folders.size())
    {
        DIR *dir;
        struct dirent *ent;
        if((dir = opendir(folders.back().c_str())) != NULL)
        {
            string cur_folder = folders.back();
            folders.pop_back();
            int i=0;
            while((ent = readdir(dir)) != NULL)
            {
                if(i<2){
                    i++;
                    continue;
                }
                string path = cur_folder;
                if(path[path.size()-1] != '/') path += "/";
                path += string(ent->d_name);
                struct stat st;
                stat(path.c_str(), &st);
                if(S_ISDIR(st.st_mode)) folders.push_back(path);
                else{
                    path.erase(0,folder.size()+1);
                    list.push_back(path);
                }
                i++;
            }
            closedir(dir);
        }
        else{
            success = false;
            break;
        }
    }
}

size_t pathlist::size(void)
{
    return list.size();
}

string pathlist::operator[](streampos pos)
{
    return list[pos];
}

pathlist::operator bool() const
{
    return success;
}
