
#pragma once

#include <json.hpp>

namespace stockfighter {
namespace rest {

using header_t = std::pair<std::string, std::string>;

auto get(const std::string& uri,
         const header_t& header = header_t{}) -> nlohmann::json;

auto post(const std::string& uri,
          const std::string& body,
          const header_t& header = header_t{}) -> nlohmann::json;

auto delete_(const std::string& uri,
             const header_t& header = header_t{}) -> nlohmann::json;

}
}