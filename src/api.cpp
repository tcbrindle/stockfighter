
#include <stockfighter/api.hpp>

#include <boost/network/protocol/http/client.hpp>
#include <cppformat/format.h>
#include <json.hpp>

namespace http = boost::network::http;
namespace nl = nlohmann;

using namespace std::string_literals;

namespace stockfighter {

namespace {

template <typename ResponseType>
auto check_response(const ResponseType& response)
{
    if (status(response) != 200) {
        throw std::runtime_error{
                fmt::format("Error: received status message \"{}\"",
                            std::to_string(status(response)),
                            static_cast<std::string>(status_message(response)))};
    }

    const auto json = nl::json::parse(body(response));

    if (!json.value("error", "").empty()) {
        throw std::runtime_error{fmt::format("Remote error with message \"{}\"",
                                             json["error"].template get<std::string>())};
    }

    return json;
}

auto do_get(const std::string& uri, const std::string& key = std::string{})
{
    auto request = http::client::request{uri};
    if (!key.empty()) {
        request << boost::network::header("X-Starfighter-Authorization", key);
    }
    auto response = http::client{}.get(request);
    return check_response(response);
}

auto do_post(const std::string& uri,
             const std::string& key,
             const std::string& body_)
{
    auto request = http::client::request{uri};
    if (!key.empty()) {
        request << boost::network::header("X-Starfighter-Authorization", key);
    }
    const auto response = http::client{}.post(request, body_);

    return check_response(response);
}

auto do_delete(const std::string& uri, const std::string& key)
{
    auto request = http::client::request{uri};
    if (!key.empty()) {
        request << boost::network::header("X-Starfighter-Authorization", key);
    }
    const auto response = http::client{}.delete_(request);

    return check_response(response);
}

auto make_order_status(const nl::json& json)
{
    auto s = order_status{
            json.at("symbol"),
            json.at("venue"),
            direction_from_string(json.at("direction")),
            json.at("originalQty"),
            json.at("qty"),
            json.at("price"),
            order_type_from_string(json.at("orderType")),
            json.at("id"),
            json.at("account"),
            json.at("ts")
    };

    for (const auto& f : json.at("fills")) {
        s.fills.push_back(order_status::fill{f.at("price"), f.at("qty"), f.at("ts")});
    }

    return s;
}

} // end anonymous namespace


bool api::heartbeat()
{
    constexpr char uri[] = "https://api.stockfighter.io/ob/api/heartbeat";
    do_get(uri);

    return true;
}

bool api::venue_heartbeat(const std::string& venue)
{
    constexpr char uri[] = "https://api.stockfighter.io/ob/api/venues/{}/heartbeat";
    const auto json = do_get(fmt::format(uri, venue));

    if (json.at("venue") != venue) {
        throw std::runtime_error{"Not working"};
    }

    return true;
}

std::vector<stock> api::get_stocks(const std::string& venue)
{
    constexpr char uri[] = "https://api.stockfighter.io/ob/api/venues/{}/stocks";
    const auto json = do_get(fmt::format(uri, venue));

    auto output = std::vector<stock>{};

    for (const auto& s : json.at("symbols")) {
        output.push_back(stock{s.at("symbol"), s.at("name")});
    }

    return output;
}

orderbook api::get_orderbook(const std::string& venue, const std::string& stock)
{
    constexpr char uri[] = "https://api.stockfighter.io/ob/api/venues/{}/stocks/{}";
    const auto json = do_get(fmt::format(uri, venue, stock));

    auto output = orderbook{venue, stock};

    for (const auto& bid : json.at("bids")) {
        output.bids.push_back(
                orderbook::request{bid.at("price"), bid.at("qty"), bid.at("isBuy")});
    }

    for (const auto& ask : json.at("asks")) {
        output.bids.push_back(
                orderbook::request{ask.at("price"), ask.at("qty"), ask.at("isBuy")});
    }

    output.timestamp = json.at("ts").get<std::string>();

    return output;
}

quote api::get_quote(const std::string& venue, const std::string& stock)
{
    constexpr char uri[] = "https://api.stockfighter.io/ob/api/venues/{}/stocks/{}/quote";
    const auto json = do_get(fmt::format(uri, venue, stock));

    return quote {
            json.at("symbol"),
            json.at("venue"),
            json.at("bid"),
            //json.at("ask"),
            json.at("bidSize"),
            json.at("askSize"),
            json.at("bidDepth"),
            json.at("askDepth"),
            json.at("last"),
            json.at("lastSize"),
            json.at("lastTrade"),
            json.at("quoteTime")
    };
}

order_status api::place_order(const std::string& account,
                              const std::string& venue,
                              const std::string& stock,
                              int price,
                              int quantity,
                              direction dir,
                              order_type type)
{
    const auto in_json = nl::json::object(
            {
                    {"account",   account},
                    {"venue",     venue},
                    {"stock",     stock},
                    {"price",     price},
                    {"qty",       quantity},
                    {"direction", to_string(dir)},
                    {"orderType", to_string(type)}
            });

    constexpr char uri[] = "https://api.stockfighter.io/ob/api/venues/{}/stocks/{}/orders";
    const auto out_json = do_post(fmt::format(uri, venue, stock),
                                  key_, in_json.dump());

    return make_order_status(out_json);
}

order_status api::cancel_order(const std::string& venue,
                               const std::string& stock, int order_id)
{
    constexpr char uri[] = "https://api.stockfighter.io/ob/api/venues/{}/stocks/{}/orders/{}";
    const auto json = do_delete(fmt::format(uri, venue, stock, order_id), key_);
    return make_order_status(json);
}

} // end namespace
