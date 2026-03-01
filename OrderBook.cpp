#include "OrderBook.hpp"

/**
 * @brief Adds a new order to the order book.
 *
 * Behavior:
 *  - If the order ID already exists, the order is ignored.
 *  - If the order does NOT cross the spread, it is inserted into the book
 *    at its price level (FIFO within that level).
 *  - If the order crosses the spread, matching is triggered immediately.
 *
 * Time priority is preserved within each price level via push_back().
 */
void OrderBook::addOrder(Order order) {
    // Prevent duplicate order IDs
    if (orderExists(order.id)) {
        std::cout << "The order " << order.id.val << " already exists." << std::endl;
        return;
    }

    // If the order does not immediately match against the opposite side,
    // insert it into the appropriate price level.
    if (!crossesSpread(order)) {
        if (order.side == Side::Buy) {
            bids[order.price].push_back(order);
            // Store iterator for O(1) cancellation
            ordersById[order.id] = std::prev(bids[order.price].end());
        }
        else {
            asks[order.price].push_back(order);
            ordersById[order.id] = std::prev(asks[order.price].end());
        }
        return;
    }

    // Otherwise, execute matching logic
    matchOrders(order);    
}


/**
 * @brief Cancels an existing order by ID.
 *
 * Behavior:
 *  - If the order does not exist, nothing happens.
 *  - Removes the order from its price level.
 *  - If the price level becomes empty, it is removed from the book.
 *  - Erases the order from the ID lookup map.
 *
 * Complexity:
 *  O(1) for lookup and erase (assuming map + list iterator).
 */
void OrderBook::cancelOrder(OrderId id) {
    if (!orderExists(id)) {
        std::cout << "The order " << id.val << " doesnt exist." << std::endl;
        return;
    }

    // Retrieve stored iterator
    auto it = ordersById.find(id);
    auto OrderIt = it->second;

    Price level = OrderIt->price;

    if (OrderIt->side == Side::Buy) {
        auto& OrdersAtLvl = bids.find(level)->second;
        OrdersAtLvl.erase(OrderIt);

        // Remove price level if empty
        if (OrdersAtLvl.empty()) {
            bids.erase(level);
        }
    }
    else {
        auto& OrdersAtLvl = asks.find(level)->second;
        OrdersAtLvl.erase(OrderIt);

        // Remove price level if empty
        if (OrdersAtLvl.empty()) {
            asks.erase(level);
        }
    }

    // Remove from ID lookup
    ordersById.erase(it);
}


/**
 * @brief Checks whether an order ID exists in the book.
 *
 * @return true if found, false otherwise.
 */
bool OrderBook::orderExists(OrderId id) {
    return ordersById.find(id) != ordersById.end();
}


/**
 * @brief Determines whether an incoming order crosses the spread.
 *
 * Buy order crosses if:
 *   order.price >= best ask
 *
 * Sell order crosses if:
 *   order.price <= best bid
 *
 * @return true if the order is marketable (should match immediately).
 */
bool OrderBook::crossesSpread(Order order) {
    if (order.side == Side::Buy) {
        return !asks.empty() && order.price.val >= asks.begin()->first.val;
    }
    else {
        return !bids.empty() && order.price.val <= bids.begin()->first.val;
    }
}


/**
 * @brief Matches an incoming order against the opposite side of the book.
 *
 * Matching rules:
 *  - Price-time priority.
 *  - Fully consumes resting orders before moving to next.
 *  - Partially fills resting orders if necessary.
 *  - Removes empty price levels.
 *
 * If the incoming order is not fully filled, the remaining quantity
 * is inserted into the book at its limit price.
 *
 * @param order Incoming order (modified if partially filled).
 */
void OrderBook::matchOrders(Order& order) {

    // Remaining quantity to match
    int32_t quantity = order.quantity.val;

    // BUY order matches against lowest ask prices first
    if (order.side == Side::Buy) {

        while (!asks.empty()) {

            // Stop if no longer crossing best ask
            if (!crossesSpread(order)) break;

            // Match within best price level (FIFO)
            while (!asks.begin()->second.empty()) {

                auto& restingOrder = asks.begin()->second.begin()->quantity.val;

                // Partial fill of resting order
                if (restingOrder > quantity) {
                    restingOrder -= quantity;
                    quantity = 0;
                    break;
                }
                // Fully consume resting order
                else {
                    quantity -= restingOrder;
                    ordersById.erase(asks.begin()->second.begin()->id);
                    asks.begin()->second.pop_front();
                }
            }

            if (quantity == 0) break;

            // Remove price level if empty
            if (asks.begin()->second.empty())
                asks.erase(asks.begin());
        }

        // If residual quantity remains, insert into book
        if (quantity) {
            order.quantity.val = quantity;
            bids[order.price].push_back(order);
            ordersById[order.id] = std::prev(bids[order.price].end());
        }
    }

    // SELL order matches against highest bid prices first
    else {

        while (!bids.empty()) {

            if (!crossesSpread(order)) break;

            while (!bids.begin()->second.empty()) {

                auto& restingOrder = bids.begin()->second.begin()->quantity.val;

                if (restingOrder > quantity) {
                    restingOrder -= quantity;
                    quantity = 0;
                    break;
                }
                else {
                    quantity -= restingOrder;
                    ordersById.erase(bids.begin()->second.begin()->id);
                    bids.begin()->second.pop_front();
                }
            }

            if (quantity == 0) break;

            if (bids.begin()->second.empty())
                bids.erase(bids.begin());
        }

        // Insert remaining quantity if not fully matched
        if (quantity) {
            order.quantity.val = quantity;
            asks[order.price].push_back(order);
            ordersById[order.id] = std::prev(asks[order.price].end());
        }
    }
}