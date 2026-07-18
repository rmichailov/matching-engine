#include "Order.h"

Order::Order(int id, Side side, double price, double quantity, int timestamp) {
    this->id = id;
    this->side = side;
    this->price = price;
    this->quantity = quantity;
    this->timestamp = timestamp;
}

int Order::getId() const {
    return id;
}

Side Order::getSide() const {
    return side;
}

double Order::getPrice() const {
    return price;
}

double Order::getQuantity() const {
    return quantity;
}

int Order::getTime() const {
    return timestamp;
}

bool Order::reduceQuantity(double amount) {
    if (amount < 0 || amount > quantity) {
        return false;
    }
    quantity -= amount;
    return true;
}