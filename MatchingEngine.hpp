#pragma once
#include "types.hpp"
#include <map>

class OrderBook {
    private:
        std::map<Price, Order> bids;
        std::map<Price, Order> asks;
};

class MatchingEngine {
    private:
        OrderBook orderBook;

    public:
        void addOrder (OrderId id, Side side, Price price, Quantity quantity);
        void cancelOrder(OrderId id);

};