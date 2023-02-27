#include <iostream>
#include <string>
#include <vector>
#include <ctime>

#include "entity.hpp"
#include "enum.hpp"


minion::minion(std::string name, int power, int agility, int hp, int exp,int exp_to_evolve,int level, type ctype, int use)
        : name(name), power(power), agility(agility), hp(hp), exp(exp), exp_to_evolve(exp_to_evolve), level(level), ctype(ctype), use(use){}

player::player(std::string name, std::vector<minion> team, minion main)
        : name(name), team(team), main(main){}

/**
 * this method generate power
 * @param t
 * @return amount of power
 */
auto generate_power(type t)->int{
    srand(time(NULL));
    switch (t) {
        case type::water:return rand()%10+15;
        case type::ground:return rand()%20+10;
        case type::air:return rand()%10+10;
        case type::fire:return rand()%10+40;
        case type::ice:return rand()%10+30;
        case type::steel:return rand()%5+45;
    }
}
/**
 * this method generate agility
 * @param t
 * @return amount of agility
 */
auto generate_agility(type t)->int {
    srand(time(NULL));
    switch (t) {
        case type::water:return rand()%5+10;
        case type::ground:return rand()%5+5;
        case type::air:return rand()%10+20;
        case type::fire:return rand()%50+10;
        case type::ice:return rand()%10+5;
        case type::steel:return rand()%10+8;
    }
}
/**
 * this method generate hp
 * @param t
 * @return amount of hp
 */
auto generate_hp(type t)->int {
    switch (t) {
        case type::water:return 80;
        case type::ground:return 110;
        case type::air:return 70;
        case type::fire:return 90;
        case type::ice:return 60;
        case type::steel:return 100;
    }
}
/**
 * this method generate uses of special power
 * @param t
 * @return amount of special power uses
 */
auto set_use_special(type t)->int{
    srand(time(NULL));
    switch (t) {
        case type::water:return rand()%4+1;
        case type::ground:return rand()%3+1;
        case type::air:return rand()%2+1;
        case type::fire:return rand()%3+1;
        case type::ice:return rand()%2+2;
        case type::steel:return 1;
    }
}
/**
 * this method generates vector of minions that players can choose from
 * @return minion vector
 */
auto pula() -> std::vector<minion>{
    type t;
    auto vec = std::vector<minion>();
    t=type::ground;
         vec.push_back(minion("John", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Renly", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Joffrey", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
    t=type::water;
         vec.push_back(minion("Stannis", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Tywin", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
    t=type::ice;
        vec.push_back(minion("Jaime", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Cercei", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
    t=type::fire;
         vec.push_back(minion("Aeron", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Asha", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
    t=type::steel;
         vec.push_back(minion("Victarion", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Arya", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Brandon", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
    t=type::air;
         vec.push_back(minion("Eddard", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Rickon", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
         vec.push_back(minion("Tyrion", generate_power(t), generate_agility(t), generate_hp(t), 0, 20, 1, t,set_use_special(t)));
    return vec;
}
/**
 * checks if minion is alive
 * @param c
 * @return if minion is alive
 */
auto is_alive(minion c)-> bool {
    return c.hp>0;
}
/**
 * this method gives bonus to attack depends of type of minion
 * @param c1
 * @param c2
 * @return bonus attack
 */
auto type_boost(const minion& c1, const minion& c2) -> int{
    switch (c1.ctype) {
        case type::water:
            if(c2.ctype==type::ground || c2.ctype==type::fire){return c2.power/4;}
            else if(c2.ctype==type::water){return (-1)*c2.power/4;}
        case type::ground:
            if(c2.ctype==type::fire || c2.ctype==type::ice || c2.ctype==type::steel){return c2.power/4;}
            else if(c2.ctype==type::air){return (-1)*c2.power/4;}
        case type::air:
            if(c2.ctype==type::ice){return c2.power/4;}
            else if(c2.ctype==type::ground ||c2.ctype==type::steel){return (-1)*c2.power/4;}
        case type::fire:
            if(c2.ctype==type::ice || c2.ctype==type::steel){return c2.power/4;}
            else if(c2.ctype==type::water || c2.ctype==type::ground){return (-1)*c2.power/4;}
        case type::ice:
            if(c2.ctype==type::ground){return c2.power/4;}
            else if(c2.ctype==type::water || c2.ctype==type::fire || c2.ctype==type::ice){return (-1)*c2.power/4;}
        case type::steel:
            if(c2.ctype==type::water || c2.ctype==type::air){return c2.power/4;}
            else if(c2.ctype==type::fire || c2.ctype==type::steel){return (-1)*c2.power/4;}
    }
    return 0;
}

/**
 * transforme enum type to string
 * @param m
 * @return string of type
 */
auto get_type(minion m)-> std::string{
    switch (m.ctype) {
        case type::water: return "water";
        case type::ground: return "ground";
        case type::air: return "air";
        case type::fire: return "fire";
        case type::ice: return "ice";
        case type::steel: return "steel";
    }
}
/**
 * transforme int type to enum type
 * @param m
 * @return enum type
 */
auto num_to_type(int x)->type{
    switch (x) {
        case 1: return type::water;
        case 2: return type::ground;
        case 3: return type::air;
        case 4: return type::fire;
        case 5: return type::ice;
        case 6: return type::steel;
    }
}

/**
 * transforme enum type to int type
 * @param m
 * @return int type
 */
auto type_to_num(type t)->int{
    switch (t) {
        case type::water: return 1;
        case type::ground: return 2;
        case type::air: return 3;
        case type::fire: return 4;
        case type::ice: return 5;
        case type::steel: return 6;
    }
}
/**
 * shows all of the available minions
 */
auto display_all_minions() -> void{
    auto i=1;
    std::cout<<'\n';
    for(auto e : pula()){
            std::cout<<i<<". "<<e.name<<
                     ": health points - "<<e.hp<<
                     ", power - "<<e.power<<
                     ", agility - "<<e.agility<<
                     ", exp - "<<e.exp<<
                     ", exp to evolve - "<<e.exp_to_evolve<<
                     ", level - "<<e.level<<
                     ", type - "<<get_type(e)<<
                     ", use - "<<e.use<<'\n';
            i++;
    }
}