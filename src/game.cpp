
#include <stockfighter/game.hpp>

#include <cppformat/format.h>
#include <json.hpp>

#include "rest.hpp"

namespace nl = nlohmann;
using namespace std::string_literals;

namespace stockfighter {
namespace game {

namespace {

auto json_to_level_info(const nl::json& json)
{
    auto info = level_info{
            json.at("account"),
            json.at("instanceId"),
            std::chrono::seconds{json.at("secondsPerTradingDay")}
    };

    for (const auto& t : json.at("tickers")) {
        info.tickers.push_back(t);
    }

    for (const auto& v : json.at("venues")) {
        info.venues.push_back(v);
    }

    return info;
}

auto json_to_level_status(const nl::json& json)
{
    const auto status = level_status{
            json.at("id"),
            json.at("done"),
            json.at("state") == "open"s ? level_state::open : level_state::closed,
            json.at("details").at("endOfTheWorldDay"),
            json.at("details").at("tradingDay")
    };

    return status;
}

}


auto start_level(const std::string& api_key, int level_num) -> level_info
{
    std::string uri;

    switch(level_num) {
    case 1:
        uri = "https://www.stockfighter.io/gm/levels/first_steps";
        break;
    case 2:
        uri = "https://www.stockfighter.io/gm/levels/chock_a_block";
        break;
    case 3:
        uri = "https://www.stockfighter.io/gm/levels/sell_side";
        break;
    default:
        throw std::runtime_error{"Unknown level"};
    }

    const auto response = rest::post(uri, "", api_key);

    return json_to_level_info(response);
}

auto restart_level(const std::string& api_key, int instance_id) -> level_info
{
    const auto response = rest::post(fmt::format("https://www.stockfighter.io/gm/instances/{}/restart",
                                                 instance_id),
                                     "",
                                     api_key);

    return json_to_level_info(response);
}

auto stop_level(const std::string& api_key, int instance_id) -> bool
{
    const auto response = rest::post(fmt::format("https://www.stockfighter.io/gm/instances/{}/stop",
                                                 instance_id),
                                     "",
                                     api_key);

    return true;
}

auto resume_level(const std::string& api_key, int instance_id) -> level_info
{
    const auto response = rest::post(fmt::format("https://www.stockfighter.io/gm/instances/{}/resume",
                                                 instance_id),
                                     "",
                                     api_key);

    return json_to_level_info(response);
}

auto get_level_status(const std::string& api_key,
                      int instance_id) -> level_status
{
    const auto response = rest::get(
            fmt::format("https://www.stockfighter.io/gm/instances/{}",
                        instance_id),
            api_key);

    return json_to_level_status(response);
}

} // end namespace game
} // end namespace stockfighter