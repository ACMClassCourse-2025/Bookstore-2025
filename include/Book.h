//
// Created by HTY on 2025/12/10.
//

#ifndef BOOKSTORE_2025_BOOK_H
#define BOOKSTORE_2025_BOOK_H
class Book {
public:
    char ISBN[25];
    char BookName[65],Author[65],Keyword[65];
    int Quantity;
    float Price;// 保留小数点后两位
    Book();

};
#endif //BOOKSTORE_2025_BOOK_H