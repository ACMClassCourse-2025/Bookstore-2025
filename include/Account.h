//
// Created by HTY on 2025/12/9.
//
// 管理账户信息，实现文件存储和读写
#ifndef NEEDANALYSIS_MD_ACCOUNT_H
#define NEEDANALYSIS_MD_ACCOUNT_H
#include<string>
#include<stack>
#include "User.h"
class User;
class Book;
class Account {
public:
    void AddNewAccount(User user);
    void LogIn(User user);
    User GetUser(int id);
    bool LogOut();
    bool FindUser(int id);
    int GetLoginNumber() {
        return login_number;
    }
private:
    std::string filename = "account.txt";
    std::string index_filename = "index_account.txt";
    std::stack<User> LogInList;
    int login_number = 0;
};
#endif //NEEDANALYSIS_MD_ACCOUNT_H