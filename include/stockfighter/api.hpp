
#pragma once

#include <stockfighter/types.hpp>

#include <string>
#include <vector>

namespace stockfighter {

class api {
public:
    // API calls not requiring an API key
    static bool heartbeat();

    static bool venue_heartbeat(const std::string& venue);

    static std::vector<stock> get_stocks(const std::string& venue);

    static orderbook get_orderbook(const std::string& venue,
                                   const std::string& stock);

    static quote get_quote(const std::string& venue, const std::string& stock);

    // Calls which require an API key
    api(std::string key) : key_{std::move(key)} {}

    order_status place_order(const std::string& account,
                             const std::string& venue,
                             const std::string& stock,
                             int price, int quantity,
                             direction dir,
                             order_type type) const;

    order_status cancel_order(const std::string& venue,
                              const std::string& stock,
                              int order_id) const;

private:
    std::string key_;
};


}
