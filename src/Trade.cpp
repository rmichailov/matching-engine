#include "Trade.h"

Trade::Trade(int buyId, int sellId, double price, double quantity) : buyId(buyId), sellId(sellId), price(price), quantity(quantity) {

}

int Trade::getBuyId() {
    return buyId;
}
int Trade::getSellId() {
    return sellId;
}

double Trade::getPrice() {
    return price;
}

double Trade::getQuantity() {
    return quantity;
}