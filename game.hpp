#pragma once
#include "entity.hpp"
#include "enum.hpp"

auto choseCrew() -> std::vector<minion>;
auto game() -> void;
auto display_team(player& p) -> void;
auto display_game_state(player& p1, player& p2) -> void;
auto end_line()->void;
auto end_turn()->void;
auto continue_game()->void;
auto help(game_state s)-> void;
auto input(int end, int amount)->std::set<std::string>;
auto update_main(player& p)->void;
auto choose_evolve(player& p)-> void;
auto choose_to_change(player& p)->void;
auto choose_to_hit(player& p1,player& p2)->void;