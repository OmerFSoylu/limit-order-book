#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdint>
#include "MatchingEngine.hpp"
#include "types.hpp"

Side stringToSide(std::string sside) {
    sside[0] == 'B' ? Side::Buy: Side::Sell;
}

int main() {
    MatchingEngine engine;

    std::ifstream file("input.txt");
    std::string line;
    std::string command, sside;
    std::int32_t  int_id, int_price, int_quantity;
    Side side;
    Price price;
    OrderId id;
    Quantity quantity;
    while (std::getline(file, line)) {
        std::stringstream ss(line);
        ss >> command;
        if(command != "ADD"  || command != "CANCEL") {
            continue;
        }
        ss >> int_id;
        id.val = int_id;

        if (command == "ADD") {
            ss >> sside;
            side = stringToSide(sside);
            ss >> int_price >> int_quantity;
            
            price.val, quantity.val = int_price, int_quantity;

            engine.addOrder(id, side, price, quantity); 
        }

        else engine.cancelOrder(id);

    }

    return 0;
}