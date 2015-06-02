/* 
 * File:   engine.cpp
 * Author: demiin
 * 
 * Created on March 19, 2014, 5:37 PM
 */

#include "engine.h"


engine::engine() 
{
    reqCounter = 1;
    responseTypes["moved"] = "Object moved to";
    responseTypes["location"] = "Location:";
//    getMaxMFN();
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
//    fOper fOUT(server + "_0", WRITE);
    
    boost::system::error_code error = boost::asio::error::host_not_found;
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    if (error)
        throw boost::system::system_error(error);
    
//    cout << "connect implemented" << endl;
    
    makeRequest_get(&socket, server);
    string form_id = getResponse(&socket, server);
    
    
//    fOUT.fWrite(form_id);
//    fOUT.fClose();
    
    parsing* pars = new parsing(form_id);
    form_id = pars->getFormID();
    login_site += "&form_build_id=";
    
    /*
     * 
     *  НИВКОЕМ СЛУЧАЕ НЕ ДОБАВЛЯТЬ ЗДЕСЬ  FORM_BUILD_ID
     *  иначе не будет выдаваться кукис
     * 
     */
    
//    cout << login_site << endl;
//    cout << "***********************  cookie: " << cookie << endl;
    error = boost::asio::error::host_not_found;
    endpoint_iterator = resolver.resolve(query);

    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
//    cout << " first file done" << endl;
    
    
    
//    fOUT.reopen(server + "_1", WRITE);

    if (error)
        throw boost::system::system_error(error);
    
    makeRequest_post(&socket, server);
    getResponse(&socket, server);
    
//    fOUT.fWrite(getResponse(&socket, server));
    
    error = boost::asio::error::host_not_found;
    endpoint_iterator = resolver.resolve(query);
    
//    cout << " second file done" << endl;
    
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    fOper f(book.pagename, WRITE);

    if (error)
        throw boost::system::system_error(error);    
    
    makeRequest_getAddPage(&socket, server);
//    getResponse(&socket, server);
    f.fWrite(getResponse(&socket, server));
    f.fClose();
//    cout << "***********************  cookie: " << cookie << endl;    

//    cout << " third file done" << endl;
  
    getForm_Token_ID();

    error = boost::asio::error::host_not_found;
    endpoint_iterator = resolver.resolve(query);
    
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
//    cout << " forth file done" << endl;
    
//    fOUT.reopen(server + "_2", WRITE);
//    fOper fls("thisis.html", WRITE)   ;
        
    if (error)
        throw boost::system::system_error(error);
    
//    makePageBody("http://static2.ozone.ru/multimedia/books_covers/c300/1000890179.jpg",
//                 "Несправедливо осужденный трибуналом Воин Ветра, офицер воздушного флота Российской империи Егор Сморода поставлен перед выбором: сгнить на каторге или присоединиться к членам загадочного Института Прикладной Экзофизики, которые при помощи невероятного оружия очищают город от вампиров, демонов, оборотней и другой агрессивной нежити. Однако после того, как Сморода вступает в подпольную организацию &laquo;охотников за привидениями&raquo;, выясняется, что ставки в этой игре гораздо более высокие, чем ему казалось вначале. Впрочем, беглому каторжнику уже нечего терять.");
    makePageBody(book.coverPath, book.annotation, book.pagename);

    addPage = "-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"title\"\r\n\r\nВы попали на эту страницу, участвуя в акции \"Покажи Книге Мир\"\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"taxonomy_vocabulary_12[und]\"\r\n\r\n_none\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"taxonomy_vocabulary_13[und]]\"\r\n\r\n_none\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[link_title]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"changed\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"body[und][0][summary]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][data-inner-width]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][data-inner-height]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][title]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][data-inner-width]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][data-inner-height]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][id]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][class]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][item_attributes][style]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[parent]\"\r\n\r\nprimary-links:0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][id]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][name]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][target]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][rel]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][class]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][style]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[options][attributes][accesskey]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"body[und][0][value]\"\r\n\r\n" + formBody + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"body[und][0][format]\"\r\n\r\n3\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"changed\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"form_build_id\"\r\n\r\n" + form_id + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"form_token\"\r\n\r\n" + form_token + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"form_id\"\r\n\r\npage_node_form\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"log\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[field_image200150_und_0]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image200150[und][0][_weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image200150[und][0][fid]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image200150[und][0][display]\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[field_image200150_und_0]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image150200[und][0][_weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image150200[und][0][fid]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_image150200[und][0][display]\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[upload_und_0]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"upload[und][0][_weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"upload[und][0][fid]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"upload[und][0][display]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][coords]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][type]\"\r\n\r\nyandex#map\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][placemarks]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][lines]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][polygons]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"field_yamaps[und][0][routes]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[link_title]\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[parent]\"\r\n\r\nmain-menu:0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"menu[weight]\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"additional_settings__active_tab\"\r\n\r\nedit-menu\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"path[alias]\"\r\n\r\n" + book.pagename + "\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"comment\"\r\n\r\n0\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"files[upload]\"; filename=\"\"\r\nContent-Type: application/octet-stream\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"xmlsitemap[status]\"\r\n\r\ndefault\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"xmlsitemap[priority]\"\r\n\r\ndefault\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"name\"\r\n\r\nDemiin\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"date\"\r\n\r\n\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"status\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"path[pathauto]\"\r\n\r\n1\r\n-----------------------------15914381318027603441113782851\r\nContent-Disposition: form-data; "
              "name=\"op\"\r\n\r\n..................\r\n-----------------------------15914381318027603441113782851--\r\n";
    
    makeRequest_addPage(&socket, server);

    cout << getResponse(&socket, server) << endl;
    
    socket.close();
//    cout << "***********************  cookie: " << cookie << endl;    
    cout << "ddone" << endl;
    cout << "created: \n\t" << this->book.autor << "\n\t" << this->book.bookname << "\n\t" << "at page: http://murmanlib.ru/" << book.bookname << endl;

}

void engine::makeRequest_get(tcp::socket* socket, string server) 
{
	boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "GET / HTTP/1.1\r\n"
                   << "Host: " << server << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n";
//                   << "Accept-Encoding: gzip, deflate\r\n";
    if (cookie != "")
        request_stream << "Cookie: " << cookie << cookie_magic << "\r\n";
    request_stream << "Connection: keep-alive\r\n\r\n";
        boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
    cookie = "";
}

void engine::makeRequest_post(tcp::socket* socket, string server) 
{
	boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "POST /node?destination=node HTTP/1.1\r\n"
                   << "Host: " << server << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                   << "Accept-Encoding: gzip, deflate\r\n"
                   << "Referer: http://murmanlib.ru/\r\n"
                   << "Cookie: " << cookie << cookie_magic << "\r\n"
                   << "Connection: keep-alive\r\n"
                   << "Content-Type: application/x-www-form-urlencoded\r\n"
                   << "Content-Length: " << login_site.length() << "\r\n\r\n"
                   << login_site;
    
        boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
    cookie = "";
}

void engine::makeRequest_getAddPage(tcp::socket* socket, string server) 
{
	boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "GET /node/add/page HTTP/1.1\r\n"
                   << "Host: " << server << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                   << "Accept-Encoding: gzip, deflate\r\n"
                   << "Referer: http://murmanlib.ru/node/add\r\n"
                   << "Cookie: " << cookie << "; " << cookie_magic << "\r\n"
                   << "Connection: keep-alive\r\n\r\n";
    
        boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);

}

void engine::makeRequest_addPage(tcp::socket* socket, string server) 
{
	boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "POST /node/add/page HTTP/1.1\r\n"
                   << "Host: " << server << "\r\n"
                   << "Accept: text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8\r\n"
                   << "Accept-Language: ru-RU,ru;q=0.8,en-US;q=0.5,en;q=0.3\r\n"
//                   << "Accept-Encoding: gzip, deflate\r\n"
                   << "Referer: http://murmanlib.ru/node/add/page\r\n"
                   << "Cookie: " << cookie /*<< "; " << cookie_magic*/ << "\r\n"
                   << "Connection: keep-alive\r\n"
                   << "Content-Type: multipart/form-data; boundary=---------------------------15914381318027603441113782851\r\n"
                   << "Content-Length: " << addPage.length() << "\r\n\r\n"
                   << addPage;
    
        boost::asio::streambuf::const_buffers_type bufs = request.data();
//        cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
    cookie = "";

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
            
            if (header.find("SESS") != string::npos)
            {
                parsing parser(header);
//                bookStorage[server] =  parser.parseLine(LOCATION);
//                cout << "Our cockie: " << parser.getCookie() << endl;
                cookie = parser.getCookie();
            }
            if (header.find("Location") != string::npos)
            {
                parsing parser(header);
//                bookStorage[server] =  parser.parseLine(LOCATION);
//                cout << "Our cockie: " << parser.getCookie() << endl;
                newPath = parser.getNewLocation();
         //       cout << "newPath: " << newPath << endl;
                bookStorage["www.ozon.ru"] = newPath;
            }
        }
   //     cout << endl;
        
 
        while (boost::asio::read(*socket, response, boost::asio::transfer_at_least(1), error))
        {
            //cout << &response;
        }
        
        if (error != boost::asio::error::eof)
            throw boost::system::system_error(error);
        
        boost::asio::streambuf::const_buffers_type bufs = response.data();
        return string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
    }
    cout << "token from header: " << form_token << endl;
    
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
    
  //  cout << "ver: " << http_version << "\ncode: " << status_code << "\nmessage: " << status_message << endl;

    if (!response_stream || http_version.substr(0, 5) != "HTTP/")
    {
//        cout << "Invalid response" << endl;
        return false;
    }
    
    return true;
}

void engine::getBooksIRBIS64() 
{
    login_unlogin("C", "A");
    getMaxMFN();
    getBookList();
    //remakeBooklist();
    
//    generatePagesNames(boost::lexical_cast<int>(maxMFN_BCROSS));
//    generateQRcodes(boost::lexical_cast<int>(maxMFN_BCROSS));

    login_unlogin("C", "B");
    
//    cout << books.size() << endl;
}

void engine::getBookList() 
{
    fOper fOUT("booklist.txt", WRITE);
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string codeCommand = "K";
    string codeAPM = "C";
//    cout << "max: " << maxMFN_BCROSS << endl;
    string prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
            + "BCROSS\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n1\n"
            "mpl,'&&&&&',&uf('+0')\n"
            "1\n" + maxMFN_BCROSS + "\n'1'";
             
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
//    cout << prepareRequest << endl;
    //parsing parse(sendRequest(&request));
    fOUT.fWrite(sendRequest(&request));
    fOUT.fClose();
    
    request_stream.clear();
   
    prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
            + "BCROSS\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n1\n"
            "mpl,'&&&&&','1@'v1,'---2@',v2'---3@'v3'&&&&&'\n"
            "1\n" + maxMFN_BCROSS + "\n'1'";
             
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
//    cout << prepareRequest << endl;
    //parsing parse(sendRequest(&request));
    string booksListing = sendRequest(&request);
//    cout << booksListing<< endl;
    parsing parser(booksListing);
    books = parser.parseBookRecord();
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
            + "BCROSS\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n1\n"
            "mpl,v3,'&&&&&',v1,'+'v2,'$'\n"
            "1\n" + maxMFN_BCROSS + '\n' +
            //"|(v3 = '" + currentISBN + "')";
            "!(if v3 = '" + currentISBN + "' then '1' else '0' fi)";
             
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
//    cout << prepareRequest << endl;
    
    request_stream << prepareRequest;
    string sss = sendRequest(&request);
//    cout << sss << endl;
    parsing parse(sss);
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
    
//    cout << "sended: " << boost::asio::write(socket, *request) << " bytes" << endl;
    boost::asio::write(socket, *request);
    /*******************************************/
    boost::asio::streambuf response;
    
    istream response_stream(&response);
    error = boost::asio::error::host_not_found;
    
    while (boost::asio::read(socket, response, boost::asio::transfer_at_least(1), error))
    {
        //cout << &response;
    }
        
    if (error != boost::asio::error::eof)
        throw boost::system::system_error(error);
        
    boost::asio::streambuf::const_buffers_type bufs = response.data();
    
    socket.close();
    io_service.stop();

    return string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + response.size());
}

void engine::sendRequest_noAnswer(boost::asio::streambuf* request) 
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
    
//    cout << "sended: " << boost::asio::write(socket, *request) << " bytes" << endl;
    boost::asio::write(socket, *request);
    /*******************************************/
    socket.close();
    io_service.stop();
}

void engine::getMaxMFN() 
{
    login_unlogin("C", "A");
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string prepareRequest = "O\nC\nO\n"
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_BCROSS_BASE;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    request_stream << prepareRequest;
    parsing parse(sendRequest(&request));
//    cout << "BCROSS: " << parse.maxMFN() << endl;
    //cout << maxMFN_CMPL << endl;
    maxMFN_BCROSS = parse.maxMFN();
//    cout << maxMFN_BCROSS << endl;
    login_unlogin("C", "B");
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

void engine::prepareServerMap_BookFio(string rqst) {
    parsing pars(rqst);
    bookStorage["www.ozon.ru"] = requestString_BooknameAndFIO_ozon + pars.replace_all_space();
    currentISBN = pars.replace_all_space();
}


void engine::getBookFromSite(string filename) 
{
    
    clearNewPath();
    fOper fWrite("isbn/" + filename, WRITE);
//    cout << "getting books from site" << endl;
    
    parsing parse("");
    fOper f(string("www.ozon.ru"), OPEN);
    size_t counter = 0;
    while (!f.eof())
    {
        string anotherLine = f.fRead();
        parse = anotherLine;
        string  autor = /*iconv_recode("cp1251", "utf-8",*/ parse.getAutor();
        if (autor != "")
        {
            cout << "author" << autor << endl;
            book.autor = autor;
        }
        
        string bookname = /*iconv_recode("cp1251", "utf-8", */parse.getBookname();
        if (bookname != "")
        {
            book.bookname = parse.removeBeginEndWhiteSpaces(bookname);
            cout << "bookname: " << book.bookname << endl;
        }
        
        string coverAddr = parse.getAddrFullCover();
        if (coverAddr != "")
        {
//            cout << "Адрес обложки: " << coverAddr << endl;
            fWrite.fWrite(coverAddr);
            fWrite.fWrite("\n");
            allFound = true;
            book.coverPath = coverAddr;
        }
        string annotation = /*iconv_recode("cp1251", "utf-8", */parse.getAnnotation();
        if (annotation != "")
        {

//            cout << "Аннотация: " << annotation << endl;
            fWrite.fWrite(annotation);
            allFound = true;
            book.annotation = annotation;
        }
        if (notNeedNewPath == false){
            newPath = parse.linkToNewAddrBook_fromMany();
            if (newPath != "")
            {
                cout << "newPath: " << newPath << endl;
                bookStorage["www.ozon.ru"] = newPath;
                
                ++counter;
                if (counter == 2){
                    break;
                }
            }
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

void engine::getForm_Token_ID() 
{
    fOper fIn(book.pagename, OPEN);
    parsing parser("");
    while (!fIn.eof())
    {
        parser = fIn.fRead();
        string tmp = parser.getFormID();
        if (tmp != "")
            form_id = tmp;
        
        
        if (form_token == "")
        {
            tmp = parser.getFormToken();
            if (tmp != "")
                form_token = tmp;
        }
    }
    
    cout << "token from 1: " << form_token << endl;
//    if (form_id != "")
//        cout << form_id << endl;
//    if (form_token != "")
//        cout << form_token << endl;
    
    
    fIn.fClose();
}

void engine::remakeBooklist() 
{
    fOper in("booklist.txt", OPEN);
    if (in.is_open_read())
    {
        for (int i = 0; i < 12; i++)
        {
            in.fRead();
        }                
        
        int counter = 0;
        parsing parser("");
        while (!in.eof())
        {
             parser = in.fRead();
             string record = parser.remakeBooklist();
             if (record != "")
                 books.at(counter).bookrecord = record;
//             cout << counter << "->>>>>> " << books.at(counter).bookrecord << endl;
             //booklist.push_back(parser.remakeBooklist() + "11#http://murmanlib.ru/" + generatedPageNames.back()+ "\x1F" + "10#\\\\cgb-centr\\qrcodes\\"+ qrcodes.back() + ".png\x1F\x0D\x0A");
//            updateBookRecord(parser.remakeBooklist() + "11#http://murmanlib.ru/" + generatedPageNames.back()+ "\x1F" + "10#\\\\cgb-centr\\qrcodes\\"+ qrcodes.back() + ".png\x1F\x0D\x0A");
//            generatedPageNames.pop_back();
//            qrcodes.pop_back();
             counter++;
        }
//        booklist.pop_back();
//        cout << booklist.back() << endl;
//        updateBookRecord(booklist.back());
    }
    in.fClose();
}

void engine::updateBookRecord(string record) 
{
    login_unlogin("C", "A");
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    
    string prepareRequest = "D\nC\nD\n"
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
                            + irbis64_BCROSS_BASE + "\n0\n1\n" + record;

    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
    
    request_stream << prepareRequest;
    cout << prepareRequest << endl;
//    parsing parse(sendRequest(&request));
    cout << sendRequest(&request) << endl;
    
    login_unlogin("C", "B");
}

void engine::getCurrentBookRecord() 
{
    cout << "template get current" << endl;
    login_unlogin("C", "A");
    boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
   
    string codeCommand = "K";
    string codeAPM = "C";
    
    string prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
            + "BCROSS\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n0\n"
            "mpl,&uf('+0')\n0\n0\n";
    if (books.front().isbn != "")
        prepareRequest += "! if v3='" + books.front().isbn +"' then '1' else '0' fi";
    else
        prepareRequest += "! if v1 = '" + books.front().bookname + "' and v2 = '" + books.front().fio + "' then '1' else '0' fi";
             
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
//    cout << prepareRequest << endl;
    request_stream << prepareRequest;
    string sss = sendRequest(&request);
//    cout << "*****" << sss << endl;
    parsing pars(sss);
    prepareRequest = codeCommand + '\n' + codeAPM + '\n' + codeCommand + '\n'
                            + MAGIC_CODE + '\n' + boost::lexical_cast<string>(reqCounter++) + '\n'
                            + irbis64_password + '\n' + irbis64_login + "\n\n\n\n"
            + "BCROSS\n\n"
            + boost::lexical_cast<string>(irbis64_countAnswers) + "\n" + pars.countFoundedRecords() + "\n"
            "mpl,&uf('+0')\n0\n0\n";

    if (books.front().isbn != "")
        prepareRequest += "! if v3='" + books.front().isbn +"' then '1' else '0' fi";
    else
        prepareRequest += "! if v1 = '" + books.front().bookname + "' and v2 = '" + books.front().fio + "' then '1' else '0' fi";
    prepareRequest.insert(0, boost::lexical_cast<string>(prepareRequest.length()) + '\n');
//    cout << "*****" << prepareRequest << endl;
    request_stream << prepareRequest;
    sss = sendRequest(&request);
//    cout << "*****" << sss << endl;
    pars = sss;
    books.front().bookrecord = pars.getLinebookRecord();
    login_unlogin("C", "B");
}


vector<string> engine::genPageName(int count) 
{
    vector<string> tmp;
    
    std::mt19937 gen(time(0));
    std::uniform_int_distribution<> uid(10000000, 99999999);
    std::uniform_int_distribution<> simb(65, 90);
    std::uniform_int_distribution<> pos(0, 7);
    for (int page = 0; page < count; page++)
    {
        string s = boost::lexical_cast<string>(uid(gen));
        for (int i = 0; i <= 8; i++)
        {
            string::iterator it = s.begin();
            for (int position = 0; position < pos(gen); position++)
            {
                it++;
            }
            s.insert(it, char(simb(gen)));
        }
        tmp.push_back(s);
    }
    book.qrcode = tmp.front();
    return tmp;
}

string engine::generatePagesNames(int count) 
{
//    vector <string> gpn = genPageName(count);
//    generatedPageNames.assign(gpn.begin(), gpn.end());
    return genPageName(count).front();
/*    
    for (int i = 0; i < count; i++)
    {
        cout << generatedPageNames.back() << endl;
        generatedPageNames.pop_back();
    }
 */
}

void engine::generateQRcodes(int count) 
{
    for (int i = 0; i < count; i++)
    {
        string s = "./createQR.sh http://murmanlib.ru/" + generatedPageNames.at(i) + " qrTest.png /mnt/filial6_.8_D/FileServerFolder/qrcodes/" + generatedPageNames.at(i);
        cout << s << endl;
        system(s.c_str());
        qrcodes.push_back(generatedPageNames.at(i));
    }
}

void engine::generateQRcodes(string pagename) 
{
//    for (int i = 0; i < count; i++)
//    {
        string s = "./createQR.sh http://murmanlib.ru/" + pagename + " qrTest.png /mnt/filial6_.8_D/FileServerFolder/qrcodes/" + pagename;
        cout << s << endl;
        system(s.c_str());
//    }
}


void engine::makePageBody(string img, string annotation, string qrcode) 
{
    formBody = //"<p style=\"font-size: medium; text-align: center;\"><strong>Вы попали на эту страницу, участвуя в акции \"Покажи Книге Мир\"</strong></p>"
               "<?php "
               "echo '<form action=\"modules/bcross_accept.php\" method=\"post\">';"
               "echo '<p style=\"text-align: justify;\">Предлагаем Вам заполнить следующие поля и помочь нам составить дневник путешествия Книги. <br />Bookcrossing  - общественное движение, действующее по принципу социальных сетей и близкое к флешмобу.  Человек, прочитав книгу, оставляет её в общественном месте (парк, кафе, поезд, станция метро), для того, чтобы другой, случайный человек мог эту книгу найти и прочитать; тот в свою очередь должен повторить эти же действия.</p>"
               "<table border=\"0\">"
               "<tbody>"
               "<tr>"
               "<td><img width=\"300\" height=\"300\" src=\"http://" + img + /*http://static2.ozone.ru/multimedia/books_covers/c300/1000890179.jpg*/ "\" border=\"0\" /></td>"
               "<td><p style=\"text-align: justify;\">" + annotation + /*Несправедливо осужденный трибуналом Воин Ветра, офицер воздушного флота Российской империи Егор Сморода поставлен перед выбором: сгнить на каторге или присоединиться к членам загадочного Института Прикладной Экзофизики, которые при помощи невероятного оружия очищают город от вампиров, демонов, оборотней и другой агрессивной нежити. Однако после того, как Сморода вступает в подпольную организацию &laquo;охотников за привидениями&raquo;, выясняется, что ставки в этой игре гораздо более высокие, чем ему казалось вначале. Впрочем, беглому каторжнику уже нечего терять.*/ + "</p></td>"
               "</tr>"
               "</tbody>"
               "</table>"
               "<p><strong>Ваше имя: </strong><br /> <input name=\"usrname\" size=\"50\" type=\"text\" /></p>"
               //"<p><strong>Возраст: </strong><select name=\"age\" size=\"1\"> <option>Возрастная категория</option> <option value=\"before14\">до 14 лет</option> <option value=\"after14\">15-24 года</option> <option value=\"after25\">25-50 лет</option> <option value=\"after50\">После 51 года</option> </select></p>"
               "<p><strong>Возраст: </strong><br /> <input name=\"age\" size=\"50\" type=\"text\" /></p>"
               "<p style=\"text-align: left;\"><strong>Где вы встретили Книгу:</strong><br /> <input name=\"geofind\" size=\"50\" type=\"text\" value=\"';"
               "include(\"modules/sypex_geo/SxGeo.php\");"
               "$SxGeo = new SxGeo('modules/sypex_geo/SxGeoCity.dat', SXGEO_FILE);"
               "$ip = $_SERVER['REMOTE_ADDR'];"
               "$city = $SxGeo->get($ip);"
               "print $city[\"city\"][\"name_ru\"]; "

                "$conn_str = \"host=192.168.9.250 port = 5432 dbname=bcross user=oa password=oa\";"
                "$conn = pg_connect($conn_str)"
                    "or die(\"error connect\". pg_last_error($conn));"

                    "$query = \"INSERT INTO readers VALUES ((SELECT COUNT (*) + 3 from readers), 'automatic', '0', '\". $city[\"city\"][\"name_ru\"]. \"',' ', '" + qrcode + "', '\". date('d.m.Y'). \"');\";"
        
        
                "$res = pg_query($query)"
                    "or die (\"Error query: \". pg_last_error($conn));"
        
               "echo '\" />"
               "<p style=\"text-align: left;\"><strong>Где вы планируете ее оставить или передать другому читателю:</strong><br /> <input name=\"geoleave\" size=\"50\" type=\"text\" />"
               "<br /><br /><button>Отправить</button></p>"
               "<p style=\"text-align: right;\"><small>Обложки книг взяты с интернет-магазина <a href=\"http://www.ozon.ru\" target=\"_blank\"><small>www.ozon.ru</small></a></small></p>'; "
               "echo '<input name=\"book\" hidden=\"true\" value=\"" + qrcode +"\">';"
               "echo '</form>';"
               "?>";
    
}

void engine::getBookDataFromSite(string server) 
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
    string sss = getResponse(&socket, server);
    //cout << sss << endl;
    parsing pars(sss);
    cout << pars.getAutor() << endl;
    
    fOUT.fWrite(sss);
    fOUT.fClose();
    
/*    error = boost::asio::error::host_not_found;
    endpoint_iterator = resolver.resolve(query);
    
    while (error && endpoint_iterator != end)
    {
        socket.close();
        socket.connect(*endpoint_iterator++, error);
    }
    
    fOUT.reopen(server, WRITE);
    
    makeRequest(&socket, server);
    fOUT.fWrite(getResponse(&socket, server));
    fOUT.fClose();*/
}

void engine::makeRequest(tcp::socket* socket, string server) 
{
	boost::asio::streambuf request;
    std::ostream request_stream(&request);
    request_stream.clear();
    request_stream << "GET /" << bookStorage[server] << " HTTP/1.0\r\n"
                   << "Host: " << server << "\r\n"
                   << "Accept: */*\r\n" 
//                   << "Accept-Charset: utf-8\r\n"            
                   << "Connection: close\r\n\r\n";
    boost::asio::streambuf::const_buffers_type bufs = request.data();
    cout << string(boost::asio::buffers_begin(bufs), boost::asio::buffers_begin(bufs) + request.size()) << endl;;
    boost::asio::write(*socket, request);
}

string engine::iconv_recode(string from, string to, string text)
{
    iconv_t cnv = iconv_open(to.c_str(), from.c_str());
    if (cnv == (iconv_t) - 1)
    {
        iconv_close(cnv);
        return "";
    }
    char *outbuf;
    if ((outbuf = (char *) malloc(text.length()*2 + 1)) == NULL)
    {
        iconv_close(cnv);
        return "";
    }
    char *ip = (char *) text.c_str(), *op = outbuf;
    size_t icount = text.length(), ocount = text.length()*2;

    if (iconv(cnv, &ip, &icount, &op, &ocount) != (size_t) - 1)
    {
        outbuf[text.length()*2 - ocount] = '\0';
        text = outbuf;
    }
    else
    {
        text = "";
    }

    free(outbuf);
    iconv_close(cnv);
    return text;
}
