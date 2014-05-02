/*
An Internet File Donloader
Can download (large) files from internet and resume the download after an
interuption

Based on an example from the book "The Art of C++" by Herbert Schildt

Jos de Jong, sept 2007
c++ program

*/

// Header file for downloader.


#ifndef DOWNLOAD_H
#define DOWNLOAD_H

//#include <iostream>
#include "../../src/const.h"

using namespace std;

const int MAX_ERRMSG_SIZE = 80;
const int MAX_FILENAME_SIZE = 512;
const int BUF_SIZE = 10240;             // 10 KB


// Exception class for donwload errors;
class DLExc
{
private:
    char err[MAX_ERRMSG_SIZE];
public:
    DLExc(char *exc)
    {
        if(strlen(exc) < MAX_ERRMSG_SIZE)
            strcpy(err, exc);
    }

    // Return a pointer to the error message
    const char *geterr()
    {
        return err;
    }
};


// A class for downloading files from the internet
class Download
{
private:
    static bool ishttp(const char *url);
    static bool httpverOK(HINTERNET hIurl);
    static bool getfname(char *url, char *fname);
    static unsigned long openfile(char *url, bool reload, ofstream &fout);
public:
    static std::vector<uint8_t> download(const char *url, bool reload=false, void (*update)(unsigned long, unsigned long)=NULL);
};


#endif

