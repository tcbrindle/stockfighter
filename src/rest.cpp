
#include "rest.hpp"

#include <boost/network/protocol/http/client.hpp>

#include <cppformat/format.h>

namespace http = boost::network::http;
namespace nl = nlohmann;

namespace stockfighter {
namespace rest {

namespace {

template <typename ResponseType>
auto check_response(const ResponseType& response)
{
    if (status(response) != 200) {
        throw std::runtime_error{
                fmt::format("Error: received status message \"{}\"",
                            std::to_string(status(response)),
                            static_cast<std::string>(status_message(
                                    response)))};
    }

    const auto json = nlohmann::json::parse(body(response));

    if (!json.value("error", "").empty()) {
        throw std::runtime_error{fmt::format("Remote error with message \"{}\"",
                                             json["error"].template get<std::string>())};
    }

    return json;
}

void add_header(http::client::request& request, const std::string& api_key)
{
    if (!api_key.empty()) {
        request << boost::network::header("X-Starfighter-Authorization",
                                          api_key);
    }
}

} // end anonymous namespace

auto get(const std::string& uri,
         const std::string& api_key) -> nl::json
{
    auto request = http::client::request{uri};
    add_header(request, api_key);
    auto response = http::client{}.get(request);
    return check_response(response);
}

auto post(const std::string& uri,
          const std::string& body_,
          const std::string& api_key) -> nl::json
{
    auto request = http::client::request{uri};
    add_header(request, api_key);
    const auto response = http::client{}.post(request, body_);
    return check_response(response);
}

auto delete_(const std::string& uri,
             const std::string& api_key) -> nl::json
{
    auto request = http::client::request{uri};
    add_header(request, api_key);
    const auto response = http::client{}.delete_(request);
    return check_response(response);
}

} // end namespace rest
} // end namespace stockfighter

