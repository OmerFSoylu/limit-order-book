#pragma once

#include <cstdint>

enum class Side {
    Buy, 
    Sell
};


struct Price{
    std::uint32_t val;
};

struct Quantity {
    std::int32_t val;
};

struct OrderId {
    std::int32_t val;
};

struct Order {
  OrderId id;
  Price price;
  Quantity quantity;  
  Side side;
};