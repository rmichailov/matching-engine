#pragma once

class Trade {
    private:
        int buyId;
        int sellId;
        double price;
        double quantity;
    
    public:
        Trade(int buyId, int sellId, double price, double quantity);
        int getBuyId() const;
        int getSellId() const;
        double getPrice() const;
        double getQuantity() const;
};