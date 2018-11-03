#pragma once

#include <exchange/book.h>
#include <exchange/common.h>
#include <exchange/messages.h>
#include <exchange/types.h>

#include <iostream>

namespace exchange {

class Exchange {
public:
    Exchange() {}

    int add_order(const TradingMsg::AddOrder& msg) {
        Order order(generate_id(), get_timestamp(), msg.instrument, msg.price, msg.qty, msg.side);
        book_.add_order(order);
        while (book_.has_messages()) {
            std::cout << book_.pop_message().str() << "\n";
        }
        return 0;
    }

    int delete_order(const TradingMsg::DeleteOrder& msg) {
        if (book_.order_exists(msg.id)) {
            book_.delete_order(msg.id);
            while (book_.has_messages()) {
                std::cout << book_.pop_message().str() << "\n";
            }
            return 0;
        } else {
            return -1;
        }
    }

    // int modify_order();

    void show_book() const { book_.show(); }

private:
    Book book_{1};
    OrderID last_id = 1000;

    OrderID generate_id() { return ++last_id; }
};

}  // namespace exchange
