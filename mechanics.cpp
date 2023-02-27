#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>

#include "game.hpp"
#include "entity.hpp"
#include "enum.hpp"
/**
 * check state of all team's minion's hp to know if you need to end the game
 * @param p1
 * @param p2
 * @return true if you need to continue the game
 */

auto check_hp_state(player p1, player p2)-> bool {
    auto sum1=0;
    auto sum2=0;
    for(auto e : p1.team){
        sum1+=e.hp;
    }
    for(auto e : p2.team){
        sum2+=e.hp;
    }
    return sum1==0 || sum2==0;
}

/**
 * check if there is any minion alive
 * @param p
 * @return true if there is at least one alive minion
 */
auto is_any_alive(player p)->bool{
    auto x=0;
    for (auto e : p.team){
        if(is_alive(e)){
            x++;
        }
    }
    return x!=0;
}
/**
 * this method looks for changing player after death
 * @param p
 * @return index of minon you can change to
 */
auto find_to_change(player p)->int{
    srand(time(NULL));
    if(is_any_alive(p)){
        while (true) {
            auto tmp = rand() % p.team.size();
            if (is_alive(p.team[tmp])) {
                return tmp;
            }
        }
    }
    return 0;
}
/**
 * looks for minion that computer can choose and check if computer has enough exp to evolve
 * @param p
 * @return index of minion that computer can evolve
 */
auto find_to_evolve(player p)->int{
    auto index=0;
    for (auto e : p.team){
        if(e.exp>=e.exp_to_evolve && is_alive(e)){
            return index;
        }else{
            index++;
        }
    }
    return -1;
}
/**
 * looks for alive enemy that computer can hit
 * @param p
 * @return index of minion to hit
 */
auto find_alive_enemy(player p)->int{
    srand(time(NULL));
    if(is_any_alive(p)){
        while (true) {
            auto tmp = rand() % 6;
            if (is_alive(p.team[tmp])) {
                return tmp;
            }
        }
    }
    return 0;
}
/**
 * draws computer actions
 * @param p
 * @return action computer can take
 */
auto rng_enemy_action(player p)->int{
    srand(time(NULL));
    if(find_to_evolve(p)!=-1 && rand()%10<5){
        return 3;
    }else if(p.main.use>0 && rand()%10<3){
        return 2;
    } else{
        return 1;
    }
}
/**
 * this method causes minions to faint
 * @param p1
 * @param p2
 * @param before
 * @param after
 * @param x
 */
auto faint(player& p1, player& p2, bool before, bool after, int x)->void{
    if(p2.team[x].hp<0){
        p2.team[x].hp=0;
    }

    if(!(before && after) && before){
        p1.main.exp += p2.team[x].exp_to_evolve/2;
        update_main(p1);
        if(p2.team.size()!=6)
        p2.main=p2.team[find_to_change(p2)];
        std::cout<<"Killed "<<p2.team[x].name<<"! ";
    }
}

/**
 * this method hits enemys
 * @param p1
 * @param p2
 * @param x
 */
auto hit(player& p1, player& p2, int x)-> void{
    srand(time(NULL));
    auto before = is_alive(p2.team[x]);
    auto power_up=p1.main.power + type_boost(p1.main,p2.team[x]);
    if(is_alive(p2.team[x]) && is_alive(p1.main)){
        if(rand() % 100 > p2.team[x].agility) {
            p2.team[x].hp -= power_up;
            std::cout<<"* "<<p1.main.name<<" hit "<<p2.team[x].name<<" for "<<power_up<<" *\n";
        }else{
            std::cout<<"* "<<p2.team[x].name<<" dodge *";
        }
    }else{
        std::cout<<p2.team[x].name<<" already fainted";
    }
    auto after= is_alive(p2.team[x]);
    faint(p1,p2,before,after,x);
}
/**
 * this method is ground minion's special
 * @param p
 */
auto wall_special(player& p)->void{
    p.main.hp+=100;
}
/**
 * this method is ground minion's special
 * @param p
 */
auto fire_ball_special(player& p1, player& p2)->void{
    srand(time(NULL));
    p2.team[rand()%p2.team.size()].hp-=50;
}
/**
 * this method is air minion's special
 * @param p
 */
auto fly_special(player& p)->void{
    p.main.exp+=100;
}
/**
 * this method is water minion's special
 * @param p
 */
auto tsunami_special(player& p1, player& p2)->void{
    for(auto& e : p2.team){
        e.hp-=20;
    }
}
/**
 * this method is ice minion's special
 * @param p
 */
auto froze_special(player& p)->void{
    p.main.agility+=10;
}
/**
 * this method is steel minion's special
 * @param p
 */
auto sword_special(player& p)->void{
    p.main.power+=50;
}
/**
 * this method uses special behavior of minions
 * @param p
 */
auto use_special(player& p1, player& p2)->void{
    if(p1.main.use>0) {
        if (is_alive(p1.main) && p1.main.use > 0) {
            switch (p1.main.ctype) {
                case type::water:
                    std::cout << "Used tsunami\nDeal damage to all enemies\n";
                    tsunami_special(p1, p2);
                    break;
                case type::ground:
                    std::cout << "Used wall\nGain hp\n";
                    wall_special(p1);
                    break;
                case type::air:
                    std::cout << "Used fly\nGain exp\n";
                    fly_special(p1);
                    break;
                case type::fire:
                    std::cout << "Used fireball\nDeal damage to random enemy\n";
                    fire_ball_special(p1, p2);
                    break;
                case type::ice:
                    std::cout << "Used froze\nGain agility\n";
                    froze_special(p1);
                    break;
                case type::steel:
                    std::cout << "Used sword\nGain power\n";
                    sword_special(p1);
                    break;
            }
        }
        p1.main.use--;
        update_main(p1);
    }else{
        std::cout<<"no more use";
    }
}
/**
 * this method change minion you controlle
 * @param p
 * @param nr
 */
auto change(player& p, int nr)-> void{
    if(is_alive(p.team[nr-1])) {
        switch (nr) {
            case 1:p.main = p.team[0];std::cout << "Control: " << p.main.name;
                break;
            case 2:p.main = p.team[1];std::cout << "Control: " << p.main.name;
                break;
            case 3:p.main = p.team[2];std::cout << "Control: " << p.main.name;
                break;
            case 4:p.main = p.team[3];std::cout << "Control: " << p.main.name;
                break;
            case 5:p.main = p.team[4];std::cout << "Control: " << p.main.name;
                break;
            case 6:p.main = p.team[5];std::cout << "Control: " << p.main.name;
                break;
        }
    } else{
        std::cout<<"\nminion fainted";
    }
}
/**
 * this method after two levels gives minion extra hp
 * @param m
 */
auto level_up(minion& m)-> void{
    m.hp+=50;
}
/**
 * this method evolve minions
 * @param p
 * @param x
 * @param nr
 */
auto evolve(player& p, int x, int nr)->void{
    if(is_alive(p.team[x])){
        if (p.team[x].exp_to_evolve <= p.team[x].exp) {
            p.team[x].exp -= p.team[x].exp_to_evolve;
            for (int i = 0; i < 2; i++) {
                switch (nr) {
                    case 1:
                        p.team[x].hp += 50;
                        std::cout << "\nAdded hp to " << p.team[x].name << "\n";
                        break;
                    case 2:
                        p.team[x].power += 30;
                        std::cout << "\nUpgraded power to " << p.team[x].name << "\n";
                        break;
                    case 3:
                        p.team[x].agility += 10;
                        std::cout << "\nUpgraded agility to " << p.team[x].name << "\n";
                        break;
                }
            }
            p.team[x].level++;
            if(p.team[x].level%2){
                level_up(p.team[x]);
            }
        } else {
            std::cout << "\nCan't evolve, wasted your move\n";
        }
    }else{
        std::cout<<"\nminion fainted";
    }
}




