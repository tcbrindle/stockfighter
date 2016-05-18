
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

} // end anonymous namespace

auto get(const std::string& uri,
         const header_t& header) -> nl::json
{
    auto request = http::client::request{uri};
    if (!header.first.empty()) {
        request.add_header(header);
    }
    auto response = http::client{}.get(request);
    return check_response(response);
}

auto post(const std::string& uri,
          const std::string& body_,
          const header_t& header) -> nl::json
{
    auto request = http::client::request{uri};
    if (!header.first.empty()) {
        request.add_header(header);
    }
    const auto response = http::client{}.post(request, body_);
    return check_response(response);
}

auto delete_(const std::string& uri,
             const header_t& header) -> nl::json
{
    auto request = http::client::request{uri};
    if (!header.first.empty()) {
        request.add_header(header);
    }
    const auto response = http::client{}.delete_(request);
    return check_response(response);
}

} // end namespace rest
} // end namespace stockfighter

