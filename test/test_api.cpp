
#include <stockfighter/api.hpp>

#include "catch.hpp"

#include <iostream>

namespace {

static const std::string test_venue = "TESTEX";
static const std::string test_stock = "FOOBAR";
static const std::string test_account = "EXB123456";

template <typename Container, typename Value>
bool contains(const Container& c, const Value& val)
{
    return std::find(std::cbegin(c), std::cend(c), val) != std::cend(c);
}

} // end anon namespace

TEST_CASE("Heartbeat is working", "[api][heartbeat]")
{
    REQUIRE(stockfighter::api::heartbeat());
}

TEST_CASE("Venue heartbeat is working", "[api][heartbeat][venue]")
{
    REQUIRE(stockfighter::api::venue_heartbeat(test_venue));
}

TEST_CASE("Heartbeat fails for unknown venue", "[api][heartbeat][venue]")
{
    REQUIRE_THROWS(stockfighter::api::venue_heartbeat("XXXX"));
}

TEST_CASE("List stocks works correctly", "[api][venue]")
{
    const auto stocks = stockfighter::api::get_stocks(test_venue);
    REQUIRE(stocks.size() > 0);
}

TEST_CASE("Orderbook works correctly", "[api][venue]")
{
    const auto orders = stockfighter::api::get_orderbook(test_venue, test_stock);
    REQUIRE(orders.venue == test_venue);
    REQUIRE(orders.symbol == test_stock);
    REQUIRE(orders.bids.size() > 0);
}

TEST_CASE("Requesting a quote works correctly", "[api]")
{
    const auto quote = stockfighter::api::get_quote(test_venue, test_stock);
    REQUIRE(quote.venue == test_venue);
    REQUIRE(quote.symbol == test_stock);
    REQUIRE(quote.bid_size <= quote.bid_depth);
    REQUIRE(quote.ask_size <= quote.ask_depth);
}

TEST_CASE("Orders can be placed", "[api]")
{
    auto* key = std::getenv("SF_API_KEY");
    if (key == nullptr) {
        std::clog << "API key not found, skipping tests\n"
                  << "Set environment variable SF_API_KEY to your StockFighter key"
                          << std::endl;
        return;
    }
    auto api = stockfighter::api{key};
    const auto status = api.place_order(
            test_account, test_venue, test_stock, 1, 1,
            stockfighter::direction::buy, stockfighter::order_type::limit
    );

    REQUIRE(status.venue == test_venue);
    REQUIRE(status.symbol == test_stock);
    REQUIRE(status.account == test_account);
    REQUIRE(status.original_quantity == 1);
    REQUIRE(status.direction == stockfighter::direction::buy);
    REQUIRE(status.order_type == stockfighter::order_type::limit);
    REQUIRE(status.open == true);

    SECTION("Orders can be deleted again")
    {
        auto status2 = api.cancel_order(test_venue, test_stock, status.id);
        REQUIRE(status2.venue == test_venue);
        REQUIRE(status2.symbol == test_stock);
        REQUIRE(status2.account == test_account);
        REQUIRE(status2.original_quantity == status.original_quantity);
        REQUIRE(status2.account == status.account);
        REQUIRE(status2.direction == status.direction);
        REQUIRE(status2.order_type == status.order_type);
        // Hmmm, shouldn't the second response have a new timestamp?
        REQUIRE(status2.timestamp == status.timestamp);
        REQUIRE_FALSE(status2.open);
    }
}