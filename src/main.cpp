#include <iostream>
#include "HashTable.h"
#include "UserManager.h"
#include <unistd.h>

#pragma GCC optimize(3)

#define MaxQL 301

#define purple "\033[35m"
#define red "\033[31m"
#define green "\033[32m"
#define yellow "\033[33m"
#define blue "\033[34m"
#define highlight "\033[1m"
#define background "\033[47m"
#define blank "                           "
#define rmv puts("\033[0m");

/* Init */
UserManager userManager;
DataOperation dataOperation;
std::string cmd, argv[15];
std::vector<std::string> logstr, opuser;
int logc, argc;
std::fstream logFile;
bool digitalWindow = true;

char Patchouli[] =
        background
        red
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO`.../OOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO/[[\\OOOOOOOOOOOOOO.ooo/OOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO/`                [\\OOOOO^=ooo    =OOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO/                       ,OOO,oo^     [O/[.,OOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO[`                                   [o^    ..]/oooOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOOOOOO`                              .          . .]ooooooOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOO[  ,OOOO/                                            ,ooo/` \\OOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOO/[  ...*.OO/                                             . ,.    \\OOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOO] ...*../                               .. .               OOO]/OOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOO`..*..                    .      /oooooo^  .            OOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOO\\....               ,]oo]`.  ,oooooooo    ....        OOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO\\ ....**.......            ./oooooOoooooOoo[[\\/ooOooo .        OOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOO\\ ...*..*.....       /ooooOooooooo\\oOooo^=^**`,\\ooooO`     ,` \\OOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOO\\.,]/OOO`         /o[[\\ooooo\\/=o^ \\\\/[`=****^=**oooOo\\]/OoooooOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOO`          oo`**o**=*=.^*,` .``..*o.^*.,,[[,,\\oooOoooOoooOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOO`       ,ooooo^,*=^..=..  ,`.  =^**=.^*= =]]=^,.,oooOooOoooOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOooo].,/ooooOoo^ .`,/*=.\\.   ` .,O[[[  =`     o/]^,OoOoOoOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOooooooooooOooo`o^*^=*^]/O\\]]`               O[OOOOoOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOoOooooOooooo^*^o*=/. OOOO^,`        ]]]]`,\\OoOooOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOO^oOoooooOoo^*^/^*\\,\\,O`,          ,]//  ,OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOO^oOooOoooooo\\^o\\**\\..             \\`,  ,OOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOO=\\oooO*oooooOooOoo^`                   OOOO\\oOOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOO=*oOoo^=ooooOooooO\\                   =OOOo*=oOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOO^^*oOOoo`o,oooOooOOo^                 /OOOOo*=oOOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOOo**oOOOoo^.oooOOooOOo`            ,]OOOOOOoo*OooOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOOO^*,OOOOOoo\\,ooooOooOoo   .   ]/OOOOOOoOOOoOo=OooOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOOOO/**oOOOOOOooOoooooOooOo\\     OOOOOOOoOooOooOooOoOoOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOOOO\\/**=ooOOoOOOooOoOooooooOo^     ,\\OOOOOoOooooOooOooOoOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOO//\\ ,/ooOO/`   .ooOooooooOoOo.   .  ,   .oOooooOoooooOOoOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOO\\o`*/o.\\/`       .ooo^oooooOoo\\       ^  .oOooooooOOooOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOOOo*/oOo/           ,=o^,oooOoOOo`       . .\\O\\oOoooOOooOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOOo\\OooOo/             =*\\*[[[\\Oooo^.     .  =*o=oOoooo^=OOOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOoOOooOo                ,*o*\\OooOOoo        ...],]//oooo ,OOOOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOoOOOooO.                  ^,^*\\OoOooO.**..*   .`=*\\ooooo^   \\OOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOO/[     ` ,.            .`\\`*\\OoOo/......    .o^,oOOoO`    =OOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOOOooOo\\   .       \\         `\\o]oOOO ***`**^     ,o/OO/        \\OOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOO*/Oooo]oo` ^       .  .*,   ,/ooO/. .**..*=       `,\\ooo*]]` ,`,\\OOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO^*OOooOOooo`              ,*.  . ..*. ..` **^   .[ooOOOo\\*... ]  ,]]OOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO*=oooOoOOO              .    ...*.]/\\*.**..*^  *./oo=oO\\.[` /OOOoOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOO^*oOooooo/             =\\.. ....*ooooO^,** .*=   o[.^=oOOo.  =OOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOO^*oOoOoo^             /ooO ..,*.,o\\ooOO^.. .*,`     *ooOOoo   OOOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOO^*oooOo`             ooooo^     *o=oooOo*. .* ^    =*\\oOoOo   ,OOOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO*\\ooO              oooooo^     ,=ooOoOo*. .. ^    =O/O/[[     \\OOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO`=Oo             ,oooooooo...]..[oOOO/...... o    ......,*`.`..OOOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO^=O             =oooOooOo^,,]]`ooo.,/ooooo^ =/   ..,]=OOO\\.. . =OOOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOO,             =oooOOooOoo` `.   =\\OO ..  .  `.    =^/oOOO^     \\OOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOOO             /oooOOOooOoo^     /,ooo\\.   *  =^    ,^ooOOOo      OOOOOOOOOOOOOOOOOOOOOO\n"
        blank "OOOOOOOOOOOO^            /ooooOOOooOoo     =./oooO\\  .* .*,    =O`ooOo`       OOOOoOOoo/oOOOOOOOOoO\n"
        blank "OOOOOOOOOOO/            /oooooOOOooo^      ^.//OoOo. .* .,/.    ,O^,O^        ,OOO\\OOOOOOOOOOOOOOOO";

inline int StringToInt(const std::string& str) {
    int ret = 0;
    for (char ch : str) {
        if (ch < '0' || ch > '9') return INVA;
        ret = ret * 10 + ch - '0';
    }
    return ret;
}
inline double StringToDouble(std::string str) {
    double ret = 0, base = 0.1;
    int i;
    for (i = 0; i < str.size() && str[i] != '.'; ++i) {
        if (str[i] < '0' || str[i] > '9') return double(INVA);
        ret = ret * 10 + str[i] -'0';
    }
    for (++i; i < str.size(); ++i) {
        if (str[i] < '0' || str[i] > '9') return double(INVA);
        ret = ret + (str[i] - '0') * base, base *= 0.1;
    }
    return ret;
}
void LogInit() {
   logFile.open("log.bin", std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
    if (!logFile) {
        std::ofstream outFile("log.bin");
        outFile.close();
        return;
    }
    logFile.seekg(0, std::ios::beg);
    logFile.read(reinterpret_cast<char *>(&logc), sizeof(int));
    for (int i = 1; i <= logc; ++i) {
        char str[MaxQL], str1[UserMaxl];
        logFile.read(reinterpret_cast<char *>(&str1), UserMaxl);
        opuser.push_back(std::string(str1));
        logFile.read(reinterpret_cast<char *>(&str), MaxQL);
        logstr.push_back(std::string(str));
    }
    logFile.close();
}
int Command() {
    argc = 0;
    for (int i = 0; i < 15; ++i) argv[i].clear();
    int ii = 0, jj, len = cmd.size();
    while ((cmd[ii] == ' ' || cmd[ii] == '\t') && ii < cmd.size())  ++ii;
    while (cmd[len-1] == ' ' || cmd[len-1] == '\t') --len;
    for (jj = ii; ii < len; ++ii) {
        if (cmd[ii] == ' ') {
            argv[++argc] = cmd.substr(jj, ii-jj);
            jj = ii + 1;
        }
    }
    if (jj < len) argv[++argc] = cmd.substr(jj, len-jj);
    //for (int i = 1; i <= argc; ++i) std::cout << argv[i] << '$';puts(""); //debug:print operation
    if (argc >= 2 && argc <= 3 && argv[1] == "su") return userManager.su(argv[2], argv[3]);
    else if (argc == 5 && argv[1] == "useradd") {
        if (argv[3].size() >= UserMaxl || (argv[4] != "3" && argv[4] != "1")) return -1;
        return userManager.useradd(argv[2], argv[3], StringToInt(argv[4]), argv[5], userManager.getAuthority());
    }
    else if (argc == 1 && argv[1] == "logout") {
        return userManager.logout(userManager.getAuthority());
    }
    else if (argc == 4 && argv[1] == "register") {
        if (argv[2].size() >= UserMaxl || argv[3].size() >= UserMaxl || argv[4].size() >= UserMaxl) return -1;
        return userManager.reg(argv[2], argv[3], argv[4]);
    }
    else if ((argc == 3 || argc == 4) && argv[1] == "passwd") {
        if (argv[2].size() >= UserMaxl || argv[3].size() >= UserMaxl) return -1;
        if (userManager.getAuthority() == 7 && argc == 3) {
            return userManager.modify_passwd(argv[2], argv[3], 7);
        }
        else if (argc == 4) {
            return userManager.modify_passwd(argv[2], argv[4], userManager.getAuthority(), argv[3]);
        }
        else return -1;
    }
    else if (argc == 2 && argv[1] == "delete") {
        if (argv[2].size() >= UserMaxl) return -1;
        return userManager.del(argv[2], userManager.getAuthority());
    }
    else if (argc == 2 && argv[1] == "select") {
        if (argv[1].size() >= ISBNMaxl) return -1;
        else {
            int pos = dataOperation.select(argv[2], userManager.getAuthority());
            if (pos != -1) return userManager.getNowUser().selectBook = pos;
            else return -1;
        }
    }
    else if (argc >= 2 && argc <= 5 && argv[1] == "modify") {
        dataOperation.updateselect(userManager.getNowUser().selectBook);
        std::string ISBN="$%default", name="$%default", author="$%default", keyword="$%default", tmp;
        double price = -1;
        for (int k = 2; k <= argc; ++k) {
            tmp = "";
            int l;
            for (l = 0; l < argv[k].size() && argv[k][l] != '='; ++l) tmp += argv[k][l];
            if (l == argv[k].size()) return -1;
            if (tmp == "-ISBN") ISBN = argv[k].substr(l+1);
            else if (tmp == "-name") name = argv[k].substr(l+2,argv[k].size()-l-3);
            else if (tmp == "-author") author = argv[k].substr(l+2,argv[k].size()-l-3);
            else if (tmp == "-keyword") keyword = argv[k].substr(l+2,argv[k].size()-l-3);
            else if (tmp == "-price") price = StringToDouble(argv[k].substr(l+1));
            else return -1;
        }
        if (ISBN.size() >= ISBNMaxl || name.size() >= BookMaxl || author.size() >= BookMaxl || keyword.size() >= BookMaxl) return -1;
        int oldpos = userManager.getNowUser().selectBook, pos = dataOperation.modify(ISBN, name, author, keyword, price, userManager.getAuthority());
        if(pos != -1) userManager.updateselect(oldpos, pos);
        else puts("Invalid");
        return pos;
    }
    else if (argc == 3 && argv[1] == "import") {
        dataOperation.updateselect(userManager.getNowUser().selectBook);
        return dataOperation.import(StringToInt(argv[2]), StringToDouble(argv[3]), userManager.getAuthority());
    }
    else if (argc >= 1 && argc <= 4 && argv[1] == "show" && argv[2] != "finance") {
        std::string ISBN="$%default", name="$%default", author="$%default", keyword="$%default", tmp;
        for (int k = 2; k <= argc; ++k) {
            tmp = "";
            int l;
            for (l = 0; l < argv[k].size() && argv[k][l] != '='; ++l) tmp += argv[k][l];
            if (l == argv[k].size()) return -1;
            if (tmp == "-ISBN") ISBN = argv[k].substr(l+1);
            else if (tmp == "-name") name = argv[k].substr(l+2,argv[k].size()-l-3);
            else if (tmp == "-author") author = argv[k].substr(l+2,argv[k].size()-l-3);
            else if (tmp == "-keyword") keyword = argv[k].substr(l+2,argv[k].size()-l-3);
            else return -1;
        }
        if (ISBN.size() >= ISBNMaxl || name.size() >= BookMaxl || author.size() >= BookMaxl || keyword.size() >= BookMaxl) return -1;
        return dataOperation.show(ISBN, name, author, keyword, userManager.getAuthority());
    }
    else if (argc == 1 && (argv[1] == "quit" || argv[1] == "exit")) {
        return -2;
    }
    else if (argc >= 2 && argc <= 3 && argv[1] == "show" && argv[2] == "finance") {
        if (argc == 2) return dataOperation.show_finance(-1, userManager.getAuthority());
        else return dataOperation.show_finance(StringToInt(argv[3]), userManager.getAuthority());
    }
    else if (argc == 3 && argv[1] == "buy") {
        if (argv[2].size() >= ISBNMaxl) return -1;
        if(dataOperation.buy(argv[2], StringToInt(argv[3]), userManager.getAuthority()) == -1) return -1;
    }
    else if (argc == 1 && argv[1] == "log") {
        std::puts("*-----------------------------------------------------------------------------------------------------------------------------------------------*");
        std::cout << "#All " << logc << " Operations are listed as follows:\n";
        for (int i = 0; i < logc; ++i)
            std::cout << '\t' << opuser[i]+", do ["+logstr[i] << "\n";
        std::cout << "#Finance Flows are listed as follows:\n";
        int j = 0;
        for (int i = 0; i < logc; ++i)
            if((logstr[i][0]=='b'||logstr[i][0]=='i')&&logstr[i][logstr[i].size()-2] == 's') {
                std::cout << logstr[i].substr(0, logstr[i].size() - 12);
                if (dataOperation.bill[j] > 0) std::cout << ", + " << std::fixed << std::setprecision(2) << dataOperation.bill[j] << '\n';
                else std::cout << ", - " << std::fixed << std::setprecision(2) << -dataOperation.bill[j] << '\n';
                ++j;
            }
        std::puts("*-----------------------------------------------------------------------------------------------------------------------------------------------*");
    }
    else if (argc == 2 && argv[1] == "report" && argv[2] == "employee") {
        if (userManager.getAuthority() < 7) return -1;
        int mylogc = 0;
        for (int i = 0; i < logc; ++i)
            if(opuser[i][1] == 'w') ++mylogc;
        std::puts("*-----------------------------------------------------------------------------------------------------------------------------------------------*");
        std::cout << "#All Employees' " << mylogc << " Operations are listed as follows:\n";
        for (int i = 0; i < logc; ++i)
            if(opuser[i][1] == 'w') std::cout << '\t' << opuser[i]+", do ["+logstr[i] << "\n";
        std::puts("*-----------------------------------------------------------------------------------------------------------------------------------------------*");
    }
    else if (argc == 2 && argv[1] == "report" && argv[2] == "myself") {
        if (userManager.getAuthority() < 3) return -1;
        int mylogc = 0;
        std::string authoritystr;
        if (userManager.getAuthority() == 7) authoritystr = "<root(7)>";
        else if (userManager.getAuthority() == 3) authoritystr = "<worker(3)>";
        for (int i = 0; i < logc; ++i)
            if(opuser[i] == authoritystr+"["+userManager.getNowUser().name+"(id:"+userManager.getNowUser().id+")"+"]") ++mylogc;
        std::puts("*-----------------------------------------------------------------------------------------------------------------------------------------------*");
        std::cout << "#Your " << mylogc << " Operations are listed as follows:\n";
        for (int i = 0; i < logc; ++i)
            if(opuser[i] == authoritystr+"["+userManager.getNowUser().name+"(id:"+userManager.getNowUser().id+")"+"]")
                std::cout << '\t' << opuser[i]+", do ["+logstr[i] << "\n";
        std::puts("*-----------------------------------------------------------------------------------------------------------------------------------------------*");
    }
    else if (argc == 2 && argv[1] == "report" && argv[2] == "finance") {
        return dataOperation.report_finance(userManager.getAuthority());
    }
    else if (argc == 1 && argv[1] == "patchouli") {
        digitalWindow = true;
        puts(background);
        puts(purple blank "*帕秋莉打开成功！");
        return -3;
    }
    else if (argc > 0) {
        return -1;
    }
    return 0;
}
void Patchouli_su() {
    std::string inp;
    cmd = "su ";
    puts(blank "*输入您要登录的用户ID~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入用户密码，若想无密码登录到低权限用户，请输入\"next\"~");
    std::cin >> inp;
    if (inp != "next") cmd += inp;
}
void Patchouli_useradd() {
    std::string inp;
    cmd = "useradd ";
    puts(blank "*输入您要添加的用户ID~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入您要添加的用户密码~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入您要添加的用户权限等级~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入您要添加的用户名~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_reg() {
    std::string inp;
    cmd = "register ";
    puts(blank "*输入您要注册的用户ID~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入您要注册的用户密码~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入您要添加的用户名~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_del() {
    std::string inp;
    cmd = "delete ";
    puts(blank "*输入您要删除的用户ID~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_passwd(bool isRoot) {
    std::string inp;
    cmd = "passwd ";
    puts(blank "*输入您要修改的用户ID~");
    std::cin >> inp;
    cmd += inp + " ";
    if(!isRoot) {
        puts(blank "*输入该用户的旧密码~");
        std::cin >> inp;
        cmd += inp + " ";
    }
    else puts(blank "*root账户不需要旧密码~");
    puts(blank "*输入该用户的新密码~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_select() {
    std::string inp;
    cmd = "select ";
    puts(blank "*输入目标图书的ISBN号~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_modify() {
    std::string inp;
    int ope, cnt = 0;
    cmd = "modify ";
    while (true) {
        if (!cnt) {
            puts(blank "*想要修改什么信息?");
            puts(blank "0、不修改了");
        }
        else {
            puts(blank "*还想修改什么信息?");
            puts(blank "0、就这些");
        }
        puts(blank "1、ISBN");
        puts(blank "2、书名");
        puts(blank "3、作者");
        puts(blank "4、关键字");
        puts(blank "5、价格");
        std::cin >> ope;
        if (ope == 0) return;
        else if (ope == 1) {
            puts(blank "*输入ISBN");
            std::cin >> inp;
            cmd += "-ISBN="+inp+" ";
        }
        else if (ope == 2) {
            puts(blank "*输入书名");
            std::cin >> inp;
            cmd += "-name=\""+inp+"\" ";
        }
        else if (ope == 3) {
            puts(blank "*输入作者");
            std::cin >> inp;
            cmd += "-author=\""+inp+"\" ";
        }
        else if (ope == 4) {
            puts(blank "*输入关键字");
            std::cin >> inp;
            cmd += "-keyword=\""+inp+"\" ";
        }
        else if (ope == 5) {
            puts(blank "*输入价格");
            std::cin >> inp;
            cmd += "-price="+inp+" ";
        }
        ++cnt;
    }
}
void Patchouli_import() {
    std::string inp;
    cmd = "import ";
    puts(blank "*输入进货的数量~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入本次进货的总花费~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_show() {
    std::string inp;
    int ope, cnt = 0;
    cmd = "show ";
    while (true) {
        if (!cnt) {
            puts(blank "*想要按什么信息查找?");
            puts(blank "0、列出所有书目");
        }
        else {
            puts(blank "*还想要按什么信息查找?");
            puts(blank "0、就这些");
        }
        puts(blank "1、ISBN");
        puts(blank "2、书名");
        puts(blank "3、作者");
        puts(blank "4、关键字");
        std::cin >> ope;
        if (ope == 0) return;
        else if (ope == 1) {
            puts(blank "*输入ISBN");
            std::cin >> inp;
            cmd += "-ISBN="+inp+" ";
        }
        else if (ope == 2) {
            puts(blank "*输入书名");
            std::cin >> inp;
            cmd += "-name=\""+inp+"\" ";
        }
        else if (ope == 3) {
            puts(blank "*输入作者");
            std::cin >> inp;
            cmd += "-author=\""+inp+"\" ";
        }
        else if (ope == 4) {
            puts(blank "*输入关键字");
            std::cin >> inp;
            cmd += "-keyword=\""+inp+"\" ";
        }
        ++cnt;
    }
}
void Patchouli_buy() {
    std::string inp;
    cmd = "buy ";
    puts(blank "*输入购买图书的ISBN号~");
    std::cin >> inp;
    cmd += inp + " ";
    puts(blank "*输入想购买的数量~");
    std::cin >> inp;
    cmd += inp;
}
void Patchouli_quit() {
    cmd = "quit";
    puts(blank "*期待下次再会~");
    rmv
}
void Patchouli_change() {
    digitalWindow = false;
    puts(blank "*帕秋莉关闭成功，再会~");
    rmv
}

int main() {
    /* Init */
    LogInit();
    puts(Patchouli);
    puts(green blank "Patchouli beta 1.0");
    puts(blank "少女折寿中..");
    sleep(1); puts(blank "2..");
    sleep(1); puts(blank "1..");
    sleep(1);
    puts(purple blank "*您好，我是图书管理员帕秋莉，我将帮助您使用红魔馆图书管理系统~");
    puts(blank"*您只需要输入不同数字即可~");
    puts(blank"*不习惯的话，输入0来关闭帕秋莉（切换至命令行模式），在命令行输入\"patchouli\"打开帕秋莉~");

    /* Cmd */
    while (true) {
        if (!digitalWindow) {
            while (!std::getline(std::cin, cmd, '\n').eof()) {
                char str[MaxQL], str1[UserMaxl];
                std::string authoritystr;
                if (userManager.getAuthority() == 7) authoritystr = "<root(7)>";
                else if (userManager.getAuthority() == 3) authoritystr = "<worker(3)>";
                else if (userManager.getAuthority() == 1) authoritystr = "<visitor(1)>";
                else authoritystr = "<unlogged(0)>";
                if (userManager.getAuthority() > 0) strcpy(str1, (userManager.getNowUser().name+"(id:"+userManager.getNowUser().id+")").c_str());
                else strcpy(str1, "Unnamed");

                std::string ope = cmd;
                int sta = Command();
                if (sta == -1) puts("Invalid"), ope += "], [Invalid]";
                else if (sta >= 0) ope += "], [Success]";
                else if (sta == -2) {
                    return 0;
                }
                else if (sta == -3) break;

                strcpy(str, ope.c_str());
                logstr.push_back(ope);
                strcpy(str1, (authoritystr+"["+std::string(str1)+"]").c_str());
                opuser.push_back(std::string(str1));
                ++logc;
                logFile.open("log.bin", std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
                logFile.seekp(0, std::ios::beg);
                logFile.write(reinterpret_cast<char *>(&logc), sizeof(int));
                logFile.seekp(sizeof(int)+(logc-1)*(MaxQL+UserMaxl), std::ios::beg);
                logFile.write(reinterpret_cast<char *>(&str1), UserMaxl);
                logFile.write(reinterpret_cast<char *>(&str), MaxQL);
                logFile.close();
            }
        }
        else {
            while (true) {
                cmd.clear();
                if (userManager.getAuthority()) std::cout << '\n' << blue blank "当前用户："+userManager.getNowUser().name+", " << " 当前的账户状态：";
                else putchar('\n'), std::cout << blue blank << "当前的账户状态：未登录";
                if (userManager.getAuthority() == 7) {
                    int ope = -1;
                    std::cout << "root账户\n";
                    puts(purple blank "*以下是您可以执行的操作，输入对应数字来执行：");
                    puts(blue blank "1、登录到某用户");
                    puts(blank "2、登出");
                    puts(blank "3、添加用户");
                    puts(blank "4、注册用户");
                    puts(blank "5、删除用户");
                    puts(blank "6、修改密码");
                    puts(blank "7、选定图书");
                    puts(blank "8、修改图书信息");
                    puts(blank "9、进货");
                    puts(blank "10、查询图书");
                    puts(blank "11、查询财务");
                    puts(blank "12、购买图书");
                    puts(blank "13、查看财务报表");
                    puts(blank "14、查看员工工作情况");
                    puts(blank "15、查看自己工作情况");
                    puts(blank "16、查看日志");
                    puts(blank "17、退出");
                    puts(purple);
                    std::cin >> ope;
                    if (ope == 1) Patchouli_su();
                    else if (ope == 2) cmd = "logout";
                    else if (ope == 3) Patchouli_useradd();
                    else if (ope == 4) Patchouli_reg();
                    else if (ope == 5) Patchouli_del();
                    else if (ope == 6) Patchouli_passwd(true);
                    else if (ope == 7) Patchouli_select();
                    else if (ope == 8) Patchouli_modify();
                    else if (ope == 9) Patchouli_import();
                    else if (ope == 10) Patchouli_show();
                    else if (ope == 11) cmd = "show finance";
                    else if (ope == 12) Patchouli_buy();
                    else if (ope == 13) cmd = "report finance";
                    else if (ope == 14) cmd = "report employee";
                    else if (ope == 15) cmd = "report myself";
                    else if (ope == 16) cmd = "log";
                    else if (ope == 17) Patchouli_quit();
                    else if (ope == 0) {
                        Patchouli_change();
                        break;
                    }
                    else {
                        puts(blank "您的输入有误~");
                        continue;
                    }
                }
                else if (userManager.getAuthority() == 3) {
                    int ope = -1;
                    std::cout << "员工\n";
                    puts(purple blank "*以下是您可以执行的操作，输入对应数字来执行：");
                    puts(blue blank "1、登录到某用户");
                    puts(blank "2、登出");
                    puts(blank "3、添加用户");
                    puts(blank "4、注册用户");
                    puts(blank "5、修改密码");
                    puts(blank "6、选定图书");
                    puts(blank "7、修改图书信息");
                    puts(blank "8、进货");
                    puts(blank "9、查询图书");
                    puts(blank "10、购买图书");
                    puts(blank "11、查看自己工作情况");
                    puts(blank "12、退出");
                    puts(purple);
                    std::cin >> ope;
                    if (ope == 1) Patchouli_su();
                    else if (ope == 2) cmd = "logout";
                    else if (ope == 3) Patchouli_useradd();
                    else if (ope == 4) Patchouli_reg();
                    else if (ope == 5) Patchouli_passwd(false);
                    else if (ope == 6) Patchouli_select();
                    else if (ope == 7) Patchouli_modify();
                    else if (ope == 8) Patchouli_import();
                    else if (ope == 9) Patchouli_show();
                    else if (ope == 10) Patchouli_buy();
                    else if (ope == 11) cmd = "report myself";
                    else if (ope == 12) Patchouli_quit();
                    else if (ope == 0) {
                        Patchouli_change();
                        break;
                    }
                    else {
                        puts(blank "您的输入有误...");
                        continue;
                    }
                }
                else if (userManager.getAuthority() == 1) {
                    int ope = -1;
                    std::cout << "顾客\n";
                    puts(purple blank "*以下是您可以执行的操作，输入对应数字来执行：");
                    puts(blue blank "1、登录到某用户");
                    puts(blank "2、登出");
                    puts(blank "3、注册用户");
                    puts(blank "4、修改密码");
                    puts(blank "5、查询图书");
                    puts(blank "6、购买图书");
                    puts(blank "7、退出");
                    puts(purple);
                    std::cin >> ope;
                    if (ope == 1) Patchouli_su();
                    else if (ope == 2) cmd = "logout";
                    else if (ope == 3) Patchouli_reg();
                    else if (ope == 4) Patchouli_passwd(false);
                    else if (ope == 5) Patchouli_show();
                    else if (ope == 6) Patchouli_buy();
                    else if (ope == 7) Patchouli_quit();
                    else if (ope == 0) {
                        Patchouli_change();
                        break;
                    }
                    else {
                        puts("您的输入有误...");
                        continue;
                    }
                }
                else if (userManager.getAuthority() == 0) {
                    int ope = -1;
                    puts("\n" purple blank "*以下是您可以执行的操作，输入对应数字来执行：");
                    puts(blue blank "1、登录到某用户");
                    puts(blank "2、注册用户");
                    puts(blank "3、退出");
                    puts(purple);
                    std::cin >> ope;
                    if (ope == 1) Patchouli_su();
                    else if (ope == 2) Patchouli_reg();
                    else if (ope == 3) Patchouli_quit();
                    else if (ope == 0) {
                        Patchouli_change();
                        break;
                    }
                    else {
                        puts(blank "您的输入有误...");
                        continue;
                    }
                }
                char str[MaxQL], str1[UserMaxl];
                std::string authoritystr;
                if (userManager.getAuthority() == 7) authoritystr = "<root(7)>";
                else if (userManager.getAuthority() == 3) authoritystr = "<worker(3)>";
                else if (userManager.getAuthority() == 1) authoritystr = "<visitor(1)>";
                else authoritystr = "<unlogged(0)>";
                if (userManager.getAuthority() > 0) strcpy(str1, (userManager.getNowUser().name+"(id:"+userManager.getNowUser().id+")").c_str());
                else strcpy(str1, "Unnamed");
                std::string ope = cmd;

                int sta = Command();
                if (sta >= 0) puts(blank "*操作成功！"), ope += "], [Success]";
                else if (sta == -1) puts(blank "*操作失败了，可能是哪里搞错了..."), ope += "], [Invalid]";
                else if (sta == -2) {
                    return 0;
                }

                strcpy(str, ope.c_str());
                logstr.push_back(ope);
                strcpy(str1, (authoritystr+"["+std::string(str1)+"]").c_str());
                opuser.push_back(std::string(str1));
                ++logc;
                logFile.open("log.bin", std::ios::ate | std::ios::in | std::ios::out | std::ios::binary);
                logFile.seekp(0, std::ios::beg);
                logFile.write(reinterpret_cast<char *>(&logc), sizeof(int));
                logFile.seekp(sizeof(int)+(logc-1)*(MaxQL+UserMaxl), std::ios::beg);
                logFile.write(reinterpret_cast<char *>(&str1), UserMaxl);
                logFile.write(reinterpret_cast<char *>(&str), MaxQL);
                logFile.close();
            }
        }
    }
}