/* 
 * File:   fOper.h
 * Author: demiin
 *
 * Created on March 21, 2014, 9:19 AM
 */

#ifndef FOPER_H
#define	FOPER_H

#include <string>
#include <fstream>
#include <iostream>
#include <boost/asio.hpp>

using namespace std;

enum open_mode {OPEN, WRITE, APPEND};

class fOper {
public:
    fOper();
    fOper(string filename, open_mode mode);
    virtual ~fOper();
    
    string fRead();
    void fWrite(string data);
    void fClose();
    void reopen(string filename, open_mode mode);
    bool eof();
    bool is_open_read()  { return in.is_open(); };
    bool is_open_write() { return out.is_open(); };
    
private:
    ifstream in;
    ofstream out;
    
};

#endif	/* FOPER_H */

