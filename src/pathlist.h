#ifndef PATHLIST_H
#define PATHLIST_H

#include "const.h"

// This give all the file paths in a specified folder
/// CURRENTLY UNUSED

class pathlist
{
    public:
        pathlist(const std::string& folder);
        size_t size(void);

        operator bool() const;
        std::string operator[](std::streampos pos);

    private:
        std::vector<std::string> list;
        bool success;
};

#endif // PATHLIST_H
