
#pragma once

#include <stockfighter/types.hpp>

#include <string>
#include <vector>

namespace stockfighter {
namespace api {
    // API calls not requiring an API key
    bool heartbeat();

    bool venue_heartbeat(const std::string& venue);

    std::vector<stock> get_stocks(const std::string& venue);

    orderbook get_orderbook(const std::string& venue,
                                   const std::string& stock);

    quote get_quote(const std::string& venue, const std::string& stock);

    order_status place_order(const std::string& api_key,
                             const std::string& account,
                             const std::string& venue,
                             const std::string& stock,
                             int price, int quantity,
                             direction dir,
                             order_type type);

    order_status cancel_order(const std::string& api_key,
                              const std::string& venue,
                              const std::string& stock,
                              int order_id);

} // end namespace api
} // end namespace stockfighter

