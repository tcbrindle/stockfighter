
#pragma once

#include <json.hpp>

namespace stockfighter {
namespace rest {

auto get(const std::string& uri,
         const std::string& api_key = {}) -> nlohmann::json;

auto post(const std::string& uri,
          const std::string& body = std::string{},
          const std::string& api_key = {}) -> nlohmann::json;

auto delete_(const std::string& uri,
             const std::string& api_key = {}) -> nlohmann::json;

}
}
