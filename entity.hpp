#pragma once
#include "enum.hpp"
#include <string>
#include <iostream>

struct minion{
    minion(std::string name, int power, int agility, int hp, int exp,int exp_to_evolve, int level, type ctype, int use);

    std::string name;
    int power;
    int agility;
    int hp;
    int exp;
    int exp_to_evolve;
    int level;
    type ctype;
    int use;
};

struct player{
   std::string name;
   std::vector<minion> team;
   minion main;
   player(std::string name, std::vector<minion> team, minion main);
};

auto pula() -> std::vector<minion>;
auto is_alive(minion c)-> bool;
auto type_boost(const minion& c1, const minion& c2) -> int;
auto get_type(minion c)-> std::string;
auto display_all_minions() -> void;
auto num_to_type(int x)->type;
auto type_to_num(type t)->int;