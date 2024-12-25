#include <iostream>
#include <string>
#include <vector>
#include <random>
#include "player.cpp"
#include "enemy.cpp"

class Game{
public:
    Game(int seed){
        this->seed = seed;
        this->floor = 1;
    }

    void processGame(){
        while(true){
            menu0();
            int in0;
            std::cin >> in0;
            switch(in0){
                case 1:
                    play();
                    break;
                case 2:
                    return;
            }
        }
    }

private:
    int seed;
    int floor;

    void play(){
        Player player(seed);
        while(floor < 13){
            menu1();
            int in1;
            std::cin >> in1;
            switch(in1){
                case 1:
                    processEnemy(player, 0);
                    break;
                case 2:
                    processEnemy(player, 1);
                    break;
                case 3:
                    break1(player);
                    break;
                case 4:
                    return;
            }
        }
        std::cout << "\n--------- end ---------" << std::endl;
    }

    void processEnemy(Player &player, int lable){
        std::vector<int> enemyId;
        if(lable == 0){
            enemyId = getEasyEnemyId(seed);
            std::cout << "common" << std::endl;
        } else {
            enemyId = getHardEnemyId(seed);
            std::cout << "leader" << std::endl;
        }
        for (int i = 0; i < enemyId.size(); i++) {
            Enemy enemy(enemyId[i], floor, this->seed);
            std::cout << "enemy" + i << std::endl;
            while(player.health > 0 && enemy.health > 0){
                player.epochDamage = 0;
                menu2();
                int in2;
                std::cin >> in2;
                switch(in2){
                    case 1:
                        attact_PP({0.25, 0.25, 0.50}, player, enemy, 1);
                        player.health += 1000*player.cureStrength;
                        if(!enemy.isBust) attact_E(1, player, enemy);
                        break;
                    case 2:
                        attact_PP({0.50, 0.25, 1.5, 1}, player, enemy, 2);
                        player.nengjinshou += 1;
                        if(!enemy.isBust) attact_E(1, player, enemy);
                        break;
                    case 3:
                        if(player.isSkillAvailable()){
                            attact_PP({0.5, 1, 1.25, 1.5, 0.75, 2.5}, player, enemy, 0, true);
                            player.nengjinshou += 2;
                            if(!enemy.isBust) attact_E(1, player, enemy);
                            player.sp -= 10;
                        } else {
                            std::cout << "| sp is not enough" << std::endl;
                        }
                        break;
                    case 4:
                        player.health += enemy.atk * 10;
                        break;
                    case 5:
                        showStatus(player);
                        if(player.health < 1) player.health = 1000;
                        break;
                }
                if(player.nengjinshou >= 6 && in2 != 4 && in2 != 5){
                    player.nengjinshou -= 6;
                    attact_PP({1, 0.5, 2, 1, 1.5}, player, enemy, 0);
                }
                std::cout << "| player's total damage: " << player.epochDamage << std::endl;
                std::cout << std::endl;
                showMsg(player, enemy);
                enemy.isBust = false;
                enemy.resistance = enemy.tempResist;
            }
            if(player.health <= 0){
                std::cout << "\n--------- end ---------" << std::endl;
                std::cout << "| you lose" << std::endl;
                return;
            }
            if(enemy.health <= 0){
                std::cout << "\n--------- end ---------" << std::endl;
                std::cout << "| you win" << std::endl;
                break;
            }
        }
        player.reward(seed);
        player.sp += 2;
        if(lable == 1){
            player.reward(seed);
            player.reward(seed);
        }
        floor++;
    }

    void break1(Player &player){
        player.health += 1000;
        player.sp += 2;
    }

    void showMsg(Player &player, Enemy &enemy){
        std::cout << "| player.";
        std::cout << "health: " << player.health << std::endl;
        std::cout << "| enemy.";
        std::cout << "health: " << enemy.health << std::endl;
    }

    std::string isCommonAvailable(){
        if(this->floor == 4 || this->floor == 8 || this->floor == 12) return "(unavailable)";
        return "(available)";
    }

    std::string isLiderAvailable(){
        if(this->floor != 4 && this->floor != 8 && this->floor != 12) return "(unavailable)";
        return "(available)";
    }

    std::string isBreakAvailable(){
        if(this->floor != 11) return "(unavailable)";
        return "(available)";
    }

    std::vector<int> getEasyEnemyId(int seed){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis = getEasyRange(seed);
        std::vector<int> ret;
        for (int i = 0; i < seed; i++) {
            int in = dis(gen);
            ret.push_back(in);
        }
        return ret;
    }

    std::vector<int> getHardEnemyId(int seed){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis = getHardRange(seed);
        std::vector<int> ret;
        for (int i = 0; i < seed; i++) {
            int in = dis(gen);
            ret.push_back(in);
        }
        return ret;
    }

    double calcDam_P(Player &player, Enemy &enemy){
        if(enemy.resistance >= 0) return (player.real_D() * (1 - (enemy.resistance * (1 - player.penentration))));
        return (player.real_D() * (1 - enemy.resistance));
    }
    double calcDam_E(Enemy &enemy, Player &player){
        return (enemy.atk * (1 - player.resistance));
    }

    void attact_P(double time, Player &player, Enemy &enemy, int sp){
        double damage = time * calcDam_P(player, enemy);
        enemy.health -= damage;
        std::cout << "| player damage: " << damage << std::endl;
        player.epochDamage += damage;
        player.sp += sp;
    }
    void attact_PP(std::vector<double> times, Player &player, Enemy &enem, int sp, bool isSkill = false){
        auto temp1 = player.strengthen;
        auto temp2 = player.critical_D;
        for (double time : times) {
            attact_P(time, player, enem, 0);
            player.epochDamage += enem.bust(2);
            if(isSkill){
                player.strengthen += 0.5;
                player.critical_D += 0.2;
            }
        }
        player.sp += sp;
        player.strengthen = temp1;
        player.critical_D = temp2;
    }
    void attact_E(double time, Player &player, Enemy &enemy){
        double damage = time * calcDam_E(enemy, player);
        player.health -= damage;
        std::cout << "| enemy damage: " << damage << std::endl;
    }

    std::uniform_int_distribution<> getEasyRange(int seed){
        std::uniform_int_distribution<> ret(seed*75, seed*150);
        return ret;
    }
    std::uniform_int_distribution<> getHardRange(int seed){
        std::uniform_int_distribution<> ret(seed*300, seed*600);
        return ret;
    }

    void menu0(){
        std::cout << "\n-----------------------" << std::endl;
        std::cout << "| 1.start" << std::endl;
        std::cout << "| 2.quit" << std::endl;
        std::cout << "-----------------------\n" << std::endl;
    }
    void menu1(){
        std::cout << "\n----- SelectEnemy -----" << std::endl;
        std::cout << "| 1.common " + isCommonAvailable() << std::endl;
        std::cout << "| 2.leader " + isLiderAvailable() << std::endl;
        std::cout << "| 3.break " + isBreakAvailable() << std::endl;
        std::cout << "| 4.quit " << std::endl;
        std::cout << "-----------------------\n" << std::endl;
    }
    void menu2(){
        std::cout << "\n------------------------" << std::endl;
        std::cout << "| 1.common attack" << std::endl;
        std::cout << "| 2.skill 1" << std::endl;
        std::cout << "| 3.skill 2" << std::endl;
        std::cout << "| 4.cure" << std::endl;
        std::cout << "| 5.show status" << std::endl;
        std::cout << "------------------------\n" << std::endl;
    }
    void showStatus(Player &player) {
        std::cout << "\n------------------------" << std::endl;
        std::cout << "| name: " << player.name << std::endl;
        std::cout << "| health: " << player.health << std::endl;
        std::cout << "| atk: " << player.atk << std::endl;
        std::cout << "| critical_C: " << player.critical_C << std::endl;
        std::cout << "| critical_D: " << player.critical_D << std::endl;
        std::cout << "| strengthen: " << player.strengthen << std::endl;
        std::cout << "| resistance: " << player.resistance << std::endl;
        std::cout << "| cureStrength: " << player.cureStrength << std::endl;
        std::cout << "| penentration: " << player.penentration << std::endl;
        std::cout << "| nengjinshou: " << player.nengjinshou << std::endl;
        std::cout << "| sp: " << player.sp << std::endl;
        std::cout << "------------------------\n" << std::endl;
    }
};
