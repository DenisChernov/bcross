/* 
 * File:   parsing.h
 * Author: demiin
 *
 * Created on March 21, 2014, 10:43 AM
 */

#ifndef PARSING_H
#define	PARSING_H


#include <boost/regex.hpp>
#include <string>
#include <iostream>
#include <boost/lexical_cast.hpp>
#include "fOper.h"

using namespace std;

enum pars_vars {OBJ_MOVED, LOCATION};

class parsing {
public:
    parsing(string line_for_regex);
    virtual ~parsing();
    
    string parseLine(pars_vars mode);
    string maxMFN();
    map <string, string> getDeliveredKSU();
    vector <string> split();
    pair<string, vector<string>> parseBookRecord(string ksu);
    string getAddrFullCover();
    string getAnnotation();
    string linkToNewAddrBook_fromMany();
    bool   nothingFindByISBN();
    string getBookNameAndFIO();
    
    string &operator = (const string new_line_for_regex) {line = new_line_for_regex;}
private:
    string line;
    boost::regex re;
    boost::smatch result;
};

#endif	/* PARSING_H */

