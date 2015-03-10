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

//using namespace std;

void printStat(vector<books_record> books);

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

/*
 * 
 */
int main(int argc, char** argv) 
{
    engine engn;
//    engn.makeBookBody();
    engn.getBooksIRBIS64();
//    engn.getBookDataFromSite("www.ozon.ru");

    printStat(engn.books);
    //map<string, vector<string>>::iterator it = engn.books.begin();
    vector <books_record>::iterator it = engn.books.begin();
//    for (int i = 0; i < 7; i++)
//        it++;
    size_t counter = 1;
    fOper flinks("links.txt", WRITE);
    flinks.fClose();
    while (it != engn.books.end())
    {
        cout << counter++ << ".\t" << (it->bookname != "" ? it->bookname : " ") << " : " << (it->fio != "" ? it->fio : " ") << endl;
        
        
        flinks.reopen("links.txt", APPEND);
        engn.book.annotation.clear();
        engn.book.coverPath.clear();
        engn.book.qrcode.clear();
        cout << it->bookrecord << endl;
        if (it->isbn != "")
//            if (it->isbn == "5-7020-0807-3")
        {
//            cout << "************************************** " << it->isbn << endl;
//            cout << endl;
        
            engn.prepareServerMap(it->isbn);
            engn.allFound = false;      
//            cout << "make conn" << endl;
            engn.getBookDataFromSite("www.ozon.ru");
            if (engn.neededReloadPagebyNewPath())
            {
//                cout << "here reload page by new path" << endl;
                engn.allFound = false;      
                engn.getBookDataFromSite("www.ozon.ru");
                engn.getBookFromSite(it->isbn);            
            }            
//            cout << "get books from site" << endl;
            engn.getBookFromSite(it->isbn);
            
            if (engn.needSearchByBookName() && !engn.neededReloadPagebyNewPath())
            {
//                cout << "will search by name" << endl;
                engn.getBookByISBN_IRBIS64();
                engn.allFound = false;      
                engn.getBookDataFromSite("www.ozon.ru");
                engn.getBookFromSite(it->isbn);
 //               cout << "getted name and FIO" << endl;
                if (engn.neededReloadPagebyNewPath())
                {
//                    cout << "ищем по новому пути" << endl;
                    engn.allFound = false;      
                    engn.getBookDataFromSite("www.ozon.ru");
                    engn.getBookFromSite(it->isbn);
//                    cout << "finished" << endl;
                }
            }
            else
            if (engn.neededReloadPagebyNewPath())
            {
 //               cout << "here reload page by new path" << endl;
                engn.allFound = false;      
                engn.getBookDataFromSite("www.ozon.ru");
                engn.getBookFromSite(it->isbn);            
            }
            // конец сбора информации, пора заносить в базу и на сайт
            
            
            
//            break;
        }
        else
        {
//            cout << "************************************** " << it->bookname << "+" << it->fio << endl;
//            cout << endl;            
//            cout << "will search by name" << endl;
            engn.allFound = false;      
            engn.prepareServerMap(it->bookname + "+" + it->fio);
            engn.getBookDataFromSite("www.ozon.ru");
            if (engn.neededReloadPagebyNewPath())
            {
//                cout << "here reload page by new path" << endl;
                engn.allFound = false;      
                engn.getBookDataFromSite("www.ozon.ru");
                engn.getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");            
            }            
            else 
                engn.getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");
//            cout << "getted name and FIO" << endl;
            if (engn.neededReloadPagebyNewPath())
            {
//                cout << "ищем по новому пути" << endl;
                engn.allFound = false;      
                engn.getBookDataFromSite("www.ozon.ru");
                engn.getBookFromSite(boost::lexical_cast<string>(counter) + ".txt");
//                cout << "finished" << endl;
            }
//            counter++;
            // конец сбора информации, пора заносить в базу и на сайт
            
            
        }
        
//        engn.book.qrcode   = engn.generateQRcodes(engn.book.pagename);
        if (engn.book.coverPath != "")
        {
            cout << "genering full page" << endl;
            flinks.fWrite("http://murmanlib.ru/" + engn.book.pagename);
            engn.book.pagename = engn.generatePagesNames(1);
            engn.generateQRcodes(engn.book.pagename);        
            engn.updateBookRecord(it->bookrecord  + "11#http://murmanlib.ru/" + engn.book.pagename + "\x1F" + "10#\\\\cgb-centr\\qrcodes\\"+ engn.book.pagename + ".png\x1F\x0D\x0A");
            engn.makeConn("murmanlib.ru");
        }
        else
        {
            cout << "genering template page" << endl;
            engn.book.pagename = engn.generatePagesNames(1);
            engn.generateQRcodes(engn.book.pagename);        
            engn.updateBookRecord(it->bookrecord  + "11#http://murmanlib.ru/" + engn.book.pagename + "\x1F" + "10#\\\\cgb-centr\\qrcodes\\"+ engn.book.pagename + ".png\x1F\x0D\x0A");
            engn.book.coverPath = "www.vsedlyainformatikov.net/_ph/3/433388687.jpg";
            engn.book.annotation = "<pre>        <b>" + it->fio + "</b><br><i>                \"" + it->bookname + "</i>\"</pre>";
            engn.makeConn("murmanlib.ru");            
        }
                
        cout << "Аннотация: " << engn.book.annotation << endl;
        cout << "Обложка:  "  << engn.book.coverPath << endl;
            


        
        it++;
        flinks.fClose();
    }    
    
    
    return 0;
}

