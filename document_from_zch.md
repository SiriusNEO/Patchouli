#                     图书管理系统开发文档



## 一、数据存储方式

一个基于二进制文件输入输出的块状链表

### 哈希表类

内部使用哈希的方式存储书籍类中的`isbn` `name`  `keyword` `author`

## 二、各个类



### 1、用户管理类

内含变量：` name`  `id` `passwd` ` authority` （每个用户都有一个操作类）

`name`、 `id`、`passwd`:均包含连续无空格的数字字母下划线，不超过30个字符，使用`string`存储

` authority`：用户权限等级，使用`int`存储，共分四个等级，老板7>员工3>顾客1>未登录0



内含函数：`su` `logout` `useradd`  `register`  `delete`  `modify_passwd`

`su`:登录函数，参数为`id`  `passwd` ，查找该用户是否存在，判断密码是否正确，若通过则将其`push`入栈，并作为当前用户

`logout`:登出函数，参数为` authority`，只有处在已登录状态时，从栈中`pop`出本层，并取出上一层作为当前用户

`useradd`:增加用户的函数，参数为`id` `passwd` ` authority` `name` ，判断当前用户权限是否可执行本次操作（即用户权限是否大于参数权限），若可执行，则按照参数创建一个新用户

`register`:参数为`id` `passwd` `name`，创建一个带有这些参数的权限为顾客的用户，无权限要求

`delete`:参数为`id` ` authority` ，只有老板可删除用户，同时在该id存在情况下删除该用户

`modify_passwd`:参数为`id` `passwd` `newpasswd` ` authority`，只有登录状态可调用，root不需要填写旧密码，其余账户需要

​	注：在程序初次启动时，创建`root`用户，之后将权限等级为0的未登录状态作为首层压入栈。

root用户名称为root，默认密码为sjtu



### 2、数据操作类

内含变量：`tot` `bill[]` 

`tot`:`int`类型变量，记录进货、卖出次数

`bill[]`：`double`类型，记录每次操作的资金流动情况，正值代表收入，负值代表支出

~~`income[]` ：`double`类型，记录每次操作后的收入~~

~~`cost[]`:`double`类型，记录每次操作后的支出~~





#### 1、书籍类

内含变量：` name`  `ISBN` `author` ` keyword` `price` `quantity`

`ISBN` 为不超过20位的连续字符串，使用`string`存储

 ` name`和`authoe`内容不超过60个英文字符，使用`string`存储

`keyword`包含多个`string`类型字符串，每个关键字本身内容无引号以及空格内容，总共不超过60个英文字符

`price` 为`double` 类型变量

`quantity` 为`int`类型变量

内含构造函数，方便创建新的书籍类。





#### 2、操作类

内含变量：~~`ISBN`及~~一个指向所选书籍类的指针（int）

~~ISBN为一个`string`类型字符串，存储当前选中的图书的ISBN码~~

内含函数：`select` `modify`  `import`  `show `  `show_finance`  `buy`  `report_finance`  `report_employee` `log`  `report_myself`



`select`: [ISBN] `#3`:调用哈希表类查找选定ISBN为指定值的图书，若不存在则调用书籍类构造函数创建该ISBN的书并将其余信息留空等待modify进行填写

`modify` :-ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] -price=[price] `#3`:根据输入参数替换选中的书的信息，并调用相应的哈希表类更新

`import`：[quantity] [cost_price(in total)] `#3`:修改选中的书籍类的相应变量，并修改`tot` `income` `cost`

`show` :-ISBN=[ISBN] -name=[name] -author=[author] -keyword=[keyword] `#1`:根据输入参数调用哈希表类查找匹配图书，将匹配的图书以ISBN号排序输出其所有数据，输出格式为:每行一本，所有信息项目间以`\t`隔开，价格保留两位小数。如果没有符合条件的书，输出一个空行`\n`

`show_finance`: [time] `#7`:time项省略时，输出总的收入与支出；否则输出近[time]次进货、卖出操作(分别算一次)的收入支出，格式为`+ 收入 - 支出\n`。

`buy` :[ISBN] [quantity] `#1`:修改相应的书籍类的相应变量，并修改`tot` `income` `cost`



`report_finance` `#7`:生成财务报表，格式自定

`report_employee` `#7`:生成员工工作情况表，记录其操作，格式自定

`log` :`#7`:返回日志记录，包括系统操作类的谁干了什么，以及财务上每一笔交易情况，格式自定 

`report_myself` `#3`:返回员工自己的操作记录，格式自定

注：上述所有函数均含参数` authority`,用于判断执行该操作是否合法





## main函数

进行初始化

依次读入操作并调用类执行

退出条件为：读到`EOF`或读到`quit`。

# 注：一切不合法操作均需输出`Invalid`