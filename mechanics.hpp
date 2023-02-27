#pragma once
#include "entity.hpp"

auto check_hp_state(player p1, player p2)-> bool;
auto rng_enemy_action(player p)->int;
auto find_alive_enemy(player p)->int;
auto find_to_evolve(player p)->int;
auto evolve(player& p, int x, int nr)->void;
auto find_to_change(player p)->int;
auto change(player& p, int nr)-> void;
auto hit(player& p1, player& p2, int x)-> void;
auto use_special(player& p1, player& p2)->void;


