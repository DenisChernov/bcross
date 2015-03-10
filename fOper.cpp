/* 
 * File:   fOper.cpp
 * Author: demiin
 * 
 * Created on March 21, 2014, 9:19 AM
 */

#include "fOper.h"

fOper::fOper() 
{
    
}

fOper::fOper(string filename, open_mode mode) 
{
    switch (mode)
    {
        case OPEN :
        {
            in.open(filename.c_str(), ios_base::in);
            break;
        }
        case WRITE :
        {
            out.open(filename.c_str(), ios_base::out);
            break;
        }
        case APPEND :
        {
            out.open(filename.c_str(), ios_base::out | ios_base::app);
            break;
        }
            
    }
}

void fOper::reopen(string filename, open_mode mode) 
{
    switch (mode)
    {
        case OPEN :
        {
            in.open(filename.c_str(), ios_base::in | ios_base::binary);
            break;
        }
        case WRITE :
        {
            out.open(filename.c_str(), ios_base::out);
            break;
        }
    }
}




fOper::~fOper() {
}

string fOper::fRead() 
{
    string line;
    getline(in, line);
    return line;
}

void fOper::fWrite(string data) 
{
    out << data;
}

void fOper::fClose() 
{
    if (in.is_open())
        in.close();
    if (out.is_open())
        out.close();
}

bool fOper::eof() 
{
    return in.eof();
}
