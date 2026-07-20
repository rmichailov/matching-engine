#include "OrderBook.h"

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>

bool approximatelyEqual(double a, double b, double epsilon = 1e-9)
{
    return std::abs(a - b) < epsilon;
}

void testEmptyBook()
{
    OrderBook book;

    assert(book.getBestBid() == -1);
    assert(book.getBestAsk() == -1);
    assert(book.getOrderById(1) == nullptr);

    std::cout << "testEmptyBook passed\n";
}

void testAddBuyOrder()
{
    OrderBook book;
    Order buy(1, Side::Buy, 100.00, 10.0, 1);

    book.addOrder(&buy);

    assert(approximatelyEqual(book.getBestBid(), 100.00));
    assert(book.getBestAsk() == -1);
    assert(book.getOrderById(1) == &buy);

    std::cout << "testAddBuyOrder passed\n";
}

void testAddSellOrder()
{
    OrderBook book;
    Order sell(1, Side::Sell, 101.00, 10.0, 1);

    book.addOrder(&sell);

    assert(approximatelyEqual(book.getBestAsk(), 101.00));
    assert(book.getBestBid() == -1);
    assert(book.getOrderById(1) == &sell);

    std::cout << "testAddSellOrder passed\n";
}

void testBestBidAndBestAsk()
{
    OrderBook book;

    Order buy1(1, Side::Buy, 99.00, 10.0, 1);
    Order buy2(2, Side::Buy, 101.00, 10.0, 2);
    Order buy3(3, Side::Buy, 100.00, 10.0, 3);

    Order sell1(4, Side::Sell, 105.00, 10.0, 4);
    Order sell2(5, Side::Sell, 103.00, 10.0, 5);
    Order sell3(6, Side::Sell, 104.00, 10.0, 6);

    book.addOrder(&buy1);
    book.addOrder(&buy2);
    book.addOrder(&buy3);

    book.addOrder(&sell1);
    book.addOrder(&sell2);
    book.addOrder(&sell3);

    assert(approximatelyEqual(book.getBestBid(), 101.00));
    assert(approximatelyEqual(book.getBestAsk(), 103.00));

    std::cout << "testBestBidAndBestAsk passed\n";
}

void testCancelExistingBuyOrder()
{
    OrderBook book;

    Order buy1(1, Side::Buy, 100.00, 10.0, 1);
    Order buy2(2, Side::Buy, 99.00, 5.0, 2);

    book.addOrder(&buy1);
    book.addOrder(&buy2);

    bool cancelled = book.cancelOrder(1);

    assert(cancelled);
    assert(book.getOrderById(1) == nullptr);
    assert(approximatelyEqual(book.getBestBid(), 99.00));

    std::cout << "testCancelExistingBuyOrder passed\n";
}

void testCancelExistingSellOrder()
{
    OrderBook book;

    Order sell1(1, Side::Sell, 101.00, 10.0, 1);
    Order sell2(2, Side::Sell, 102.00, 5.0, 2);

    book.addOrder(&sell1);
    book.addOrder(&sell2);

    bool cancelled = book.cancelOrder(1);

    assert(cancelled);
    assert(book.getOrderById(1) == nullptr);
    assert(approximatelyEqual(book.getBestAsk(), 102.00));

    std::cout << "testCancelExistingSellOrder passed\n";
}

void testCancelNonexistentOrder()
{
    OrderBook book;

    assert(!book.cancelOrder(999));

    std::cout << "testCancelNonexistentOrder passed\n";
}

void testCancelOneOrderAtSharedPrice()
{
    OrderBook book;

    Order buy1(1, Side::Buy, 100.00, 10.0, 1);
    Order buy2(2, Side::Buy, 100.00, 20.0, 2);

    book.addOrder(&buy1);
    book.addOrder(&buy2);

    assert(book.cancelOrder(1));

    assert(book.getOrderById(1) == nullptr);
    assert(book.getOrderById(2) == &buy2);
    assert(approximatelyEqual(book.getBestBid(), 100.00));

    std::cout << "testCancelOneOrderAtSharedPrice passed\n";
}

void testNoMatch()
{
    OrderBook book;

    Order restingSell(1, Side::Sell, 105.00, 10.0, 1);
    book.addOrder(&restingSell);

    Order incomingBuy(2, Side::Buy, 100.00, 5.0, 2);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.empty());
    assert(approximatelyEqual(incomingBuy.getQuantity(), 5.0));
    assert(book.getOrderById(2) == &incomingBuy);

    assert(approximatelyEqual(book.getBestBid(), 100.00));
    assert(approximatelyEqual(book.getBestAsk(), 105.00));

    std::cout << "testNoMatch passed\n";
}

void testExactMatch()
{
    OrderBook book;

    Order restingSell(1, Side::Sell, 100.00, 10.0, 1);
    book.addOrder(&restingSell);

    Order incomingBuy(2, Side::Buy, 100.00, 10.0, 2);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.size() == 1);

    assert(trades[0].getBuyId() == 2);
    assert(trades[0].getSellId() == 1);
    assert(approximatelyEqual(trades[0].getPrice(), 100.00));
    assert(approximatelyEqual(trades[0].getQuantity(), 10.0));

    assert(approximatelyEqual(incomingBuy.getQuantity(), 0.0));
    assert(approximatelyEqual(restingSell.getQuantity(), 0.0));

    assert(book.getOrderById(1) == nullptr);
    assert(book.getOrderById(2) == nullptr);

    assert(book.getBestBid() == -1);
    assert(book.getBestAsk() == -1);

    std::cout << "testExactMatch passed\n";
}

void testIncomingBuyPartiallyFilled()
{
    OrderBook book;

    Order restingSell(1, Side::Sell, 100.00, 5.0, 1);
    book.addOrder(&restingSell);

    Order incomingBuy(2, Side::Buy, 100.00, 10.0, 2);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.size() == 1);
    assert(approximatelyEqual(trades[0].getQuantity(), 5.0));

    assert(approximatelyEqual(restingSell.getQuantity(), 0.0));
    assert(approximatelyEqual(incomingBuy.getQuantity(), 5.0));

    assert(book.getOrderById(1) == nullptr);
    assert(book.getOrderById(2) == &incomingBuy);

    assert(approximatelyEqual(book.getBestBid(), 100.00));
    assert(book.getBestAsk() == -1);

    std::cout << "testIncomingBuyPartiallyFilled passed\n";
}

void testRestingSellPartiallyFilled()
{
    OrderBook book;

    Order restingSell(1, Side::Sell, 100.00, 10.0, 1);
    book.addOrder(&restingSell);

    Order incomingBuy(2, Side::Buy, 101.00, 4.0, 2);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.size() == 1);
    assert(approximatelyEqual(trades[0].getQuantity(), 4.0));

    assert(approximatelyEqual(incomingBuy.getQuantity(), 0.0));
    assert(approximatelyEqual(restingSell.getQuantity(), 6.0));

    assert(book.getOrderById(1) == &restingSell);
    assert(book.getOrderById(2) == nullptr);

    assert(approximatelyEqual(book.getBestAsk(), 100.00));

    std::cout << "testRestingSellPartiallyFilled passed\n";
}

void testIncomingSellMatch()
{
    OrderBook book;

    Order restingBuy(1, Side::Buy, 100.00, 10.0, 1);
    book.addOrder(&restingBuy);

    Order incomingSell(2, Side::Sell, 99.00, 10.0, 2);
    std::vector<Trade> trades = book.matchOrder(&incomingSell);

    assert(trades.size() == 1);

    assert(trades[0].getBuyId() == 1);
    assert(trades[0].getSellId() == 2);
    assert(approximatelyEqual(trades[0].getPrice(), 100.00));
    assert(approximatelyEqual(trades[0].getQuantity(), 10.0));

    assert(book.getBestBid() == -1);
    assert(book.getBestAsk() == -1);

    std::cout << "testIncomingSellMatch passed\n";
}

void testMultiplePriceLevelMatches()
{
    OrderBook book;

    Order sell1(1, Side::Sell, 100.00, 4.0, 1);
    Order sell2(2, Side::Sell, 101.00, 6.0, 2);
    Order sell3(3, Side::Sell, 102.00, 8.0, 3);

    book.addOrder(&sell1);
    book.addOrder(&sell2);
    book.addOrder(&sell3);

    Order incomingBuy(4, Side::Buy, 101.00, 10.0, 4);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.size() == 2);

    assert(trades[0].getSellId() == 1);
    assert(approximatelyEqual(trades[0].getPrice(), 100.00));
    assert(approximatelyEqual(trades[0].getQuantity(), 4.0));

    assert(trades[1].getSellId() == 2);
    assert(approximatelyEqual(trades[1].getPrice(), 101.00));
    assert(approximatelyEqual(trades[1].getQuantity(), 6.0));

    assert(approximatelyEqual(incomingBuy.getQuantity(), 0.0));
    assert(book.getOrderById(1) == nullptr);
    assert(book.getOrderById(2) == nullptr);
    assert(book.getOrderById(3) == &sell3);

    assert(approximatelyEqual(book.getBestAsk(), 102.00));

    std::cout << "testMultiplePriceLevelMatches passed\n";
}

void testFifoAtSamePrice()
{
    OrderBook book;

    Order sell1(1, Side::Sell, 100.00, 5.0, 1);
    Order sell2(2, Side::Sell, 100.00, 5.0, 2);

    book.addOrder(&sell1);
    book.addOrder(&sell2);

    Order incomingBuy(3, Side::Buy, 100.00, 7.0, 3);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.size() == 2);

    // The earlier order at the same price must execute first.
    assert(trades[0].getSellId() == 1);
    assert(approximatelyEqual(trades[0].getQuantity(), 5.0));

    assert(trades[1].getSellId() == 2);
    assert(approximatelyEqual(trades[1].getQuantity(), 2.0));

    assert(book.getOrderById(1) == nullptr);
    assert(book.getOrderById(2) == &sell2);

    assert(approximatelyEqual(sell2.getQuantity(), 3.0));
    assert(approximatelyEqual(book.getBestAsk(), 100.00));

    std::cout << "testFifoAtSamePrice passed\n";
}

void testExecutionUsesRestingPrice()
{
    OrderBook book;

    Order restingSell(1, Side::Sell, 100.00, 5.0, 1);
    book.addOrder(&restingSell);

    Order incomingBuy(2, Side::Buy, 110.00, 5.0, 2);
    std::vector<Trade> trades = book.matchOrder(&incomingBuy);

    assert(trades.size() == 1);

    // The incoming buyer was willing to pay 110,
    // but the resting order's price is used.
    assert(approximatelyEqual(trades[0].getPrice(), 100.00));

    std::cout << "testExecutionUsesRestingPrice passed\n";
}

void testPrintMethods()
{
    OrderBook book;

    Order buy(1, Side::Buy, 99.00, 5.0, 1);
    Order sell(2, Side::Sell, 101.00, 7.0, 2);

    book.addOrder(&buy);
    book.addOrder(&sell);

    // These are mostly visual tests unless std::cout is redirected.
    book.printBids();
    book.printAsks();
    book.printBook();

    std::cout << "testPrintMethods completed; inspect output formatting\n";
}

int main()
{
    testEmptyBook();
    testAddBuyOrder();
    testAddSellOrder();
    testBestBidAndBestAsk();

    testCancelExistingBuyOrder();
    testCancelExistingSellOrder();
    testCancelNonexistentOrder();
    testCancelOneOrderAtSharedPrice();

    testNoMatch();
    testExactMatch();
    testIncomingBuyPartiallyFilled();
    testRestingSellPartiallyFilled();
    testIncomingSellMatch();
    testMultiplePriceLevelMatches();
    testFifoAtSamePrice();
    testExecutionUsesRestingPrice();

    testPrintMethods();

    std::cout << "\nAll OrderBook tests passed.\n";
    return 0;
}