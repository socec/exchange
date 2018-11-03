#pragma once

#include <exchange/types.h>

#include <chrono>

namespace exchange {

Timestamp get_timestamp() {
    auto timestamp = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration_cast<std::chrono::milliseconds>(timestamp).count();
}

}  // namespace exchange
