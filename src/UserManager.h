//
// Created by SiriusNEO on 2020/10/29.
// Filename:UserManager.h
// 用户管理类 声明文件
//

#ifndef BOOKSTORESYSTEM_USERMANAGER_H
#define BOOKSTORESYSTEM_USERMANAGER_H

#include "HashTable.h"
#include "DataOperation.h"
#include <fstream>
#include <string>
#include <vector>
#include <iostream>
#include <cstring>

const int UserMaxl = 31;

class User {
    public:
        std::string name, id, passwd;
        int authority;
        bool isDeleted;
        int selectBook;
        User();
        void Print() const;
};

class UserManager {
    private:
        std::vector <User> userStack;
        std::fstream dbFile;
        std::string dbFileName;
        HashTable idHashTable;

    public:
        UserManager();
        int Save(User user);
        void SaveIn(User user, int offset);
        User Load(int offset);
        void Print() const;
        User& getNowUser();
        int getAuthority();
        void su(const std::string& id, const std::string& passwd);
        void logout(int authority);
        void useradd(const std::string& id, const std::string& passwd, int authority, const std::string& name, int myauthority);
        void reg(const std::string& id, const std::string& passwd, const std::string& name);
        void del(const std::string& id, int authority);
        void modify_passwd(const std::string& id, const std::string& newpasswd, int authority, const std::string& oldpasswd = "I_am_root");
};

#endif //BOOKSTORESYSTEM_USERMANAGER_H