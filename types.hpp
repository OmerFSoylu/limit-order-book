#pragma once

#include <cstdint>

enum class Side {
    Buy, 
    Sell
};


struct Price{
    std::uint32_t val;
    bool operator<(const Price& other) const {
        return val < other.val;
    }
    bool operator>(const Price& other) const {
        return val > other.val;
    }
    bool operator<=(const Price& other) const {
        return val <= other.val;
    }
    bool operator>=(const Price& other) const {
        return val >= other.val;
    }
    bool operator==(const Price& other) const {
        return val == other.val;
    }
    bool operator!=(const Price& other) const {
        return val != other.val;
    }
};

struct Quantity {
    std::int32_t val;
};

struct OrderId {
    std::int32_t val;
    bool operator==(const OrderId& other) const {
        return val == other.val;
    }
    bool operator!=(const OrderId& other) const {
        return val != other.val;
    }
    bool operator<(const OrderId& other) const {
        return val < other.val;
    }
    bool operator>(const OrderId& other) const {
        return val > other.val;
    }
};

struct Order {
  OrderId id;
  Price price;
  Quantity quantity;  
  Side side;
};