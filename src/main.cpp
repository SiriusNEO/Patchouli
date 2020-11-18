#include <iostream>
#include <fstream>
#include <cstring>
#include "HashTable.h"
#include "UserManager.h"
#include "DataOperation.h"
#include <ctime>

inline int StringToInt(std::string str) {
    int ret = 0;
    for (int i = 0; i < str.size(); ++i) ret = ret * 10 + str[i] - '0';
    return ret;
}
inline double StringToDouble(std::string str) {
    double ret = 0, base = 0.1;
    int i;
    for (i = 0; i < str.size() && str[i] != '.'; ++i) ret = ret * 10 + str[i] -'0';
    for (++i; i < str.size(); ++i) ret = ret + (str[i] - '0') * base, base *= 0.1;
    return ret;
}
inline bool IsInAlphaBet(char ch) {
    return (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0') && (ch <= '9') || (ch =='-') || (ch == '_');
}

int main() {
    UserManager userManager;
    DataOperation dataOperation;
    std::string cmd, argv[15];
    int argc;
    while (!std::getline(std::cin, cmd, '\n').eof()) {
        argc = 0;
        for (int i = 0; i < 15; ++i) argv[i].clear();
        int i, j;
        for (i = 0, j = 0; i < cmd.size(); ++i) {
            if (cmd[i] == ' ') {
                argv[++argc] = cmd.substr(j, i-j);
                j = i + 1;
            }
        }
        if(IsInAlphaBet(cmd[j]))
            argv[++argc] = cmd.substr(j, cmd.size()-j);
        //for (int i = 1; i <= argc; ++i) std::cout << argv[i] << '$';puts(""); //debug:print operation
        if (argc >= 2 && argc <= 3 && argv[1] == "su") {
            userManager.su(argv[2], argv[3]);
        }
        else if (argc == 5 && argv[1] == "useradd") {
            if (argv[3].size() >= UserMaxl || (argv[4] != "3" && argv[4] != "1")) {
                puts("Invalid");
                continue;
            }
            userManager.useradd(argv[2], argv[3], StringToInt(argv[4]), argv[5], userManager.getAuthority());
        }
        else if (argc == 1 && argv[1] == "logout") userManager.logout(userManager.getAuthority());
        else if (argc == 4 && argv[1] == "register") {
            if (argv[2].size() >= UserMaxl || argv[3].size() >= UserMaxl || argv[4].size() >= UserMaxl) {
                puts("Invalid");
                continue;
            }
            userManager.reg(argv[2], argv[3], argv[4]);
        }
        else if ((argc == 3 || argc == 4) && argv[1] == "passwd") {
            if (argv[2].size() >= UserMaxl || argv[3].size() >= UserMaxl) {
                puts("Invalid");
                continue;
            }
            if (userManager.getAuthority() == 7 && argc == 3) userManager.modify_passwd(argv[2], argv[3], 7);
            else if (argc == 4) userManager.modify_passwd(argv[2], argv[4], userManager.getAuthority(), argv[3]);
            else {
                puts("Invalid");
                continue;
            }
        }
        else if (argc == 2 && argv[1] == "delete") {
            if (argv[2].size() >= UserMaxl) {
                puts("Invalid");
                continue;
            }
            userManager.del(argv[2], userManager.getAuthority());
        }
        else if (argc == 2 && argv[1] == "select") {
            if (argv[1].size() >= ISBNMaxl) {
                puts("Invalid");
                continue;
            }
            if (!userManager.getAuthority()) puts("Invalid");
            else {
                int pos = dataOperation.select(argv[2], userManager.getAuthority());
                if (pos != -1) userManager.getNowUser().selectBook = pos;
                else puts("Invalid");
            }
        }
        else if (argc >= 2 && argc <= 5 && argv[1] == "modify") {
            std::string ISBN, name, author, keyword, tmp;
            double price = -1;
            bool breaker = false;
            for (int k = 2; k <= argc; ++k) {
                tmp = "";
                int l;
                for (l = 0; l < argv[k].size() && argv[k][l] != '='; ++l) tmp += argv[k][l];
                if (l == argv[k].size()) {
                    puts("Invalid");
                    breaker = true;
                    break;
                }
                if (tmp == "-ISBN") ISBN = argv[k].substr(l+1);
                else if (tmp == "-name") name = argv[k].substr(l+2,argv[k].size()-l-3);
                else if (tmp == "-author") author = argv[k].substr(l+2,argv[k].size()-l-3);
                else if (tmp == "-keyword") keyword = argv[k].substr(l+2,argv[k].size()-l-3);
                else if (tmp == "-price") price = StringToDouble(argv[k].substr(l+1));
                else {
                    puts("Invalid");
                    breaker = true;
                    break;
                }
            }
            if (breaker) continue;
            if (ISBN.size() >= ISBNMaxl || name.size() >= BookMaxl || author.size() >= BookMaxl || keyword.size() >= BookMaxl) {
                puts("Invalid");
                continue;
            }
            int pos = dataOperation.modify(ISBN, name, author, keyword, price, userManager.getAuthority());
            if(pos != -1) userManager.getNowUser().selectBook = pos;
            else puts("Invalid");
        }
        else if (argc == 3 && argv[1] == "import") {
            dataOperation.import(StringToInt(argv[2]), StringToDouble(argv[3]), userManager.getAuthority());
        }
        else if (argc >= 1 && argc <= 4 && argv[1] == "show" && argv[2] != "finance") {
            std::string ISBN, name, author, keyword, tmp;
            bool breaker = false;
            for (int k = 2; k <= argc; ++k) {
                tmp = "";
                int l;
                for (l = 0; l < argv[k].size() && argv[k][l] != '='; ++l) tmp += argv[k][l];
                if (l == argv[k].size()) {
                    puts("Invalid");
                    breaker = true;
                    break;
                }
                if (tmp == "-ISBN") ISBN = argv[k].substr(l+1);
                else if (tmp == "-name") name = argv[k].substr(l+2,argv[k].size()-l-3);
                else if (tmp == "-author") author = argv[k].substr(l+2,argv[k].size()-l-3);
                else if (tmp == "-keyword") keyword = argv[k].substr(l+2,argv[k].size()-l-3);
                else {
                    puts("Invalid");
                    breaker = true;
                    break;
                }
            }
            if (breaker) continue;
            if (ISBN.size() >= ISBNMaxl || name.size() >= BookMaxl || author.size() >= BookMaxl || keyword.size() >= BookMaxl) {
                puts("Invalid");
                continue;
            }
            dataOperation.show(ISBN, name, author, keyword, userManager.getAuthority());
        }
        else if (argc == 1 && (argv[1] == "quit" || argv[1] == "exit")) {
            return 0;
        }
        else if (argc >= 2 && argc <= 3 && argv[1] == "show" && argv[2] == "finance") {
            if (argc == 2) dataOperation.show_finance(-1, userManager.getAuthority());
            else dataOperation.show_finance(StringToInt(argv[3]), userManager.getAuthority());
        }
        else if (argc == 3 && argv[1] == "buy") {
            if (argv[2].size() >= ISBNMaxl) {
                puts("Invalid");
                continue;
            }
            dataOperation.buy(argv[2], StringToInt(argv[3]), userManager.getAuthority());
        }
        else {
            puts("Invalid");
            //std::cout << argv[1] << " " << argv[2] << '\n';
            continue ;
        }
        //userManager.Print();
        //dataOperation.Print();
    }
    return 0;
}