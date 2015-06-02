/* 
 * File:   main.cpp
 * Author: demiin
 *
 * Created on April 21, 2014, 12:43 PM
 */

#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include "engine.h"
#include "parsing.h"

#include <stdio.h>
#include <stdlib.h> 
#include <string.h> 
#include <unistd.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h>



#include <boost/bind.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/enable_shared_from_this.hpp> 
#include <boost/asio.hpp>
#include <boost/thread.hpp>

//using namespace std;

void printStat(vector<books_record> books);
void parse_all_exist_base(engine* engn);

struct empty
{
    size_t count = 0;
    vector<size_t> record;
};

void printStat(vector<books_record> books)
{
    size_t counter = 1;
    size_t size = books.size();
    empty emptyF;
    empty emptyN;
    empty emptyISBN;
    vector <books_record>::iterator it = books.begin();
    while (it != books.end())
    {
        if (it->bookname == "")
        {
            emptyN.count++;
            emptyN.record.push_back(counter);
        }
        if (it->fio == "")
        {
            emptyF.count++;
            emptyF.record.push_back(counter);
        }
        if (it->isbn == "")
        {
            emptyISBN.count++;
            emptyISBN.record.push_back(counter);
        }
        counter++;
        it++;
    }
    
    cout << "размер: " << size << endl;
    cout << "кол-во без автора: " << emptyF.count << endl;
    cout << "кол-во без заглавия: " << emptyN.count << endl;            
    cout << "кол-во без isbn: " << emptyISBN.count << endl;    
}

void parse_all_exist_base(engine* engn)
{
//    engn->makeBookBody();
//    if (engn->books.size()  == 0)
//        engn->getBooksIRBIS64();
//    engn->getBookDataFromSite("www.ozon.ru");

//    printStat(engn->books);
    //map<string, vector<string>>::iterator it = engn->books.begin();
    vector <books_record>::iterator it = engn->books.begin();
//    for (int i = 0; i < 7; i++)
//        it++;
    size_t counter = 1;

//    while (it != engn->books.end())
//    {
//        cout << counter++ << ".\t" << (it->bookname != "" ? it->bookname : " ") << " : " << (it->fio != "" ? it->fio : " ") << endl;
        
        
        engn->book.annotation.clear();
        engn->book.coverPath.clear();
        engn->book.qrcode.clear();
        engn->book.autor.clear();
        engn->book.bookname.clear();
//        cout << it->bookrecord << endl;
        if (it->isbn != "")
//            if (it->isbn == "5-7020-0807-3")
        {
//            cout << "************************************** " << it->isbn << endl;
//            cout << endl;
        
            engn->prepareServerMap(it->isbn);
            engn->allFound = false;      
//            cout << "make conn" << endl;
            //engn->getBookDataFromSite("www.ozon.ru");
//            if (engn->neededReloadPagebyNewPath())
//            {
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(it->isbn);            
//            }            
            cout << "get books from site" << endl;
//            engn->getBookFromSite(it->isbn);
            
/*            if (!engn->needSearchByBookName() && !engn->neededReloadPagebyNewPath())
            {
                cout << "will search by name" << endl;
                engn->getBookByISBN_IRBIS64();
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(it->isbn);
 //               cout << "getted name and FIO" << endl;
                if (engn->neededReloadPagebyNewPath())
                {
                    cout << "ищем по новому пути" << endl;
                    engn->allFound = false;      
                    engn->getBookDataFromSite("www.ozon.ru");
                    engn->getBookFromSite(it->isbn);
                    cout << "finished" << endl;
                }
            }
            else*/
                if (engn->book.autor != "")
                    engn->notNeedNewPath = true;            
            if (engn->notNeedNewPath == false)
            {
                cout << "here reload page by new path" << endl;
                engn->allFound = false;      
                if (engn->book.autor != "")
                {
                    engn->notNeedNewPath = true;
                }
                    engn->getBookDataFromSite("www.ozon.ru");
                    engn->getBookFromSite(it->isbn);            
                
            }
            
            if (engn->notNeedNewPath == false)
            {
                cout << "here reload page by new path2" << endl;
                engn->allFound = false;      
                if (engn->book.autor == ""){
                    
                    engn->notNeedNewPath = true;
                
                    engn->getBookDataFromSite("www.ozon.ru");
                    engn->getBookFromSite(it->isbn);            
                }
            }            
            // конец сбора информации, пора заносить в базу и на сайт
            
            
            
//            break;
        }
        else
        {
//            cout << "************************************** " << it->bookname << "+" << it->fio << endl;
//            cout << endl;            
//            cout << "will search by name" << endl;
            engn->allFound = false;
            engn->prepareServerMap_BookFio(it->bookname + "%20" + it->fio);
            engn->getBookDataFromSite("www.ozon.ru");
            if (engn->neededReloadPagebyNewPath())
            {
                cout << "here reload page by new path" << endl;
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");            
            }            
            else 
                engn->getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");
//            cout << "getted name and FIO" << endl;
            if (engn->neededReloadPagebyNewPath())
            {
                cout << "ищем по новому пути" << endl;
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");
                cout << "finished" << endl;
            }
//            counter++;
            // конец сбора информации, пора заносить в базу и на сайт
            
            
        }

//        engn->book.qrcode   = engn->generateQRcodes(engn->book.pagename);
        if (engn->book.coverPath == "") {
            engn->book.coverPath = "www.vsedlyainformatikov.net/_ph/3/433388687.jpg";
            engn->book.annotation = "<pre>        <b>" + it->fio + "</b><br><i>                \"" + it->bookname + "</i>\"</pre>";
        }
        
        
        
        if (engn->book.coverPath != "")
        {
            cout << "genering full page" << endl;
            engn->book.pagename = engn->generatePagesNames(1);
            engn->generateQRcodes(engn->book.pagename);        
                        
            engn->getCurrentBookRecord();
            parsing pars(engn->books.front().bookrecord);
            cout << engn->books.front().bookrecord << endl;;
            size_t countRequest = 0;
            bool stopCurrentRequest = false;
            while (!pars.haveFioOrBookname()) {
                engn->getCurrentBookRecord();
                pars = engn->books.front().bookrecord;
                ++countRequest;
                if (countRequest == 15) {
                    stopCurrentRequest = true;
                    break;
                }
            }
            if (!stopCurrentRequest) {
                        engn->getCurrentBookRecord();
                        
                        cout << engn->books.front().bookrecord << endl;
                        //cout << "book record: " << engn->books.front() << endl;
                        
                        pars = it->bookrecord;
                        
                        
                        engn->updateBookRecord(pars.remakeBooklist()  + "\x1F" + "1#" + engn->book.bookname +"\x1F" +  "2#" + engn->book.autor +  "\x1F" + "11#http://murmanlib.ru/" + engn->book.pagename + "\x1F" + "10#\\\\192.168.6.8\\FileServerFolder\\qrcodes\\"+ engn->book.pagename + ".png\x1F\x0D\x0A");
                        //engn->updateBookRecord(pars.remakeBooklist()  + "\x1F" + "1#" + it->bookname +"\x1F" +  "2#" + it->fio +  "\x1F" + "11#http://murmanlib.ru/" + engn->book.pagename + "\x1F" + "10#\\\\192.168.6.8\\FileServerFolder\\qrcodes\\"+ engn->book.pagename + ".png\x1F\x0D\x0A");
                        
                        engn->makeConn("murmanlib.ru");
            }
        }
        else
        {
            cout << "genering template page" << endl;
            engn->book.pagename = engn->generatePagesNames(1);
            engn->generateQRcodes(engn->book.pagename);        
            engn->book.coverPath = "www.vsedlyainformatikov.net/_ph/3/433388687.jpg";
            engn->book.annotation = "<pre>        <b>" + it->fio + "</b><br><i>                \"" + it->bookname + "</i>\"</pre>";
                        engn->getCurrentBookRecord();
                        
                        cout << engn->books.front().bookrecord << endl;
                        //cout << "book record: " << it->bookrecord << endl;
                        parsing pars(it->bookrecord);
                        //parsing pars(engn->books.front().bookrecord);
                        
                        string sss = pars.remakeBooklist()  + "\x1F" + "1#" + engn->book.bookname +"\x1F" +  "2#" + engn->book.autor +  "\x1F" + "11#http://murmanlib.ru/" + engn->book.pagename + "\x1F" + "10#\\\\192.168.6.8\\FileServerFolder\\qrcodes\\"+ engn->book.pagename + ".png\x1F\x0D\x0A";
                        cout << "sss: " << sss << endl;
                        cout << "front: " << engn->books.front().bookrecord << endl;
                        cout << "it: " << it->bookrecord << endl;
                        engn->updateBookRecord(pars.remakeBooklist()  + "\x1F" + "1#" + engn->book.bookname +"\x1F" +  "2#" + engn->book.autor +  "\x1F" + "11#http://murmanlib.ru/" + engn->book.pagename + "\x1F" + "10#\\\\192.168.6.8\\FileServerFolder\\qrcodes\\"+ engn->book.pagename + ".png\x1F\x0D\x0A");
                        
                        engn->makeConn("murmanlib.ru");
        }
                
        //cout << "Аннотация: " << engn->book.annotation << endl;
        //cout << "Обложка:  "  << engn->book.coverPath << endl;
            


        
//        it++;

//    }    
}


using boost::asio::ip::tcp;
using namespace std;


class tcp_connection : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;
    
    static pointer create (boost::asio::io_service& io_service)
    {
        return pointer (new tcp_connection(io_service));
    }
    
    tcp::socket& socket()
    {
        return socket_;
    }
    
     tcp::socket socket_;
    
    void start(boost::array<char, 10096> buf)
    {
        cout << "async write" << endl;
            engine* engn;
            
            engn = new engine();
            books_record book;
  
            parsing pars(boost::lexical_cast<string>(buf));

            vector<string> tmp = pars.isbn_bookname_fio();
            book.isbn = tmp.at(0);
            book.bookname = tmp.at(1);
            book.fio = tmp.at(2);
        
            engn->books.push_back(book);
            parse_all_exist_base(engn);
            cout << "done" << endl;      
            
        
/*        boost::asio::async_write(socket_, boost::asio::buffer("insert answere here"),
                                                boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));*/
    }
    
private:
    tcp_connection(boost::asio::io_service& io_service) : socket_ (io_service)
    {
    }
    
    void handle_write(const boost::system::error_code&, size_t)
    {

        
    }
    
    
   
};


class tcp_server
{
public:
    tcp_server(boost::asio::io_service& io_service) : acceptor_(io_service, tcp::endpoint(tcp::v4(), 31773))
    {
        start_accept();
    }
    
private:
    void start_accept()
    {
        tcp_connection::pointer new_connection = tcp_connection::create(acceptor_.get_io_service());
        acceptor_.async_accept(new_connection->socket(), 
                                                boost::bind(&tcp_server::handle_accept, this, new_connection,
                                                boost::asio::placeholders::error));
   }
    
    void handle_accept(tcp_connection::pointer new_connection, const boost::system::error_code& error)
    {
        if (!error)
        {

            //getRecord(&new_connection);
            //new_connection.reset();
            boost::system::error_code error;
            boost::array< char, 10096 > buf;

            size_t length = boost::asio::read((*new_connection).socket_, boost::asio::buffer(buf, 10096), boost::asio::transfer_all(), error);
            new_connection->start(buf);        
        }
      //  start_accept();
    }
   
    tcp::acceptor acceptor_;
};





 void error(char *msg)

{
  perror(msg);
  exit(1);
}

/*
 * 
 */
int main(int argc, char** argv) 
{

    for (;;)
    {
    try
    {
        boost::asio::io_service io_service;
        tcp_server server(io_service);
        io_service.run();
    }
    catch (std::exception& e)
    {
        cerr << e.what() << endl;
    }
    }

    return 0;
}

