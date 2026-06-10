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

    book.printAsks();

    return 0;
}