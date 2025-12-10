//
// Created by HTY on 2025/12/9.
//
#include<iostream>
#include "../include/Account.h"
#include "../include/User.h"
#include<string>

int main() {
    User cur_user("root","sjtu",7);
    Account account;
    account.AddNewAccount(cur_user);
    std::string op;
    while (std::cin >> op) {
        // 账户系统
        bool valid = true;
        if (op == "su") {
            char UserID[35];
            char Password[35];
            std::cin >> UserID;
            User new_user = account.GetUser(UserID);
            if (new_user.UserID == -1) valid = false;
            if (cur_user.Privilege <= new_user.Privilege) {
                std::cin >> Password;
                if (new_user.Password != Password) valid = false;
            }
            if (valid) {
                account.LogIn(new_user);
                cur_user = new_user;
            }
        }
        if (op == "logout") {
            if (!account.LogOut()) valid = false;
        }
        if (op == "register") {
            int UserID;
            char Password[35];
            char Username[35];
            std::cin >> UserID >> Password >> Username;
            if (account.FindUser(UserID)) valid = false;
            User new_user(Username, Password,1,UserID);
            account.AddNewAccount(new_user);
        }
        if (op == "passwd") {
            int UserID;
            char CurrentPassword[35];
            char NewPassword[35];
        }
        if (op == "useradd") {

        }
        if (op == "delete") {

        }
        //图书系统
    }
    return 0;
}