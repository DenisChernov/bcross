/* 
 * File:   parsing.cpp
 * Author: demiin
 * 
 * Created on March 21, 2014, 10:43 AM
 */



#include "parsing.h"
#include "fOper.h"

parsing::parsing(string line_for_regex) 
{
    line = line_for_regex;
}

parsing::~parsing() {
}

string parsing::parseLine(pars_vars mode) 
{
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    
    switch (mode)
    {
        case OBJ_MOVED:
        {
            boost::regex re("Object moved to.*\"(.*)\".*");

            while (boost::regex_search(begin, end,  result, re))
            {
                begin = result[2].second;
                
                if (boost::lexical_cast<string>(result[1]).length() > 0)
                    return boost::lexical_cast<string>(result[1]);
            }
            
            break;
        }
        
        case LOCATION:
        {
            return line.erase(line.length()-1, 1).erase(0, line.find("context"));
            break;
        }
    
    }
    
    return "";
}

string parsing::maxMFN() 
{
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string lastResult;
//    cout << line << endl;
    boost::regex re("(\\d{3})");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[1].second;
        if (boost::lexical_cast<string>(result[1]).length() > 0)
            lastResult = boost::lexical_cast<string>(result[1]);
    }    
    cout << lastResult << endl;
    return lastResult;
}

map<string, string> parsing::getDeliveredKSU() 
{
    map <string, string> ksu;
//    cout << line << endl;
//    cout << endl;
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    //boost::regex re("^\\d{1,6}#\\d{0,6}#([0-9\\/]*).*\\^CКРФ\\^A(\\d{8})");
    boost::regex re("\\d{1,6}#(\\d{4}\\/\\d{1,3})&&&&&.*\\^CКРФ\\^A(\\d{8})");
    while (boost::regex_search(begin, end,  result, re))
    {
        
                
        if (boost::lexical_cast<string>(result[1]).length() > 0)
        {
        //    cout << "ksu: " << boost::lexical_cast<string>(result[1]) << endl;
        //    cout << "date: " << boost::lexical_cast<string>(result[2]) << endl;
            ksu[boost::lexical_cast<string>(result[1])] = boost::lexical_cast<string>(result[2]);
        }
        
        begin = result[1].second;
    }    
    
    return ksu;
}

vector<string> parsing::split() 
{
    vector <string> splited;
//    cout << line << endl;
    size_t pos = 0;
    size_t new_pos = 0;
    
    while ((new_pos = line.find('$', pos)) != string::npos)
    {
        splited.push_back(line.substr(pos, new_pos-pos));
//        cout << currentLine << ": "<< line.substr(pos, new_pos-pos) << endl;
        pos = new_pos+1;
    }
    return splited;
}

pair <string, vector<string>> parsing::parseBookRecord(string ksu) 
{
    vector <string> filials;
    //cout << line << endl;
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();

    boost::regex re_isbn("\\^A([0-9а-яА-ЯA-Za-z\\-]*)");
    boost::regex re_filials(ksu + "\\-\\-\\-([фФЦГБАОДИКХОЛМП\\s0-9]*)");
    
    boost::regex_search(begin, end, result, re_isbn);
    if (boost::lexical_cast<string>(result[1]).length() > 0)
    {
        //cout << result[1] << "\t";
        begin = result[2].second;
    }

    boost::smatch result_filials;
    //cout << "filials: ";
    while (boost::regex_search(begin, end, result_filials, re_filials))
    {
        if (boost::lexical_cast<string>(result_filials[1]).length() > 0) 
        {
            //cout << result_filials[1] << ", ";
            filials.push_back(result_filials[1]);
        }
        begin = result_filials[2].second;
    }
    // cout << endl;
    
    
    //return make_pair(boost::lexical_cast<string>(result[1]), filials);
    pair<string, vector<string>> sss = make_pair(result[1], filials);
    //cout << "returning isbn: " << sss.first << endl;
    
    //return pair<string, vector<string>>(sss != "" ? sss.first : "!!!", filials);
    return sss;
}

//

string parsing::getAddrFullCover() 
{
    string fullpath;
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    boost::smatch result;

    boost::regex re("eMicroGallery_fullImage\" src=\"\\/\\/([a-z0-9\\.\\/_]*)");
    while (boost::regex_search(begin, end, result, re))
    {
        if (boost::lexical_cast<string>(result[1]) != "")
            fullpath = boost::lexical_cast<string>(result[1]);        
        begin = result[2].second;
    }
    
    return fullpath;
}

string parsing::getAnnotation() 
{
    string annotation;
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    boost::smatch result;

//    boost::regex re("<!-- Data\\[ANNOTATION\\] -->([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.0-9\\-,<br>]*)");
    boost::regex re("<!-- Data\\[ANNOTATION\\] -->(.*)");
    while (boost::regex_search(begin, end, result, re))
    {
        if (boost::lexical_cast<string>(result[1]) != "")
            annotation = boost::lexical_cast<string>(result[1]);
        begin = result[2].second;
    }    
    
    if (annotation != "")
    {
        annotation.replace(annotation.find_first_of("<br>", 0), 4, "");
        annotation.replace(annotation.find_first_of("<BR>", 0), 4, "");
        annotation.replace(annotation.find_first_of("</td>", 0), annotation.length() - annotation.find_first_of("</td>", 0) - 2, "");
    //    annotation.replace(annotation.find_first_of("e>", 0), annotation.length() - annotation.find_first_of("e>", 0) - 2, "");
    }
    
    return annotation;
}

string parsing::linkToNewAddrBook_fromMany() 
{
    string newPath;
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    boost::smatch result;
    
    //boost::regex re("\\A  <div itemprop=\"itemListElement\".*data-omniture-suffix=\"pic\" href=\"\\/(context\\/detail\\/id\\/[0-9\\/\\._]*)\".*");
    boost::regex re("(tilesTotalCount)");
    
    boost::regex re_newpath(".*<div class=\"bOneTile inline jsUpdateLink \" data-href=\"/([0-9a-zA-Z\\/]*)\".*");
        if (boost::regex_search(line, result, re_newpath)) {
            newPath = result[1];
        }
/*    bool hasNewPath = false;
    while (boost::regex_search(begin, end, result, re))
    {
        cout << result[1] << endl;
        if (boost::lexical_cast<string>(result[1]) != "") {
            hasNewPath = true;
            break;
        }
        begin = result[2].second;
    }
    if (hasNewPath) {
        if (boost::regex_search(begin, end, result, re_newpath)) {
            newPath = result[1];
        }
    }
*/    
    return newPath;
}

bool parsing::nothingFindByISBN() 
{
    if (line.find_first_of("Мы ничего не нашли по запросу", 0) != string::npos)
        return true;
    
    return false;
}

string parsing::getBookNameAndFIO() 
{
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string lastResult;
    
    boost::regex re(".*&&&&&(.*)\\$");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[2].second;
                
        if (boost::lexical_cast<string>(result[1]).length() > 0)
            lastResult = boost::lexical_cast<string>(result[1]);
    }    
    
    while (lastResult.find_first_of(" ", 0) != string::npos)
        lastResult.replace(lastResult.find_first_of(" ", 0), 1, "+");
    
    return lastResult;
}

string parsing::getCookie() 
{
    cout << "cook: " << line.substr(12, 80) << endl;
    return line.substr(12, 80);
}

string parsing::getFormID() 
{
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string id;
    
//    <input type="hidden" name="form_token" id="edit-page-node-form-form-token" value="8f126a43db3df7a5cc4cada8c4f1683f"  />

//    boost::regex re("name=\"(.*)\"\\sid=\".*\".*value=\"(.*)\"");
    boost::regex re("name=\"form_build_id\"\\svalue=\"([a-zA-Z0-9\\-_]*)\"");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[2].second;
                
        //if (boost::lexical_cast<string>(result[1]) == "form_build_id")
            id = boost::lexical_cast<string>(result[1]);

    }    
    
    return id;    
}

string parsing::getFormToken() 
{
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string token;
    
    boost::regex re("name=\"form_token\"\\svalue=\"([a-zA-Z0-9\\-_]*)\"");
    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[2].second;
                
        //if (boost::lexical_cast<string>(result[1]) == "form_token")
        if (result[1] != "")
        {
            token = boost::lexical_cast<string>(result[1]);
            cout << token << endl;
            break;
        }  
            
    }    
    
    return token;    
}

string parsing::remakeBooklist() 
{
    size_t begin_pos = line.find_first_of("\x1F", 0) + 1;
    return line.substr(begin_pos, line.length() - begin_pos - 1);
}

vector<books_record> parsing::parseBookRecord() 
{
    vector <books_record> booksList;
    books_record book;
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    fOper in("booklist.txt", OPEN);
    for (int i = 0; i < 12; i++)
    {
        in.fRead();
    }      
    
    
    boost::regex re("&&&&&1@([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮa-zA-Z\\s\\.\\,\\-0-9;:\\!\"\\?\\(\\)]*)---2@([\\-0-9йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s\\.;\\,:\\!\"\\?\\(\\)]*)---3@([0-9\\-a-zA-ZХ]*)&&&&&");
    //size_t counter = 1;
    while (boost::regex_search(begin, end,  result, re))
    {
        string ln = in.fRead();
        size_t begin_pos = ln.find_first_of("\x1F", 0) + 1;
    //    cout << counter++ << ".\t"<< result[0] << endl;;
        begin = result[3].second;
        //cout << "1#: " << result[1] << "\t2#: " << result[2] << "\t3#: " << result[3] <<endl;
        book.bookname = result[1];
        book.fio = result[2];
        book.isbn = result[3];
        book.bookrecord = ln.substr(begin_pos, ln.length() - begin_pos - 1);
        
        booksList.push_back(book);
        
        book.bookname.clear();
        book.fio.clear();
        book.isbn.clear();
        book.bookrecord.clear();
    }    
    cout << "total: " << booksList.size() << endl;
    
    return booksList;
}

string parsing::getNewLocation() 
{
    return line.substr(29, line.length() - 30);
}

vector<string> parsing::isbn_bookname_fio() 
{
   
    vector<string> tmp;
    boost::regex re("([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\-0-9\\.\\,\\sa-zA-Z]*)%([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\-0-9\\.\\,\\sa-zA-Z]*)%([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\-0-9\\.\\,\\sa-zA-Z]*)");
    boost::smatch result;
    if (boost::regex_search(line, result, re))
    {
        tmp.push_back(result[1]);
        tmp.push_back(result[2]);
        tmp.push_back(string(result[3]).substr(0, string(result[3]).length() - 1));
    }
    return tmp;
}

string parsing::replace_all_space() 
{
    size_t pos = 0;
    size_t new_pos = 0;
    while ((new_pos = line.find(" ", pos)) != string::npos)
    {
        line.replace(new_pos, 1, "%20");
        pos = new_pos+1;
    }
    return line;
}

string parsing::countFoundedRecords() 
{
     vector <string> splited;
    size_t pos = 0;
    size_t new_pos = 0;
    
    while ((new_pos = line.find('\n', pos)) != string::npos)
    {
        splited.push_back(line.substr(pos, new_pos-pos));
//        cout << "currentLine" << ": "<< line.substr(pos, new_pos-pos) << endl;
        pos = new_pos+1;
    }

    return splited.back();;
}

string parsing::getLinebookRecord() 
{
    vector <string> splited;
    size_t pos = 0;
    size_t new_pos = 0;
    
    while ((new_pos = line.find('\n', pos)) != string::npos)
    {
        splited.push_back(line.substr(pos, new_pos-pos));
//        cout << "currentLine" << ": "<< line.substr(pos, new_pos-pos) << endl;
        pos = new_pos+1;
    }
    string s = splited.back().substr(0, splited.back().length() - 1);
    
    return s;
}

string parsing::getAutor() {
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    line= iconv_recode("cp1251", "utf-8", line);
    boost::regex re(".*itemprop=\"author\".*title='([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s]*)'>.*");
    //boost::regex re(".*title='([йцукенгшщзхъфывапролджэячсмитьбюЙЦУКЕНГШЩЗХЪФЫВАПРОЛДЖЭЯЧСМИТЬБЮ\\s]*)'.*");

    if (boost::regex_search(line,  result, re))
    {
        //begin = result[2].second;
        return result[1];
    }    
    
    return "";
}

string parsing::getBookname() {
  // <div id="DetailChooseFormat">  </div> </div> <!-- Подписка на новинки категории -->  </div> </div> <div class="bCombiningColumn"> <div class="bContentColumn"> <div class="bContentBlock" >  <h1 class="fn" itemprop="name">Linux. Необходимый код и команды. Карманный справочник</h1>  <div class="eDetail_ProductId">ID 3178480</div>  <div id="marketing-tags" class="bMarketMessages_Tags js_scrolload " data-scrolload='{"title": "Загрузка маркетинговых сообщений...", "module": "marketing-tags", "id": 3178480 }'>  <div class="eMarketMessages_Tag" title="Наиболее популярный, хорошо продающийся на Ozon.ru товар">Бестселлер</div>  </div> <div id="rating_bar" class="js_scrolload bProductRating" data-scrolload='{"title": "Загружается рейтинг", "module": "rating-vote-buttons", "id": 3178480, "modelId": 3178480, "callback" : "initRatingVoteButtonsContainer" }'>  <div itemprop="aggregateRating" itemscope itemtype="http://schema.org/AggregateRating"> <a href="#tab_comments" class="bProductRating_Stars js_DetailCommentCount"> <div class="bStars inline m5 "> <div class="eStarsIn"></div> </div> (<span>46 отзывов</span>) </a>  <noscript> <a href="/reviews/3178480/" class="bProductRating_Stars"> <div class="bStars inline m5 "> <div class="eStarsIn"></div> </div> (<span>46 отзывов</span>) </a> <style> .js_DetailCommentCount
    string::const_iterator begin = line.begin();
    string::const_iterator end   = line.end();
    string bookname;
//    line= iconv_recode("cp1251", "utf-8", line);

    boost::regex re("<h1\\sclass=\"fn\"\\sitemprop=\"name\">(.*)<\\/h1>");
    if (boost::regex_search(line, result, re)) {
        return result[1];
    }
/*    boost::regex re("(itemprop=\"name\">)(.*)<\\/h1>");

    while (boost::regex_search(begin, end,  result, re))
    {
        begin = result[2].second;
        
        int pos_first = 0;
        int pos_second = 0;
    
        bookname = result[2];
        
        for (int i = 0; i < bookname.length(); i++)
        {
            if (bookname.at(i) != ' ')
            {
                bookname = bookname.substr(i, bookname.length() - i);
                break;
            }
        }

        for (int i = bookname.length()- 1; i >= 0; i--)
        {
            if (bookname.at(i) != ' ')
            {
                bookname = bookname.substr(0, i+1);
                break;
            }
        }
        
        return bookname;
    }    
*/    
    
    return "";
}

string parsing::removeBeginEndWhiteSpaces() 
{
    int pos_first = 0;
    int pos_second = 0;
    
    for (int i = 0; i < line.length(); i++)
    {
        if (line.at(i) != ' ')
        {
            line = line.substr(i, line.length() - i);
            break;
        }
    }

    for (int i = line.length()- 1; i >= 0; i--)
    {
        if (line.at(i) != ' ')
        {
            line = line.substr(0, i+1);
            break;
        }
    }
    
    return line;
}

string parsing::removeBeginEndWhiteSpaces(string line) 
{
    int pos_first = 0;
    int pos_second = 0;
    
    for (int i = 0; i < line.length(); i++)
    {
        if (line.at(i) != ' ')
        {
            line = line.substr(i, line.length() - i);
            break;
        }
    }

    for (int i = line.length()- 1; i >= 0; i--)
    {
        if (line.at(i) != ' ')
        {
            line = line.substr(0, i+1);
            break;
        }
    }
    
    return line;
}

bool parsing::haveFioOrBookname() {
    boost::regex re("1#(.*)2#");
    boost::smatch result;
    cout << line << endl;
    if (boost::regex_search(line, result , re)){
        cout << "fio: " << result[1] << endl;
        return true;
    }
    boost::regex re2("3#(.*)7#");
    if (boost::regex_search(line, result , re2)){
        cout << "fio: " << result[1] << endl;
        return true;
    }
    
    return false;
}

string parsing::iconv_recode(string from, string to, string text)
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