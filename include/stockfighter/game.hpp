
#ifndef STOCKFIGHTER_GAME_HPP
#define STOCKFIGHTER_GAME_HPP

#include <stockfighter/types.hpp>

#include <string>

namespace stockfighter {
namespace game {

auto start_level(const std::string& api_key, int level_num) -> level_info;

auto restart_level(const std::string& api_key, int instance_id) -> level_info;

auto stop_level(const std::string& api_key, int instance_id) -> bool;

auto resume_level(const std::string& api_key, int instance_id) -> level_info;

auto get_level_status(const std::string& api_key, int instance_id) -> level_status;

}
}

#endif // STOCKFIGHTER_GAME_HPP
