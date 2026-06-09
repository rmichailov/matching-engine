#pragma once

class Trade {
    private:
        int buyId;
        int sellId;
        double price;
        double quantity;
    
    public:
        Trade(int buyId, int sellId, double price, double quantity);
        int getBuyId();
        int getSellId();
        double getPrice();
        double getQuantity();
};