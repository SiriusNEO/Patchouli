//
// Created by SiriusNEO on 2020/11/7.
// Filename:DataOperation.h
// 数据操作类 声明文件
//

#ifndef BOOKSTORESYSTEM_DATAOPERATION_H
#define BOOKSTORESYSTEM_DATAOPERATION_H

#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <fstream>
#include <algorithm>
#include <iomanip>
#include "HashTable.h"

const int ISBNMaxl = 21;
const int BookMaxl = 61;
const int INVA = -2147483643;

class Book {
    public:
        std::string ISBN, name, author, keyword;
        double price;
        int quantity;
        bool isDeleted;
        Book();
        void Print() const;
};

class DataOperation {
    private:
        int selectBook;
        int tot;
        std::fstream dbFile, billFile;
        const std::string dbFileName, billFileName;
        HashTable ISBNHashTable, nameHashTable, authorHashTable, keywordHashTable;

    public:
        std::vector<double> bill;
        DataOperation();
        int Save(Book book);
        void SaveIn(Book book, int offset);
        Book Load(int offset);
        int bookadd(const std::string& ISBN, const std::string& name, const std::string& author, const std::string& keyword, double price, int quantity);
        int select(const std::string& ISBN, int authority);
        void updateselect(int newSelect);
        int modify(std::string ISBN, std::string name, std::string author, std::string keyword, double price, int authority);
        int import(int quantity, double cost_price, int authority);
        int show(const std::string& ISBN, const std::string& name, const std::string& author, const std::string& keyword, int authority);
        int buy(const std::string& ISBN, int quantity, int authority);
        int show_finance(int time, int authority);
        int report_finance(int authority);
};

#endif //BOOKSTORESYSTEM_DATAOPERATION_H