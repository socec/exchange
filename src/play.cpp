#include <exchange/common.h>
#include <exchange/exchange.h>
#include <exchange/messages.h>

#include <iostream>

void print(auto x) {
    std::cout << x  << '\n';
}

using namespace exchange;

int main(int argc, char** argv) {
    Exchange e;
    e.add_order(TradingMsg::AddOrder(1, 100, 10, Side::Buy));
    e.add_order(TradingMsg::AddOrder(1, 101, 20, Side::Buy));
    e.add_order(TradingMsg::AddOrder(1, 200, 10, Side::Sell));
    e.add_order(TradingMsg::AddOrder(1, 201, 20, Side::Sell));
    e.show_book();

    e.add_order(TradingMsg::AddOrder(1, 101, 20, Side::Buy));
    e.add_order(TradingMsg::AddOrder(1, 200, 20, Side::Sell));
    e.show_book();

    e.delete_order(TradingMsg::DeleteOrder(1, 1001));
    e.show_book();

    e.add_order(TradingMsg::AddOrder(1, 101, 30, Side::Sell));
    e.show_book();

    e.add_order(TradingMsg::AddOrder(1, 300, 60, Side::Buy));
    e.show_book();
}
