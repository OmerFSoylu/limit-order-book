#pragma once
#include "types.hpp"
#include <map>
#include <list>

class OrderBook {
    private:
        std::map<Price, std::list<Order>, std::greater<Price>> bids;
        std::map<Price, std::list<Order>> asks;
};

class MatchingEngine {
    private:
        OrderBook orderBook;

    public:
        void addOrder (OrderId id, Side side, Price price, Quantity quantity);
        void cancelOrder(OrderId id);

};