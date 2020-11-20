//
// Created by SiriusNEO on 2020/11/7.
// Filename:DataOperation.cpp
// 数据操作类 实现文件
//

#include "DataOperation.h"

#define InvalidReturn {puts("Invalid");return;}

Book::Book() {
    ISBN = name = author = keyword = "";
    price = 0.0, quantity = 0, isDeleted = false;
}
void Book::Print() const{
    std::cout << ISBN << '\t' << name << '\t' << author << '\t' << keyword << '\t' << std::fixed << std::setprecision(2) << price << '\t' << quantity << '\n';
}
DataOperation::DataOperation(): ISBNHashTable("ISBNindex.bin"), nameHashTable("nameindex.bin"), authorHashTable("authorindex.bin"), keywordHashTable("keywordindex.bin"), dbFileName("bookdb.bin"), billFileName("bill.bin") {
    selectBook = -1;
    tot = 0;
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    if (!dbFile) {
        std::ofstream outFile(dbFileName);
        outFile.close();
    }
    dbFile.close();
    billFile.open(billFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    if (!billFile) {
        std::ofstream outFile(billFileName);
        outFile.close();
        return ;
    }
    billFile.seekg(0, std::ios::beg);
    billFile.read(reinterpret_cast<char *>(&tot), sizeof(int));
    for (int i = 1; i <= tot; ++i) {
        double tmp;
        billFile.read(reinterpret_cast<char *>(&tmp), sizeof(double));
        bill.push_back(tmp);
    }
    billFile.close();
}
DataOperation::~DataOperation() {
    billFile.open(billFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    billFile.seekp(0, std::ios::beg);
    billFile.write(reinterpret_cast<char *>(&tot), sizeof(int));
    for (int i = 0; i < tot; ++i)
        billFile.write(reinterpret_cast<char *>(&bill[i]), sizeof(double));
    billFile.close();
}
int DataOperation::Save(Book book) {
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    int savePos = dbFile.tellp();
    char str[BookMaxl], str1[ISBNMaxl];
    strcpy(str1, book.ISBN.c_str());
    dbFile.write(reinterpret_cast<char *>(&str1), sizeof(str1));
    strcpy(str, book.name.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    strcpy(str, book.author.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    strcpy(str, book.keyword.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    dbFile.write(reinterpret_cast<char *>(&book.price), sizeof(double));
    dbFile.write(reinterpret_cast<char *>(&book.quantity), sizeof(int));
    dbFile.write(reinterpret_cast<char *>(&book.isDeleted), sizeof(bool));
    dbFile.close();
    return savePos;
}
void DataOperation::SaveIn(Book book, int offset) {
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    dbFile.seekp(offset, std::ios::beg);
    char str[BookMaxl], str1[ISBNMaxl];
    strcpy(str1, book.ISBN.c_str());
    dbFile.write(reinterpret_cast<char *>(&str1), sizeof(str1));
    strcpy(str, book.name.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    strcpy(str, book.author.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    strcpy(str, book.keyword.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    dbFile.write(reinterpret_cast<char *>(&book.price), sizeof(double));
    dbFile.write(reinterpret_cast<char *>(&book.quantity), sizeof(int));
    dbFile.write(reinterpret_cast<char *>(&book.isDeleted), sizeof(bool));
    dbFile.close();
}
Book DataOperation::Load(int offset) {
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    Book tmp;
    char str[BookMaxl], str1[ISBNMaxl];
    dbFile.seekg(offset, std::ios::beg);
    dbFile.read(reinterpret_cast<char *>(&str1), ISBNMaxl);
    tmp.ISBN = str1;
    dbFile.read(reinterpret_cast<char *>(&str), BookMaxl);
    tmp.name = str;
    dbFile.read(reinterpret_cast<char *>(&str), BookMaxl);
    tmp.author = str;
    dbFile.read(reinterpret_cast<char *>(&str), BookMaxl);
    tmp.keyword = str;
    dbFile.read(reinterpret_cast<char *>(&tmp.price), sizeof(double));
    dbFile.read(reinterpret_cast<char *>(&tmp.quantity), sizeof(int));
    dbFile.read(reinterpret_cast<char *>(&tmp.isDeleted), sizeof(bool));
    dbFile.close();
    return tmp;
}
int DataOperation::select(const std::string &ISBN, int authority) {
    if (authority < 3) return -1;
    std::vector<int> books =  ISBNHashTable.Find(ISBN.c_str());
    for (int i : books) if (!Load(i).isDeleted) return i;
    return bookadd(ISBN, "", "", "", 0, 0);
}
void DataOperation::updateselect(int newSelect) {
    selectBook = newSelect;
}
int DataOperation::bookadd(const std::string& ISBN, const std::string& name, const std::string& author, const std::string& keyword, double price, int quantity) {
    Book newBook;
    newBook.ISBN = ISBN, newBook.name = name, newBook.author = author, newBook.keyword = keyword, newBook.price = price, newBook.quantity = quantity;
    std::vector <std::string> keywords;
    int i, j;
    for (i = 0, j = 0; i < keyword.size(); ++i) {
        if (keyword[i] == '|') {
            keywords.push_back(keyword.substr(j, i-j));
            j = i + 1;
        }
    }
    if (!keyword.empty()) keywords.push_back(keyword.substr(j, keyword.size()-j));
    for (i = 0 ; i < keywords.size(); ++i)
        for (j = i + 1; j < keywords.size(); ++j)
            if (keywords[i] == keywords[j]) return -1;  //如果关键字重复，加书失败，返回-1，不进行存储操作
    int savePos = Save(newBook);
    ISBNHashTable.Insert(savePos, newBook.ISBN.c_str());
    nameHashTable.Insert(savePos, newBook.name.c_str());
    authorHashTable.Insert(savePos, newBook.author.c_str());
    for (const std::string& kk : keywords) keywordHashTable.Insert(savePos,kk.c_str());
    return savePos;
}
int DataOperation::modify(std::string ISBN, std::string name, std::string author, std::string keyword, double price, int authority) {
    if (authority < 3 || selectBook == -1 || price == double(INVA)) return -1;
    Book tmp1 = Load(selectBook);                                          //找当前选的书
    if (ISBN == "$%default")  ISBN = tmp1.ISBN;                                   //无ISBN，则把当前选的ISBN赋上去
    else {
        std::vector<int> books =  ISBNHashTable.Find(ISBN.c_str());
        for (int i : books) if (!Load(i).isDeleted) return -1;
    }
    if (name == "$%default")  name = tmp1.name;
    if (author == "$%default") author = tmp1.author;
    if (keyword == "$%default") keyword = tmp1.keyword;
    if (price == -1) price = tmp1.price;
    int retPos = bookadd(ISBN, name, author, keyword, price, tmp1.quantity);
    if (retPos != -1) {                                                      //加书没有失败
        tmp1.isDeleted = true;                                               //删掉当前的书
        SaveIn(tmp1, selectBook);
        return retPos;
    }
    else return -1;
}
void DataOperation::import(int quantity, double cost_price, int authority) {
    if (authority < 3 || selectBook == -1 || quantity == INVA || cost_price == double(INVA)) InvalidReturn
    Book tmp = Load(selectBook);
    tmp.quantity += quantity;
    SaveIn(tmp, selectBook);
    bill.push_back(-cost_price), ++tot;
}
void DataOperation::show(const std::string& ISBN, const std::string& name, const std::string& author, const std::string& keyword, int authority) {
    if (!authority) InvalidReturn
    std::vector<int> list, tmp, vand;
    std::vector<Book> books;
    bool allEmpty = true;
    if (ISBN != "$%default") {
        tmp = ISBNHashTable.Find(ISBN.c_str());
        list = tmp;
        allEmpty = false;
    }
    if (name != "$%default") {
        tmp = nameHashTable.Find(name.c_str());
        if (allEmpty) list = tmp;
        else {
            std::set_intersection(list.begin(), list.end(), tmp.begin(), tmp.end(), std::back_inserter(vand));
            list = vand, vand.clear();
        }
        allEmpty = false;
    }
    if (author != "$%default") {
        tmp = authorHashTable.Find(author.c_str());
        if (allEmpty) list = tmp;
        else {
            std::set_intersection(list.begin(), list.end(), tmp.begin(), tmp.end(), std::back_inserter(vand));
            list = vand, vand.clear();
        }
        allEmpty = false;
    }
    if (keyword != "$%default") {
        tmp = keywordHashTable.Find(keyword.c_str());
        if (allEmpty) list = tmp;
        else {
            std::set_intersection(list.begin(), list.end(), tmp.begin(), tmp.end(), std::back_inserter(vand));
            list = vand;
        }
        allEmpty = false;
    }
    Book tmp1;
    if (allEmpty) {
        dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
        int eofPos = dbFile.tellg();
        dbFile.seekg(0, std::ios::beg);
        while (dbFile.tellg() != eofPos) {
            char str[BookMaxl], str1[ISBNMaxl];
            dbFile.read(reinterpret_cast<char *>(&str1), ISBNMaxl);
            tmp1.ISBN = str1;
            dbFile.read(reinterpret_cast<char *>(&str), BookMaxl);
            tmp1.name = str;
            dbFile.read(reinterpret_cast<char *>(&str), BookMaxl);
            tmp1.author = str;
            dbFile.read(reinterpret_cast<char *>(&str), BookMaxl);
            tmp1.keyword = str;
            dbFile.read(reinterpret_cast<char *>(&tmp1.price), sizeof(double));
            dbFile.read(reinterpret_cast<char *>(&tmp1.quantity), sizeof(int));
            dbFile.read(reinterpret_cast<char *>(&tmp1.isDeleted), sizeof(bool));
            if (!tmp1.isDeleted) books.push_back(tmp1);
        }
        dbFile.close();
    }
    else {
        for (int i : list) {
            tmp1 = Load(i);
            if (!tmp1.isDeleted) books.push_back(tmp1);
        }
    }
    std::sort(books.begin(), books.end(), [](const Book& b1, const Book& b2){return b1.ISBN < b2.ISBN;});
    if (books.empty()) puts("");
    else for (const Book& ibook : books) ibook.Print();
}
void DataOperation::buy(const std::string& ISBN, int quantity, int authority) {
    if (!authority || quantity == INVA) InvalidReturn
    std::vector<int> books =  ISBNHashTable.Find(ISBN.c_str());
    Book tmp;
    for (int i : books) {
        tmp = Load(i);
        if (!tmp.isDeleted) {
            if (tmp.quantity < quantity) InvalidReturn
            tmp.quantity -= quantity;
            SaveIn(tmp, i);
            bill.push_back(tmp.price*quantity), ++tot;
            std::cout << std::fixed << std::setprecision(2) << tmp.price*quantity << '\n';
            return;
        }
    }
    InvalidReturn
}
void DataOperation::show_finance(int time, int authority) {
    if (authority < 7 || time > tot || time == INVA) InvalidReturn
    if (time == -1) time = tot;
    double intot = 0, outtot = 0;
    for (int i = tot-time; i < tot; ++i)
        (bill[i] > 0) ? intot += bill[i] : outtot -= bill[i];
    std::cout << std::fixed << std::setprecision(2) << "+ " << intot << " - " << outtot << '\n';
}