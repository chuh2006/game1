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
                Enemy enemy(enemyId[i]);
                enemy.force(floor);
                if(floor == 12){
                    enemy.final();
                    enemy.health *= seed/10 + 1;
                }
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
                    showMsg(player, enemy);
                    enemy.isBust = false;
                    enemy.resistance = enemy.tempResist;
                }
                if(player.health <= 0){
                    std::cout << "| you lose" << std::endl;
                    return;
                }
                if(enemy.health <= 0){
                    std::cout << "| you win" << std::endl;
                    break;
                }
            }
            reward(player);
            if(lable == 1){
                reward(player);
                reward(player);
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
                player.epochDamage += enem.bust(1);
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

        void reward(Player &player){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 6);
            int in = dis(gen);
            int reward = dis(gen);
            reward += seed/10;
            if(seed>=10) {
                std::cout << "| get std upgrade" << std::endl;
                player.health += 3000 * reward;
                player.atk += 200 * reward;
                player.critical_C += 0.1 * reward;
                player.critical_D += 0.25 * reward;
                player.resistance += 0.05 * reward;
                player.strengthen += 0.1 * reward;
            }
            switch(in){
                case 1:
                    std::cout << "| get nengjinshou" << std::endl;
                    player.nengjinshou += 2 * reward;
                    break;
                case 2:
                    std::cout << "| get atk" << std::endl;
                    player.atk += 250 * reward;
                    break;
                case 3:
                    std::cout << "| get critical" << std::endl;
                    player.critical_C += 0.15 * reward;
                    player.critical_D += 0.3 * reward;
                    break;
                case 4:
                    std::cout << "| get strengthen and resisitance" << std::endl;
                    player.strengthen += 0.2 * reward;
                    player.resistance += 0.1 * reward;
                    break;
                case 5:
                    std::cout << "| get all upgrade" << std::endl;
                    player.health += 2500 * reward;
                    player.atk += 200 * reward;
                    player.critical_C += 0.1 * reward;
                    player.critical_D += 0.25 * reward;
                    player.strengthen += 0.2 * reward;
                    player.resistance += 0.05 * reward;
                    player.cureStrength += 0.2 * reward;
                    break;
                case 6:
                    std::cout << "| get all upgrade" << std::endl;
                    player.health += 2500 * reward;
                    player.atk += 200 * reward;
                    player.critical_C += 0.1 * reward;
                    player.critical_D += 0.25 * reward;
                    player.strengthen += 0.2 * reward;
                    player.resistance += 0.05 * reward;
                    player.cureStrength += 0.2 * reward;
                    break;
            }
            if(player.critical_C > 1){
                double temp = player.critical_C - 1;
                player.critical_C = 1;
                player.critical_D += 2 * temp;
            }
            if(player.resistance > 0.95){
                double temp = player.resistance - 0.95;
                player.resistance = 0.95;
                player.strengthen += 2 * temp;
            }
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
            std::cout << "| 1.go" << std::endl;
            std::cout << "| 2.quit" << std::endl;
            std::cout << "-----------------------\n" << std::endl;
        }
        void menu1(){
            std::cout << "\n-----------------------" << std::endl;
            std::cout << "| 1.common " + isCommonAvailable() << std::endl;
            std::cout << "| 2.leader " + isLiderAvailable() << std::endl;
            std::cout << "| 3.break " + isBreakAvailable() << std::endl;
            std::cout << "| 4.quit " << std::endl;
            std::cout << "-----------------------\n" << std::endl;
        }
        void menu2(){
            std::cout << "\n-----------------------" << std::endl;
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
            std::cout << "| nengjinshou: " << player.nengjinshou << std::endl;
            std::cout << "| sp: " << player.sp << std::endl;
            std::cout << "------------------------\n" << std::endl;
        }
    };
