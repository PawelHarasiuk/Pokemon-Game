#include <iostream>
#include <string>
#include <vector>
#include <set>
#include <cstdlib>
#include <ctime>
#include <fstream>

#include "entity.hpp"
#include "enum.hpp"
#include "mechanics.hpp"

int difficult;
int rounds;
int round=1;
game_state gs;

auto end_line()->void{
    std::cout<<'\n';
    for (int i = 0; i < 120; ++i) {
        std::cout<<"_";
    }
    std::cout<<'\n';
}

auto end_turn()->void{
    std::cout<<'\n';
    std::cout<<'\n';
    for (int i = 0; i < 240; ++i) {
        std::cout<<"*";
    }
    std::cout<<'\n';
}

/**
 * help player
 * @param s
 */
auto help(game_state s)-> void{
    end_line();
    if(s == game_state::choose_minion){
        std::cout<<"- Enter number from 1 to 15 to choose 6 minions to your team\n";
        display_all_minions();
    }if(s == game_state::new_game){
        std::cout<<"- Enter 1 to start new game and choose new team\n- Enter 2 to load game";
    }else if(s == game_state::menu){
        std::cout<<"- Enter number from 1 to 4 to take an action\n1 - you can deal damage to opponent\n2 - use your special\n3 - change minion you controlling\n4 - upgrade one of your minion's parameter\n";
    }else if(s == game_state::hit){
        std::cout<<"- Choose enemy's minion from 1 to "<<difficult<<" to deal damage\n- Damage depends on your power and type of fighting minions\n";
    }else if(s == game_state::special){
        std::cout<<"- C\n";
    }else if(s == game_state::change_minion){
        std::cout<<"- Enter number from 1 to 6 to choose minion you want to control\n- Only minion you control, can take an action\n";
    }else if(s == game_state::evolve){
        std::cout<<"- Enter number from 1 to 6 to choose minion you want to evolve\n- Only minion that have enough exp, can evolve\n- After choosing minion you can decide what two parameters you want to upgrade\n";
        std::cout<<"- Enter number from 1 to 3\n1 - heal your minion\n2 - upgrade power to deal more damage\n3 - upgrade agility to have bigger chance to dodge hit\n";
    }else if(s == game_state::settings){
        std::cout<<"- Choose number of rounds:\n1 - 1 round\n2 - 2 round\n3 - 3 round\n4 - 4 round\n";
        std::cout<<"- Choose difficulty:\n1 - (easy) 1 enemy minion\n2 - (medium) 2 enemy minions\n3 - (hard) 3 enemy minions\n4 - (very hard) 4 enemy minions\n";
    }else if(s == game_state::end_game){
        std::cout<<"- Enter 1 to continue game, you have "<<rounds-round<<"left\nEnter 2 to save your progress and exit game\n";
    }
    end_line();
}

/**
 * provides legal choices
 * @param amount
 * @return returns the set of commands to be used
 */
auto legal_action_choose(int amount)->std::set<std::string>{
    auto s = std::set<std::string>();
    for (int i = 1; i < amount; i++) {
        s.insert(std::to_string(i));
    }
    return s;
}

/**
 * provides commands to be executed
 * @param end
 * @param amount
 * @return returns commands
 */
auto input(int end, int amount)->std::set<std::string>{
    auto s = std::set<std::string>();
    std::string x;
    while(s.size()<end){
        std::cout<<'\n'<<s.size()+1<<"/"<<end<<" enter command: ";
        std::cin>>x;
        if(x=="--h" || x=="-help"){
            help(gs);
        }
        for(auto e : legal_action_choose(amount)){
            if(e==x){
                s.insert(x);
            }
        }
    }
    return s;
}
/**
 * gives player team
 * @return returns the team
 */
auto choseCrew() -> std::vector<minion> {
    gs=game_state::choose_minion;
    std::cout<<"\nChoose members of your team\n";
    std::vector<minion> vec;
    for (auto e : input(6,15)) {
        vec.push_back(pula()[(stoi(e)-1)]);
    }
    return vec;
}

/**
 * draws enemy team
 * @return vector of enemy team
 */
auto draw_enemy() -> std::vector<minion>{
    srand(time(NULL));
    std::vector<minion> vec;
    std::set<int> s;
    while(s.size()<difficult){
        s.insert(rand()%15);
    }
    for (auto e : s) {
        vec.push_back(pula()[e]);
    }
    return vec;
}

/**
 * updates the main character in the vector
 * @param p
 */
auto update_main(player& p)->void{
    for (auto& e : p.team) {
        if(e.name==p.main.name){
            e=p.main;
        }
    }
}
/**
 * show players team
 * @param p
 */
auto display_team(player& p) -> void{
    for(auto e : p.team){
        if(is_alive(e)) {
            std::cout << e.name <<
                      ": health points - " << e.hp <<
                      ", power - " << e.power <<
                      ", agility - " << e.agility <<
                      ", exp - " << e.exp <<
                      ", exp to evolve - " << e.exp_to_evolve <<
                      ", level - " << e.level <<
                      ", type - " << get_type(e) << '\n';
            }
        else {
            std::cout << e.name << " is dead " << '\n';
        }
    }
    std::cout<<"\nMain: "<<p.main.name<<'\n';
}
/**
 * shows state of both teams
 * @param p1
 * @param p2
 */
auto display_game_state(player& p1, player& p2) -> void{
    std::cout<<"\nYOUR TEAM:\n";
    display_team(p1);
    end_line();
    std::cout<<"\nENEMYS'S TEAM:\n";
    display_team(p2);
    end_line();
}
/**
 * allows player to stop game and read interface
 */
auto continue_game()->void{
    std::string x;
    std::cout<<"TYPE TO CONTINUE ";
    std::cin>>x;
}
/**
 * player can choose who want to hit
 * @param p1
 * @param p2
 */
auto choose_to_hit(player& p1,player& p2)->void{
    std::cout<<"\nChoose enemy minion to hit\n";
    auto x=0;
    for (auto e : input(1,difficult+1)) {
        x=stoi(e)-1;
    }
    std::cout<<"\nYOUR ACTIONS:\n";
    hit(p1,p2, x);
}

/**
 * player can choose who want to change
 * @param p
 */
auto choose_to_change(player& p)->void{
    std::cout<<"\nChoose minion you want to controle\n";
    auto x=0;
    for (auto e : input(1,7)) {
        x=stoi(e);
    }
    std::cout<<"\nYOUR ACTIONS:\n";
    change(p,x);
}

/**
 * player can choose who want evolve
 * @param p
 */
auto choose_evolve(player& p)-> void{
    std::cout<<"\nWhich minion you choose to evolve\n";
    auto x=0;
    auto nr=0;
    for (auto e : input(1,7)) {
        x=(stoi(e)-1);
    }
    std::cout<<"\nWhich parametr you choose: \n1.add health\n2.upgrade power\n3.upgrade agility\n";
    for (auto e: input(1, 7)) {
        nr=stoi(e);
    }
    std::cout<<"\nYOUR ACTIONS:\n";
    evolve(p, x,nr);
}

/**
 * this metod generates a round
 */
auto game_round(player p1, player p2)-> void{
    do{
        gs=game_state::menu;
        auto action=0;
        display_game_state(p1,p2);
        std::cout<<"\npress 1 to hit\n"<<"press 2 to use special\n"<<"press 3 to change\n"<<"press 4 to evolve\n";
        for (auto e : input(1,5)){
            action=stoi(e);
        }
        switch (action) {
            case 1:gs=game_state::hit;choose_to_hit(p1,p2);
                break;
            case 2:gs=game_state::special;use_special(p1,p2);
                break;
            case 3:gs=game_state::change_minion;choose_to_change(p1);
                break;
            case 4:gs=game_state::evolve;choose_evolve(p1);
                break;
        }
        std::cout<<"\nENEMY ACTIONS:\n";
        auto enemy_action= rng_enemy_action(p2);
        srand(time(NULL));
        switch (enemy_action) {
            case 1:hit(p2,p1,find_alive_enemy(p1));
                break;
            case 2:use_special(p2,p1);
                break;
            case 3:evolve(p2, find_to_evolve(p2),rand()%3+1);
                break;
        }
        end_turn();
        continue_game();
    }while (!check_hp_state(p1,p2));
}

/**
 * this method allows player to choose difficulty and amount of rounds
 */
auto set_settings()->void{
    gs=game_state::settings;
    std::cout<<"\nEnter number of rounds\n";
    auto x=0;
    for (auto e : input(1,5)){
        x=stoi(e);
    }
    rounds = x;
    std::cout<<"\nEnter difficulty\n";
    for (auto e : input(1,5)){
        x=stoi(e);
    }
    difficult = x;
}
/**
 * this method allows player to save the game
 * @param p
 */
auto save_game(player p)->void{
    auto file=std::fstream("..\\text.txt", std::ios::out | std::ios::trunc);
    file<<round<<'\n';
    file<<rounds<<'\n';
    file<<difficult<<'\n';
    file=std::fstream("..\\text.txt", std::ios::out | std::ios::app);
    for (auto e : p.team){
        auto h=100;
        if(e.hp>0){
        h=e.hp;
        }
    file<<e.name<<'\n';
    file<<e.power<<'\n';
    file<<e.agility<<'\n';
    file<<h<<'\n';
    file<<e.exp<<'\n';
    file<<e.exp_to_evolve<<'\n';
    file<<e.level<<'\n';
    file<<type_to_num(e.ctype)<<'\n';
    file<<e.use<<'\n';
    }
}
/**
 * this method allows player to save the game
 * @return
 */
auto load_game()->std::vector<minion>{
    std::vector<minion> vec;
    std::vector<std::string> v;
    auto file=std::fstream("..\\text.txt");

    auto round_current = std::string();
    auto nr_of_rounds = std::string();
    auto difficulty_level = std::string();
    file>>round_current;
    file>>nr_of_rounds;
    file>>difficulty_level;

    round=stoi(round_current);
    rounds=stoi(nr_of_rounds);
    difficult=stoi(difficulty_level);

    auto str = std::string();
    while (file>>str){
        v.push_back(str);
        std::cout<<str<<'\n';
    }
    for (int i = 0; i < v.size(); i+=v.size()/6) {
        auto m = minion(v[i], stoi(v[i+1]),stoi(v[i+2]),stoi(v[i+3]),stoi(v[i+4]),stoi(v[i+5]),stoi(v[i+6]),num_to_type(stoi(v[i+7])), stoi(v[i+8]));
        vec.push_back(m);
    }
    return vec;
}
/**
 * this method genareate new game
 * @return
 */
auto new_game()->int{
    gs=game_state::new_game;
    auto x=0;
    std::cout<<"\n1 - New Game\n2 - Load Game\n";
    for (auto e : input(1,3)){
        x=stoi(e);
    }
    if(x==1) {
        set_settings();
    }
    return x;
}
/**
 * this is main method that provides gameplay
 */
auto game() -> void{
    std::cout <<"\nUse keyboard to navigate the game\n";
    std::cout<<"Enter --h or -help, at any point in game get some help\n";
    std::vector<minion> team1;
    std::vector<minion> team2;
    switch (new_game()) {
        case 1:team1=choseCrew();
            break;
        case 2:team1=load_game();
            break;
    }
    auto p1=player("Player", team1, team1[0]);
    for ( ; 0 <= rounds-round; round++) {
        team2=draw_enemy();
        auto e1=player("Enemy", team2, team2[0]);
        game_round(p1,e1);
        std::cout<<"End of round "<<round<<'\n';
        end_line();
            gs=game_state::end_game;
            std::cout<<"\nEnter 1 to continue\nEnter 2 to save game and exit\n";
            auto x = 0;
            for (auto e: input(1, 3)) {
                x = stoi(e);
            }
            if (x == 2) {
                save_game(p1);
                rounds = 0;
            } else{
                end_line();
                std::cout<<"\nROUND "<<round<<" / "<<rounds<<'\n';
                end_line();
            }
    }
    std::cout<<"\nTHE END\n";
}
