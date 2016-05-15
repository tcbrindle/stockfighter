
#ifndef STOCKFIGHTER_TYPES_HPP
#define STOCKFIGHTER_TYPES_HPP

#include <string>
#include <vector>

namespace stockfighter {

struct stock {
    std::string symbol;
    std::string name;
};

enum class direction {
    buy,
    sell
};

inline auto to_string(direction d) -> std::string
{
    switch (d) {
    case direction::buy: return "buy";
    case direction::sell: return "sell";
    }
}

inline auto direction_from_string(const std::string& str) -> direction
{
    if (str == "buy") {
        return direction::buy;
    } else if (str == "sell") {
        return direction::sell;
    } else {
        throw std::domain_error("Unexpected argument \"" + str +
                "\" to direction_from_string()");
    }
}

struct orderbook {
    struct request {
        int price;
        int quantity;
        bool is_buy;
    };

    std::string venue;
    std::string symbol;
    std::vector<request> bids;
    std::vector<request> asks;
    std::string timestamp;
};

enum class order_type {
    limit,
    market,
    fill_or_kill,
    immediate_or_cancel
};

inline auto to_string(order_type o) -> std::string
{
    switch (o) {
    case order_type::limit: return "limit";
    case order_type::market: return "market";
    case order_type::fill_or_kill: return "fill-or-kill";
    case order_type::immediate_or_cancel: return "immediate-or-cancel";
    }
}

inline auto order_type_from_string(const std::string& str) -> order_type
{
    if (str == "limit") {
        return order_type::limit;
    } else if (str == "market") {
        return order_type::market;
    } else if (str == "fill-or-kill") {
        return order_type::fill_or_kill;
    } else if (str == "immediate-or-cancel") {
        return order_type::immediate_or_cancel;
    } else {
        throw std::domain_error("Unexpected argument \"" + str +
                "\" to direction_from_string()");
    }
}

struct quote {
    std::string symbol;
    std::string venue;
    int bid = 0; // best price currently bid for the stock
    //int ask = 0; // best price currently offered for the stock [broken/lie in docs]
    int bid_size = 0; // aggregate size of all orders at the best bid
    int ask_size = 0; // aggregate size of all orders at the best ask
    int bid_depth = 0; // aggregate size of *all bids*
    int ask_depth = 0; // aggregate sizeo of *all asks*
    int last = 0; // price of last trade
    int last_size = 0; // quantity of last trade
    std::string last_trade; // timestamp of last trade
    std::string quote_time; // timestamp of quote
};

struct order_status {
    struct fill {
        int price = 0;
        int quantity = 0;
        std::string timestamp;
    };

    std::string symbol;
    std::string venue;
    direction direction = direction::buy;
    int original_quantity = 0;
    int quantity = 0;
    int price = 0;
    order_type order_type = order_type::limit;
    int id = 0;
    std::string account;
    std::string timestamp;
    std::vector<fill> fills;

    int total_filled = 0;
    bool open = false;
};

}

#endif //STOCKFIGHTER_TYPES_HPP
