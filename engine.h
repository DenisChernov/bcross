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
#include <vector>
#include <ctime>
#include "parsing.h"        
#include <boost/foreach.hpp>
#include <iconv.h>

using namespace std;
using boost::asio::ip::tcp;

struct bookinfo
{
    string annotation;
    string coverPath;
    string qrcode;
    string pagename;
};

class engine {
public:
    engine();
    engine(map<string, string>);
    virtual ~engine();
    
    void makeConn(string server);
    void getBookDataFromSite(string server);
    void prepareServerMap(string rqst);
    void getBookFromSite(string filename);
    void setCurrentISBN(string isbn);
    bool neededReloadPagebyNewPath() {return (newPath != "" ? true : false);};
    void clearNewPath() {newPath = "";};
    bool needSearchByBookName();
    void clearSearchByBookName() {needSearchByName = false;};
    void getForm_Token_ID();

    /**************************************************************************/
   
    void getCurrentBookRecord();    
    void getBooksIRBIS64();
    void getBookByISBN_IRBIS64();
//    map <string, vector<string>> books;    
    vector <books_record> books;
    bool allFound = false;
    bookinfo book;
    string generatePagesNames(int count);
    void generateQRcodes(string pagename);    
    void updateBookRecord(string record);

private:
    string maxMFN_CMPL = "1";
    string maxMFN_BOOKBASE = "1";
    string maxMFN_BCROSS = "1";
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
    string irbis64_BCROSS_BASE = "BCROSS";
    map <string, string> ksu;
    map <string, pair<string, string>> fullDescriptionBooks;
    //map <string, string> serverMap;
    string requestString_ozon = "?context=search&text=";
    string requestString_BooknameAndFIO_ozon = "?context=search&group=div_book&text=";
    string newPath = "";
    bool needSearchByName = false;
    string form_id = "";
    string form_token = "";
    //string login_site = "name=Demiin&pass=pr0tsa5h&form_build_id=form-5t1KVQjrfEmrit8Rm3W6YfzBcqkjccZA3nwtDFkSLYg&form_id=user_login_block&op=%D0%92%D1%85%D0%BE%D0%B4+%D0%B2+%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D1%83";
    string login_site = "name=Demiin&pass=pr0tsa5h&form_id=user_login_block&op=%D0%92%D1%85%D0%BE%D0%B4+%D0%B2+%D1%81%D0%B8%D1%81%D1%82%D0%B5%D0%BC%D1%83";
    //string cookie_magic = " __utma=6607406.1397039187.1397812446.1398068709.1398071274.5; __utmz=6607406.1397812446.1.1.utmcsr=(direct)|utmccn=(direct)|utmcmd=referral|utmcct=/node; __utmv=6607406.authenticated%20user%2Cmoderator%2Cadmin; advpollvote1211=voted; Drupal.tableDrag.showWeight=0; _ga=GA1.2.314233222.1398083429; has_js=1; _ym_visorc_27906747=w; _ym_visorc_27906750=w; _ym_visorc_27423227=w; _gat=1";
    string cookie_magic = "has_js=1; _ga=GA1.2.438614576.1426181815; _gat=1; _ym_visorc_27916745=w; _ym_visorc_27916755=w; _ym_visorc_27433225=w";
    string cookie = "";
    string addPage = "";
    string formBody;
    vector <string> booklist;
    vector <string> generatedPageNames;
    vector <string> qrcodes;

    
    /*
     *  Возвращает текущую дату вида ГГГГММ
     */
    string curdate();
    
    
    /*
     *  tcp::socket& socket - ранее установленное соедениение через сокет, 
     *                        с помощью которого происходит общение с сервером
     */
    void makeRequest(tcp::socket* socket, string server);
    void makeRequest_get(tcp::socket* socket, string server);
    void makeRequest_post(tcp::socket* socket, string server);
    void makeRequest_addPage(tcp::socket* socket, string server);
    void makeRequest_getAddPage(tcp::socket* socket, string server);
    
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
    

    vector<string> genPageName(int count);
    void remakeBooklist();

    void generateQRcodes(int count);
    void makePageBody(string img, string annotation, string qrcode);
    
    string iconv_recode(string from, string to, string text);
};

#endif	/* ENGINE_H */
