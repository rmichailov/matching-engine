#include "Trade.h"

Trade::Trade(int buyId, int sellId, double price, double quantity) : buyId(buyId), sellId(sellId), price(price), quantity(quantity) {

}

int Trade::getBuyId() const{
    return buyId;
}
int Trade::getSellId() const {
    return sellId;
}

double Trade::getPrice() const {
    return price;
}

double Trade::getQuantity() const {
    return quantity;
}