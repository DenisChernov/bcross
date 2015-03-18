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

#include <boost/bind.hpp> 
#include <boost/shared_ptr.hpp> 
#include <boost/enable_shared_from_this.hpp> 
#include <boost/asio.hpp>

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
//        cout << it->bookrecord << endl;
        if (it->isbn != "")
//            if (it->isbn == "5-7020-0807-3")
        {
//            cout << "************************************** " << it->isbn << endl;
//            cout << endl;
        
            engn->prepareServerMap(it->isbn);
            engn->allFound = false;      
//            cout << "make conn" << endl;
            engn->getBookDataFromSite("www.ozon.ru");
            if (engn->neededReloadPagebyNewPath())
            {
//                cout << "here reload page by new path" << endl;
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(it->isbn);            
            }            
//            cout << "get books from site" << endl;
            engn->getBookFromSite(it->isbn);
            
            if (engn->needSearchByBookName() && !engn->neededReloadPagebyNewPath())
            {
//                cout << "will search by name" << endl;
                engn->getBookByISBN_IRBIS64();
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(it->isbn);
 //               cout << "getted name and FIO" << endl;
                if (engn->neededReloadPagebyNewPath())
                {
//                    cout << "ищем по новому пути" << endl;
                    engn->allFound = false;      
                    engn->getBookDataFromSite("www.ozon.ru");
                    engn->getBookFromSite(it->isbn);
//                    cout << "finished" << endl;
                }
            }
            else
            if (engn->neededReloadPagebyNewPath())
            {
 //               cout << "here reload page by new path" << endl;
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(it->isbn);            
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
            engn->prepareServerMap(it->bookname + "+" + it->fio);
            engn->getBookDataFromSite("www.ozon.ru");
            if (engn->neededReloadPagebyNewPath())
            {
//                cout << "here reload page by new path" << endl;
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");            
            }            
            else 
                engn->getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");
//            cout << "getted name and FIO" << endl;
            if (engn->neededReloadPagebyNewPath())
            {
//                cout << "ищем по новому пути" << endl;
                engn->allFound = false;      
                engn->getBookDataFromSite("www.ozon.ru");
                engn->getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");
//                cout << "finished" << endl;
            }
//            counter++;
            // конец сбора информации, пора заносить в базу и на сайт
            
            
        }

//        engn->book.qrcode   = engn->generateQRcodes(engn->book.pagename);
        if (engn->book.coverPath != "")
        {
            cout << "genering full page" << endl;
            engn->book.pagename = engn->generatePagesNames(1);
            engn->generateQRcodes(engn->book.pagename);        
            

                        
                        
                        /*
                         
                         Получить запись искомой книги, путем поиска ее данных
                         
                         */
                        engn->getCurrentBookRecord();
                        //cout << "book record: " << it->bookrecord << endl;
                        parsing pars(it->bookrecord);
                        engn->updateBookRecord(pars.remakeBooklist()  + "\x1F" + "11#http://murmanlib.ru/" + engn->book.pagename + "\x1F" + "10#\\\\cgb-centr\\qrcodes\\"+ engn->book.pagename + ".png\x1F\x0D\x0A");
                        engn->makeConn("murmanlib.ru");

        }
        else
        {
            cout << "genering template page" << endl;
            engn->book.pagename = engn->generatePagesNames(1);
            engn->generateQRcodes(engn->book.pagename);        
            engn->book.coverPath = "www.vsedlyainformatikov.net/_ph/3/433388687.jpg";
            engn->book.annotation = "<pre>        <b>" + it->fio + "</b><br><i>                \"" + it->bookname + "</i>\"</pre>";

                engn->getCurrentBookRecord();
                //cout << "book record: " << it->bookrecord << endl;
                parsing pars(it->bookrecord);
                engn->updateBookRecord(pars.remakeBooklist()  + "\x1F" + "11#http://murmanlib.ru/" + engn->book.pagename + "\x1F" + "10#\\\\cgb-centr\\qrcodes\\"+ engn->book.pagename + ".png\x1F\x0D\x0A");

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
    
    void start()
    {
        cout << "async write" << endl;
        
        boost::asio::async_write(socket_, boost::asio::buffer("insert answere here"),
                                                boost::bind(&tcp_connection::handle_write, shared_from_this(),
                                                boost::asio::placeholders::error,
                                                boost::asio::placeholders::bytes_transferred));
    }
    
private:
    tcp_connection(boost::asio::io_service& io_service) : socket_ (io_service)
    {
    }
    
    void handle_write(const boost::system::error_code&, size_t)
    {

        
    }
    
    
    tcp::socket socket_;
};

string getRecord(tcp_connection::pointer* new_connection)
{
        boost::system::error_code error;
        boost::array< char, 10096 > buf;
        size_t length = boost::asio::read((*new_connection)->socket(), boost::asio::buffer(buf, 10096), boost::asio::transfer_all(), error);
        engine* engn;
        engn = new engine();
        books_record book;
  
        parsing pars(boost::lexical_cast<string>(buf.c_array()));
        vector<string> tmp = pars.isbn_bookname_fio();
        book.isbn = tmp.at(0);
        book.bookname = tmp.at(1);
        book.fio = tmp.at(2);
        
        engn->books.push_back(book);
        parse_all_exist_base(engn);
        free(engn);
        cout << "done" << endl;
        return "";
}

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
        cout << "*************************handle" << endl;
        if (!error)
        {
            new_connection->start();
            getRecord(&new_connection);
        }
        //start_accept();
    }
   
    tcp::acceptor acceptor_;
};







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

