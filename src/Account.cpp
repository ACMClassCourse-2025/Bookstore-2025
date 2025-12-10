//
// Created by HTY on 2025/12/9.
//
#include "../include/Account.h"
#include "../include/DataMap.h"
#include "../include/User.h"

Database<int,User,1000> db;
void Account::AddNewAccount(User user) {
    db.Insert(user.UserID,user);
}
void Account::LogIn(User user) {
    LogInList.push(user);
}
User Account::GetUser(int id) {
    return db.GetValue(id);
}
bool Account::LogOut() {
    if (LogInList.empty()) return false;
    else LogInList.pop();
}
bool Account::FindUser(int id) {
    User user = db.GetValue(id);
    if (user.UserID == -1) return false;
    else return true;
}