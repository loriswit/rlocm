#ifndef PROCESS_H
#define PROCESS_H

#include "seed.h"
#include "const.h"

class process
{
    public:
        process(const std::string& process_name);
        seed get_seed(bool type);
        bool change_seed(seed cur_seed, seed new_seed);

        operator bool() const;
        std::string get_last_error(void);

        static HANDLE get_handle(const std::string& process_name);

    private:

        HANDLE phandle;
        bool success;
        unsigned int last_location;

        std::string last_error;
};

#endif // PROCESS_H
