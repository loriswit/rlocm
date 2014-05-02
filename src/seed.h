#ifndef SEED_H
#define SEED_H

#include "const.h"

class seed
{
    public:
        seed(int _a, int _b, int _c, int _d):
            a(_a), b(_b), c(_c), d(_d) {}
        seed(const std::string& _str);
        seed(void): a(0), b(0), c(0), d(0) {}

        int operator[](size_t pos);

        std::string to_str(void);
        void set(size_t pos, int i);

    private:
        unsigned char a; // the 4 bytes of the seed
        unsigned char b;
        unsigned char c;
        unsigned char d;
};

#endif // SEED_H
