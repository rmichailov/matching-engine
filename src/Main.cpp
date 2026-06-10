#include "OrderBook.h"
#include "Order.h"
int main() {
    OrderBook book;

    Order s1(1, Side::Sell, 101.00, 10, 1);
    Order s2(2, Side::Sell, 100.50, 5, 2);
    Order s3(3, Side::Sell, 101.00, 20, 3);

    book.addOrder(&s1);
    book.addOrder(&s2);
    book.addOrder(&s3);

    Order b1(1, Side::Buy, 100.0, 10, 1);
    Order b2(2, Side::Buy, 101.0, 20, 2);
    Order b3(3, Side::Buy, 100.0, 30, 3);
    Order b4(4, Side::Buy, 102.0, 40, 4);
    Order b5(5, Side::Buy, 101.0, 50, 5);

    book.addOrder(&b1);
    book.addOrder(&b2);
    book.addOrder(&b3);
    book.addOrder(&b4);
    book.addOrder(&b5);

    book.printBids();    
    // book.printAsks();

    return 0;
}