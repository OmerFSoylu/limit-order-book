#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include "OrderBook.hpp"
#include "types.hpp"

Side stringToSide(std::string sside) {
    return (sside[0] == 'B') ? Side::Buy: Side::Sell;
}

int main() {
    OrderBook orderBook;

    std::ifstream file("input.txt");
    std::string line;
    std::string command, sside;
    std::int32_t  id, price, quantity;
    Order order;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> command;
        if(command != "ADD"  || command != "CANCEL") {
            continue;
        }
        ss >> id;
        order.id.val = id;

        if (command == "ADD") {
            ss >> sside;
            if (sside != "BUY" || sside != "SELL") {
                continue;
            }
            order.side = stringToSide(sside);
            ss >> price >> quantity;
            
            order.price.val = price;
            order.quantity.val = quantity;
            
            if (price <= 0 || quantity <= 0) {
                continue;
            }

            orderBook.addOrder(order); 
        }

        else orderBook.cancelOrder(order.id);

    }

    return 0;
}