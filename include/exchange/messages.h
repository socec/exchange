#pragma once

#include <exchange/types.h>

#include <sstream>
#include <string>

namespace exchange {

////////////////////////////////////////////////////////////////////////////////////////////////////

struct TradingMsg {

    struct AddOrder {
        AddOrder(Instrument instrument, Price price, Quantity qty, Side side)
            : instrument(instrument), price(price), qty(qty), side(side) {}
        Instrument instrument;
        Price price;
        Quantity qty;
        Side side;
    };

    struct DeleteOrder {
        DeleteOrder(Instrument instrument, OrderID id)
            : instrument(instrument), id(id) {}
        Instrument instrument;
        OrderID id;
    };

    struct ModifyOrder {
        ModifyOrder(Instrument instrument, OrderID id, Price price, Quantity qty)
            : instrument(instrument), id(id), price(price), qty(qty) {}
        Instrument instrument;
        OrderID id;
        Price price;
        Quantity qty;
    };

};

////////////////////////////////////////////////////////////////////////////////////////////////////

enum class MarketDataMsgType { Add, Delete, Modify, Trade };

struct MarketDataMsg {
    MarketDataMsg(MarketDataMsgType type, Instrument instrument, OrderID id, Price price,
                  Quantity qty, Side side)
        : type(type), instrument(instrument), id(id), price(price), qty(qty), side(side) {}
    MarketDataMsgType type;
    Instrument instrument;
    OrderID id;
    Price price;
    Quantity qty;
    Side side;

    const std::string str() const {
        std::ostringstream ss;
        std::string type_str = "XXX";
        switch (type) {
            case MarketDataMsgType::Add:
                type_str = "ADD";
                break;
            case MarketDataMsgType::Delete:
                type_str = "DEL";
                break;
            case MarketDataMsgType::Modify:
                type_str = "MOD";
                break;
            case MarketDataMsgType::Trade:
                type_str = "TRD";
                break;
        }
        ss << type_str << " " << instrument << " " << id << " " << side_str(side) << " "
           << qty << " @ " << price;
        return ss.str();
    }
};

////////////////////////////////////////////////////////////////////////////////////////////////////

}  // namespace exchange
