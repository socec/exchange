#pragma once

#include <cstdint>
#include <string>

namespace exchange {

using Timestamp = uint64_t;
using Instrument = uint32_t;
using OrderID = Timestamp;
using Price = uint32_t;
using Quantity = uint32_t;

enum class Side { Buy, Sell };
std::string side_str(const exchange::Side& side) {
    return side == exchange::Side::Buy ? "B" : "S";
}

}  // namespace exchange
