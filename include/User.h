//
// Created by HTY on 2025/12/9.
//

#ifndef NEEDANALYSIS_MD_USER_H
#define NEEDANALYSIS_MD_USER_H
#include<string>
class User {
public:
    int UserID;
    char Username[35];
    char Password[35];
    int Privilege;
    User();
    User(std::string User_name, std::string Password_,int privilege = 0, int User_ID = 0): UserID(User_ID), Privilege(privilege){
        int cnt = 0;
        while (User_name[cnt] != '\0') Username[cnt] = User_name[cnt],cnt++;
        cnt = 0;
        while (Password_[cnt] != '\0') Password[cnt] = Password_[cnt],cnt++;
    }
};
#endif //NEEDANALYSIS_MD_USER_H