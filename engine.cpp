/* 
 * File:   engine.cpp
 * Author: demiin
 * 
 * Created on March 19, 2014, 5:37 PM
 */

#include <boost/foreach.hpp>

#include "engine.h"
#include "parsing.h"

engine::engine() 
{
    reqCounter = 1;
    responseTypes["moved"] = "Object moved to";
    responseTypes["location"] = "Location:";
}

engine::engine(map<string,string> newAddressesMap)
{
    bookStorage = newAddressesMap;
    responseTypes["moved"] = "Object moved to";
    responseTypes["location"] = "Location:";
}

engine::~engine() 
{
    
}

void engine::makeConn(string server) 
{
    tcp::resolver resolver(io_service) ;
    tcp::resolver::query query(server, "http");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    tcp::socket socket(io_service);
    fOper fOUT(server, WRITE);
    
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    if (error)
        throw boost::system::system_error(error);
    
//    cout << "connect implemented" << endl;
    
    makeRequest(&socket, server);
    
    fOUT.fWrite(getResponse(&socket, server));
    fOUT.fClose();
    
    error = boost::asio::error::host_not_found;
    endpoint_iterator = resolver.resolve(query);
    
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    fOUT.reopen(server, WRITE);
    
    makeRequest(&socket, server);
    fOUT.fWrite(getResponse(&socket, server));
    fOUT.fClose();
    
}

void engine::makeRequest(tcp::socket* socket, string server) 
{
	boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "GET /" << bookStorage[server] << " HTTP/1.0\r\n"
                   << "Host: " << server << "\r\n"
                   << "Accept: */*\r\n" 
                   << "Connection: close\r\n\r\n";
        boost::asio::streambuf::const_buffers_type bufs = request.data();
        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
}

string engine::getResponse(tcp::socket* socket, string server) 
{
    boost::asio::streambuf response;
    boost::asio::read_until(*socket, response, "\r\n");
    if (validateResponse(socket, &response))
    {
        istream response_stream(&response);
        boost::system::error_code error = boost::asio::error::host_not_found;
        boost::asio::read_until(*socket, response, "\r\n\r\n");
    
        string header;
        while (getline(response_stream, header) && header != "\r")
        {
//            cout << header << endl;
            
            if (header.find(responseTypes["location"]) != string::npos)
            {
                parsing parser(header);
                bookStorage[server] =  parser.parseLine(LOCATION);
            }
        }
//        cout << endl;
        
 
        while (boost::asio::read(*socket, response, boost::asio::transfer_at_least(1), error))
        {
            //cout << &response;
        }
        
        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        
        boost::asio::streambuf::const_buffers_type bufs = response.data();
        return string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
    }
    
    
    return "";
}

bool engine::validateResponse(tcp::socket* socket, boost::asio::streambuf* response) 
{
    istream response_stream(response);
    string http_version;
    response_stream >> http_version;
    unsigned int status_code;
    response_stream >> status_code;
    string status_message;
    getline(response_stream, status_message);
    
    cout << "ver: " << http_version << "\ncode: " << status_code << "\nmessage: " << status_message << endl;

    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
        cout << "Invalid response" << endl;
        return false;
    }
    
    return true;
}

void engine::getBooksIRBIS64() 
{
    login_unlogin("C", "A");
    getMaxMFN();
    getBookList();
    login_unlogin("C", "B");
    
//    cout << books.size() << endl;
}

void engine::getBookList() 
{
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string codeCommand = "K";
    string codeAPM = "C";
    
    string prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
              + "CMPL\n\n"
              + boost::lexical_cast<string>(irbis64_countAnswers) + "\n"
              "1\n"
              "mpl,v88^a'&&&&&'v907\n"
              "1\n" +
              maxMFN_CMPL + '\n' +
              "!(if v907^c : 'КРФ' then '1' else '0' fi)";

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
    
    //parsing parse(sendRequest(&request));
    string sss = sendRequest(&request);
//    cout << sss << endl;
    parsing parse(sss);
    ksu = parse.getDeliveredKSU();
    
    string maxKSU = maxKSUDate(ksu);

    map<string,string>::iterator ksuIter = ksu.begin();
    
    /* удаление ненужных КСУ */
    while (ksuIter != ksu.end())
    {
//        cout << "KSU: " << ksuIter->first << " % " << ksuIter->second << endl;
        if (ksuIter->second.substr(0, 6) != maxKSU)
            ksu.erase(ksuIter);

        ksuIter++;
    }
    
    request_stream.clear();

    ksuIter = ksu.begin();
    prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
            + "OLDEK\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n1\n"
            "mpl,v10,'&&&&&',(v910^u|---|v910^d|###|),'$'\n"
            "1\n" + maxMFN_BOOKBASE + '\n' +
            "!(if v910^u : '" + ksuIter->first + "' "; 

            if (ksu.size() > 1)
            {
                while (ksuIter != ksu.end())
                {
                    prepareRequest += "or v910^u : '" + ksuIter->first + "' ";
                    ksuIter++;
                }
            }
            
            prepareRequest += "then '1' else '0' fi)";
             
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
//    cout << prepareRequest << endl;
    parse = sendRequest(&request);
    vector <string> splitedResponse = parse.split();
//    cout << "line count: " << splitedResponse.size() << endl;
    ksuIter = ksu.begin();
    BOOST_FOREACH(string splitedLine, splitedResponse)
    {
        parse = splitedLine;
//        cout << splitedLine << endl;
        books.insert(parse.parseBookRecord(ksuIter->first));
        //cout << "size books: " << books.size() << endl;
    }
//    map<string, vector<string>>::iterator it = books.begin();
//    while (it != books.end())
//    {
//        cout << it->first << endl;
//        it++;
//    }
}

void engine::getBookByISBN_IRBIS64() 
{
    login_unlogin("C", "A");
    
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string codeCommand = "K";
    string codeAPM = "C";
    
    string prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
            + "OLDEK\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n1\n"
            "mpl,v10,'&&&&&',if p(v200^a) then v200^a else v461^c fi,'+'v700^a'+'v700^g,'$'\n"
            "1\n" + maxMFN_BOOKBASE + '\n' +
            "!(if v10^a = '" + currentISBN + "' then '1' else '0' fi)";
             
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
    parsing parse(sendRequest(&request));
    login_unlogin("C", "B");
    bookStorage["www.ozon.ru"] = requestString_BooknameAndFIO_ozon + parse.getBookNameAndFIO();
}


void engine::login_unlogin(string codeAPM, string codeCommand) 
{
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_login + '\n' + irbis64_password;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
    
    sendRequest(&request);
}


string engine::sendRequest(boost::asio::streambuf* request) 
{
    tcp::resolver resolver(io_service) ;
    tcp::resolver::query query(server64, "6666");
    tcp::resolver::iterator endpoint_iterator = resolver.resolve(query);
    tcp::resolver::iterator end;
    tcp::socket socket(io_service);
        
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    if (error)
        throw boost::system::system_error(error);
    
    cout << "sended: " << boost::asio::write(socket, *request) << " bytes" << endl;
    /*******************************************/
    boost::asio::streambuf response;
    
    istream response_stream(&response);
    error = boost::asio::error::host_not_found;
    
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
    {
       // cout << &response;
    }
        
    if (error != boost::asio::error::eof)
        throw boost::system::system_error(error);
        
    boost::asio::streambuf::const_buffers_type bufs = response.data();
    return string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
}

void engine::getMaxMFN() 
{
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string prepareRequest = "O\nC\nO\n"
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_CMPL_BASE;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');

    request_stream << prepareRequest;
    parsing parse(sendRequest(&request));
    //cout << "CMPL: " << parse.maxMFN() << endl;
    //cout << maxMFN_CMPL << endl;
    maxMFN_CMPL = parse.maxMFN();
    
    request_stream.clear();
    prepareRequest = "O\nC\nO\n"
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_BOOK_BASE;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');

    request_stream << prepareRequest;
    parse = sendRequest(&request);
    //cout << "OLDEK: " << parse.maxMFN() << endl;
    maxMFN_BOOKBASE = parse.maxMFN();
}

string engine::curdate() 
{
    boost::posix_time::ptime now = boost::posix_time::second_clock::local_time();
    std::stringstream ss;
    ss << static_cast<int>(now.date().year()) << (now.date().month().as_number() < 10 ? "0" + boost::lexical_cast<string>(now.date().month().as_number()) : boost::lexical_cast<string>(now.date().month().as_number()));

    return ss.str();
}

string engine::maxKSUDate(map<string, string> ksu) 
{
    map<string, string>::iterator it = ksu.begin();
    string tmp = "0";
    while (it != ksu.end())
    {
        if (it->second > tmp)
            tmp = it->second;
        it++;
    }
    
    return tmp.substr(0, 6);
}

void engine::prepareServerMap(string rqst) 
{
//    cout << "isbn: " << rqst << endl;
    bookStorage["www.ozon.ru"] = requestString_ozon + rqst;
//    cout << bookStorage["www.ozon.ru"] << endl;
    currentISBN = rqst;
}

void engine::getBookFromSite(string filename) 
{
    clearNewPath();
    fOper fWrite(filename, WRITE);
    cout << "getting books from site" << endl;
    
    parsing parse("");
    fOper f(string("www.ozon.ru"), OPEN);
    size_t counter = 0;
    while (!f.eof())
    {
        string anotherLine = f.fRead();
//        cout << anotherLine << endl;
        parse = anotherLine;
        //cout << ++counter << f.fRead() << endl;
        string coverAddr = parse.getAddrFullCover();
        if (coverAddr != "")
        {
            cout << "Адрес обложки: " << coverAddr << endl;
            fWrite.fWrite(coverAddr);
            fWrite.fWrite("\n");
            allFound = true;
        }
        string annotation = parse.getAnnotation();
        if (annotation != "")
        {
            cout << "Аннотация: " << annotation << endl;
            fWrite.fWrite(annotation);
            allFound = true;
        }
        newPath = parse.linkToNewAddrBook_fromMany();
        if (newPath != "")
        {
            cout << "newPath: " << newPath << endl;
            bookStorage["www.ozon.ru"] = newPath;
            break;
        }
        if (allFound)
            needSearchByName = false;
        else
            needSearchByName = parse.nothingFindByISBN();
    }
    fWrite.fClose();
}

bool engine::needSearchByBookName() 
{
    return needSearchByName;
}
