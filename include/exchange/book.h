#pragma once

#include <exchange/common.h>
#include <exchange/messages.h>
#include <exchange/types.h>

#include <deque>
#include <functional>
#include <map>
#include <set>
#include <unordered_map>

namespace exchange {

////////////////////////////////////////////////////////////////////////////////////////////////////

struct Order {
    Order(OrderID id, Timestamp priority, Instrument instrument, Price price, Quantity qty,
          Side side)
        : id(id), priority(priority), instrument(instrument), price(price), qty(qty), side(side) {}
    OrderID id;
    Timestamp priority;
    Instrument instrument;
    Price price;
    Quantity qty;
    Side side;

    const std::string str() const;
};

////////////////////////////////////////////////////////////////////////////////////////////////////

class Book {
public:
    Book(Instrument instrument) : instrument_(instrument) {}

    const Instrument get_instrument() const { return instrument_; }
    bool order_exists(OrderID id) const { return orders_.find(id) != orders_.end(); }

    int add_order(Order& order);
    int delete_order(OrderID id);
    // int modify_order();

    bool has_messages() { return not messages_.empty(); }
    MarketDataMsg pop_message();

    void show() const;
    void show_orders() const;

private:
    Instrument instrument_;
    std::deque<MarketDataMsg> messages_;

    // Orders are kept in a hash table based on order ID. This allows O(1) access for a known ID.
    std::unordered_map<OrderID, Order> orders_;

    // Book side is a map of prices to price levels. Price level is a set of order IDs.
    // This structure keeps the time-price priority of orders because:
    // - Price priority is ensured by std::map keys being sorted
    // - Time priority is ensured by std::set elements being sorted (order IDs show time priority)
    using PriceLevel = std::set<OrderID>;
    using BookSideBuy = std::map<Price, PriceLevel, std::greater<Price>>;
    using BookSideSell = std::map<Price, PriceLevel, std::less<Price>>;
    BookSideBuy buy_side_{};
    BookSideSell sell_side_{};

    template <typename T> void try_match(Order& order, T book_side) {
        Price worst_price = 0;
        Quantity order_full_qty = order.qty;

        for (auto& level : book_side) {
            // Stop if order qty is filled or we reached a price level deeper than the order price
            if (order.qty == 0) break;
            if (book_side.key_comp()(order.price, level.first)) break;

            worst_price = level.first;
            for (auto& oid : level.second) {
                if (order.qty >= orders_.at(oid).qty) {
                    order.qty -= orders_.at(oid).qty;
                    delete_order(oid);
                } else {
                    orders_.at(oid).qty -= order.qty;
                    order.qty = 0;

                    messages_.push_back(MarketDataMsg(MarketDataMsgType::Modify, instrument_,
                        oid,level.first, orders_.at(oid).qty, orders_.at(oid).side));

                    break;
                }
            }
        }

        if (worst_price != 0) {
            messages_.push_front(MarketDataMsg(MarketDataMsgType::Trade, instrument_,
                order.id, worst_price, order_full_qty - order.qty, order.side));
        }

    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace exchange
