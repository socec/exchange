#include <exchange/book.h>

#include <iostream>

#include <sstream>
#include <string>

namespace exchange {

////////////////////////////////////////////////////////////////////////////////////////////////////

const std::string Order::str() const {
    std::ostringstream ss;
    ss << id << " " << side_str(side) << " " << qty << " @ " << price;
    return ss.str();
}

////////////////////////////////////////////////////////////////////////////////////////////////////

int Book::add_order(Order& order) {
    Side passive_side = (order.side == Side::Buy) ? Side::Sell : Side::Buy;
    if (passive_side == Side::Buy) {
        try_match<BookSideBuy>(order, buy_side_);
    } else {
        try_match<BookSideSell>(order, sell_side_);
    }
    if (order.qty == 0) {
        return 0;
    }

    orders_.emplace(order.id, order);
    if (order.side == Side::Buy) {
        buy_side_[order.price].insert(order.id);
    } else {
        sell_side_[order.price].insert(order.id);
    }

    messages_.push_back(MarketDataMsg(MarketDataMsgType::Add, instrument_,
        order.id, order.price, order.qty, order.side));

    return 0;
}

int Book::delete_order(OrderID id) {
    auto order = orders_.at(id);

    messages_.push_back(MarketDataMsg(MarketDataMsgType::Delete, instrument_,
        id, order.price, order.qty, order.side));

    if (order.side == Side::Buy) {
        buy_side_[order.price].erase(id);
        if (buy_side_[order.price].empty()) {
            buy_side_.erase(order.price);
        }
    } else {
        sell_side_[order.price].erase(id);
        if (sell_side_[order.price].empty()) {
            sell_side_.erase(order.price);
        }
    }
    orders_.erase(id);
    return 0;
}

MarketDataMsg Book::pop_message() {
    MarketDataMsg msg = messages_.front();
    messages_.pop_front();
    return msg;
}

void Book::show() const {
    std::cout << "book:\n";
    std::cout << "========\n";
    for (auto level = sell_side_.crbegin(); level != sell_side_.crend(); ++level) {
        std::ostringstream ss;
        Quantity total = 0;
        for (auto& oid : level->second) {
            total += orders_.at(oid).qty;
            ss << orders_.at(oid).str() << " | ";
        }
        std::cout << total << " @ " << level->first << " : " << ss.str() << std::endl;
    }
    std::cout << "--------\n";
    for (auto level = buy_side_.cbegin(); level != buy_side_.cend(); ++level) {
        std::ostringstream ss;
        Quantity total = 0;
        for (auto& oid : level->second) {
            total += orders_.at(oid).qty;
            ss << orders_.at(oid).str() << " | ";
        }
        std::cout << total << " @ " << level->first << " : " << ss.str() << std::endl;
    }
    std::cout << "========\n";
    std::cout << "\n";
}

void Book::show_orders() const {
    std::cout << "orders:\n";
    for (auto& o : orders_) {
        std::cout << o.second.str() << "\n";
    }
    std::cout << "\n";
}

////////////////////////////////////////////////////////////////////////////////////////////////////

} // namespace exchange
