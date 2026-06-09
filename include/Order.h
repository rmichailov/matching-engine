#pragma once

enum class Side {
    Buy, 
    Sell
};

class Order {
    private:
        int id;
        Side side;
        double price;
        double quantity;
        int timestamp;
    
    public:
        Order(int id, Side side, double price, double quantity, int timestamp);
        int getId() const;
        Side getSide() const;
        double getPrice() const;
        double getQuantity() const;
        int getTime() const;      
};