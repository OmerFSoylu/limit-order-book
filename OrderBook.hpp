#pragma once
#include "types.hpp"
#include <map>
#include <list>
#include <unordered_map>
#include <iostream>

class OrderBook {
    private:
        std::map<Price, std::list<Order>, std::greater<Price>> bids;
        std::map<Price, std::list<Order>> asks;
        std::map<OrderId, std::list<Order>::iterator> ordersById; // TODO: convert back to unordered_map 

    public:
        void addOrder (Order order);
        void cancelOrder(OrderId id);
        bool crossesSpread(Order order);
        bool orderExists(OrderId id);
        void matchOrders(Order& order);

};
