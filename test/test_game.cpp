
#include "catch.hpp"

#include <stockfighter/game.hpp>

#include <iostream>

using namespace std::chrono_literals;

TEST_CASE("Game API is available", "[game]")
{
    auto* api_key = std::getenv("SF_API_KEY");
    if (api_key == nullptr) {
        std::clog << "API key not found, skipping test\n"
                << "Set environment variable SF_API_KEY to your StockFighter key"
                << std::endl;
        return;
    }

    SECTION("Level 1 can be started correctly")
    {
        auto info = stockfighter::game::start_level(api_key, 1);
        REQUIRE_FALSE(info.account.empty());
        REQUIRE(info.instance_id > 0);
        REQUIRE(info.seconds_per_trading_day > 0s);
        REQUIRE_FALSE(info.tickers.empty());
        REQUIRE_FALSE(info.venues.empty());

        SECTION("Can get level status")
        {
            auto status = stockfighter::game::get_level_status(api_key,
                                                               info.instance_id);
            REQUIRE(status.id == info.instance_id);
            REQUIRE(status.done == false);
            REQUIRE(status.state == stockfighter::level_state::open);
            REQUIRE(status.end_of_the_world_day > 0);
            REQUIRE(status.trading_day > 0);
        }

        SECTION("Can resume level")
        {
            auto new_info = stockfighter::game::resume_level(api_key,
                                                             info.instance_id);
            REQUIRE(new_info.instance_id == info.instance_id);
            REQUIRE(new_info.account == info.account);
            REQUIRE(new_info.venues == info.venues);
            REQUIRE(new_info.tickers == info.tickers);
            REQUIRE(new_info.seconds_per_trading_day == info.seconds_per_trading_day);
        }

        SECTION("Can restart level")
        {
            auto new_info = stockfighter::game::restart_level(api_key,
                                                              info.instance_id);
            REQUIRE(new_info.instance_id == info.instance_id);
            REQUIRE(new_info.account != info.account);
            REQUIRE(new_info.venues != info.venues);
            REQUIRE(new_info.tickers != info.tickers);
            REQUIRE(new_info.seconds_per_trading_day == info.seconds_per_trading_day);
        }

        SECTION("Can stop level")
        {
            REQUIRE_NOTHROW(stockfighter::game::stop_level(api_key, info.instance_id));

            SECTION("Can't get status of a stopped level") {
                REQUIRE_THROWS(stockfighter::game::get_level_status(api_key, info.instance_id));
            }
        }
    }
}