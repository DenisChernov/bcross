/* 
 * File:   engine.h
 * Author: demiin
 *
 * Created on March 19, 2014, 5:37 PM
 */

#ifndef ENGINE_H
#define	ENGINE_H

#include "fOper.h"
#include <cstdlib>
#include <boost/asio.hpp>
#include <string>
#include <iostream>
#include <boost/system/system_error.hpp>
#include <map>
#include <boost/date_time.hpp>

using namespace std;
using boost::asio::ip::tcp;

class engine {
public:
    engine();
    engine(map<string, string>);
    virtual ~engine();
    
    void makeConn(string server);
    void prepareServerMap(string rqst);
    void getBookFromSite(string filename);
    void setCurrentISBN(string isbn);
    bool neededReloadPagebyNewPath() {return (newPath != "" ? true : false);};
    void clearNewPath() {newPath = "";};
    bool needSearchByBookName();
    void clearSearchByBookName() {needSearchByName = false;};
    /**************************************************************************/
   
    void getBooksIRBIS64();
    void getBookByISBN_IRBIS64();
    map <string, vector<string>> books;    
    bool allFound = false;
private:
    string maxMFN_CMPL = "1";
    string maxMFN_BOOKBASE = "1";
    const string MAGIC_CODE = "31771";
    size_t reqCounter;
    boost::asio::io_service io_service;
    map <string, string> bookStorage;
    map <string, string> responseTypes;
    string currentISBN = "";
    string irbis64_login    = "irbisoft";
    string irbis64_password = "9f9@7Nuq";
    string irbis64_countAnswers = "1000";
    string server64         = "192.168.9.249";
    string irbis64_CMPL_BASE = "CMPL";
    string irbis64_BOOK_BASE = "OLDEK";
    map <string, string> ksu;
    map <string, pair<string, string>> fullDescriptionBooks;
    //map <string, string> serverMap;
    string requestString_ozon = "?context=search&text=";
    string requestString_BooknameAndFIO_ozon = "?context=search&group=div_book&text=";
    string newPath = "";
    bool needSearchByName = false;
    
    /*
     *  Возвращает текущую дату вида ГГГГММ
     */
    string curdate();
    
    
    /*
     *  tcp::socket& socket - ранее установленное соедениение через сокет, 
     *                        с помощью которого происходит общение с сервером
     */
    void makeRequest(tcp::socket* socket, string server);
    
    /*
     *  tcp::socket& socket - ранее установленное соедениение через сокет, 
     *                        с помощью которого происходит общение с сервером
     * 
     *  возвращается весь ответ типа string
     */
    string getResponse(tcp::socket* socket, string server);
    
    bool validateResponse(tcp::socket* socket, boost::asio::streambuf* response);
    
    /**************************************************************************/
    void login_unlogin(string codeAPM, string codeCommand);
    
    
    void getBookList();
    string sendRequest(boost::asio::streambuf* request);
    void getMaxMFN();
    string recive();
    
    string maxKSUDate(map <string, string> ksu);
    
};

#endif	/* ENGINE_H */
