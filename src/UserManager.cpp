//
// Created by SiriusNEO on 2020/10/29.
// Filename:UserManager.cpp
// 用户管理类 实现文件
//

#include "UserManager.h"

User::User() {
    id = passwd = name = "";
    authority = 0, isDeleted = false;
    selectBook = -1;
}
void User::Print() const {
    std::cout << id << '$' << passwd << '$' << authority << '$' << name << '$' << isDeleted << '$' << selectBook <<'\n';
}

UserManager::UserManager(): idHashTable("idindex.bin") {
    dbFileName = "userdb.bin";
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    if (!dbFile) {
        std::ofstream outFile(dbFileName);
        outFile.close();
        useradd("root", "sjtu", 7, "root", 8);
        return ;
    }
    dbFile.close();
}
int UserManager::Save(User user) {
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    int savePos = dbFile.tellp();
    char str[UserMaxl];
    strcpy(str, user.id.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    strcpy(str, user.passwd.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    dbFile.write(reinterpret_cast<char *>(&user.authority), sizeof(int));
    strcpy(str, user.name.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    dbFile.write(reinterpret_cast<char *>(&user.isDeleted), sizeof(bool));
    dbFile.close();
    return savePos;
}
void UserManager::SaveIn(User user, int offset) {
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    dbFile.seekp(offset, std::ios::beg);
    char str[UserMaxl];
    strcpy(str, user.id.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    strcpy(str, user.passwd.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    dbFile.write(reinterpret_cast<char *>(&user.authority), sizeof(int));
    strcpy(str, user.name.c_str());
    dbFile.write(reinterpret_cast<char *>(&str), sizeof(str));
    dbFile.write(reinterpret_cast<char *>(&user.isDeleted), sizeof(bool));
    dbFile.close();
}
User UserManager::Load(int offset) {
    dbFile.open(dbFileName, std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    User tmp;
    char str[UserMaxl];
    dbFile.seekg(offset, std::ios::beg);
    dbFile.read(reinterpret_cast<char *>(&str), UserMaxl);
    tmp.id = str;
    dbFile.read(reinterpret_cast<char *>(&str), UserMaxl);
    tmp.passwd = str;
    dbFile.read(reinterpret_cast<char *>(&tmp.authority), sizeof(int));
    dbFile.read(reinterpret_cast<char *>(&str), UserMaxl);
    tmp.name = str;
    dbFile.read(reinterpret_cast<char *>(&tmp.isDeleted), sizeof(bool));
    dbFile.close();
    return tmp;
}
void UserManager::Print() const {
    for (const User &iuser : userStack) iuser.Print();
}
User& UserManager::getNowUser() {
    return userStack[userStack.size()-1];
}
int UserManager::getAuthority() {
    if (userStack.empty()) return 0;
    return userStack[userStack.size()-1].authority;
}
void UserManager::su(const std::string& id, const std::string& passwd) {
    std::vector <int> users = idHashTable.Find(id.c_str());
    bool isFound = false; User tmp;
    for (int i : users) {
        tmp = Load(i);
        if (!tmp.isDeleted) {isFound = true; break;}
    }
    if (!isFound) {
        puts("Invalid");
        return ;
    }
    if (getAuthority() < tmp.authority && passwd.empty()) {
        puts("Invalid");
        return ;
    }
    if (!passwd.empty() && tmp.passwd != passwd) {
        puts("Invalid");
        return ;
    }
    userStack.push_back(tmp);
}
void UserManager::logout(int authority) {
    if (!authority) {
        puts("Invalid");
        return ;
    }
    userStack.pop_back();
}
void UserManager::useradd(const std::string& id, const std::string& passwd, int authority, const std::string& name, int myauthority) {
    if (myauthority <= authority) {
        puts("Invalid");
        return ;
    }
    std::vector <int> users = idHashTable.Find(id.c_str());
    bool isFound = false; User tmp;
    for (int i : users) {
        tmp = Load(i);
        if (!tmp.isDeleted) {isFound = true; break;}
    }
    if (isFound) {
        puts("Invalid");
        return ;
    }
    User newUser;
    newUser.id = id, newUser.passwd = passwd, newUser.authority = authority, newUser.name = name;
    idHashTable.Insert(Save(newUser), id.c_str());
}
void UserManager::reg(const std::string& id, const std::string& passwd, const std::string& name) {
    useradd(id, passwd, 1, name, 7);
}
void UserManager::del(const std::string& id, int authority) {
    if (authority < 7) {
        puts("Invalid");
        return ;
    }
    std::vector <int> users = idHashTable.Find(id.c_str());
    bool isFound = false; User tmp;
    int i;
    for (i = 0; i< users.size(); ++i) {
        tmp = Load(users[i]);
        if (!tmp.isDeleted) {isFound = true; break;}
    }
    if (!isFound) {
        puts("Invalid");
        return ;
    }
    for (const User &iuser : userStack) if (tmp.id == iuser.id) {puts("Invalid");return;}
    tmp.isDeleted = true;
    SaveIn(tmp, users[i]);
}
void UserManager::modify_passwd(const std::string& id, const std::string& newpasswd, int authority, const std::string& oldpasswd) {
    if (!authority) {
        puts("Invalid");
        return ;
    }
    std::vector <int> users = idHashTable.Find(id.c_str());
    bool isFound = false; User tmp;
    for (int i : users) {
        tmp = Load(i);
        if (!tmp.isDeleted) {isFound = true; break;}
    }
    if (!isFound) {
        puts("Invalid");
        return ;
    }
    if (oldpasswd != "I_am_root" && oldpasswd != tmp.passwd) {
        puts("Invalid");
        return ;
    }
    tmp.passwd = newpasswd;
    SaveIn(tmp, users[0]);
}