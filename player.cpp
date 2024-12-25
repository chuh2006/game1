#include <iostream>
#include <string>
#include <random>

class Player{
public:
    Player(int seed){
        this->id = seed;
        std::cout << "Enter your name: ";
        std::cin >> this->name;
        this->health = 12500 * seed;
        this->atk = 2500 * (seed / 50 + 1);
        this->critical_C = 0.5;
        this->critical_D = 1.5;
        this->strengthen = 0.1;
        this->resistance = 0.2;
        this->sp = 0;
        this->cureStrength = 1;
        this->nengjinshou = 3;
        this->penentration = 0.0;
        this->basicATK = this->atk;
    }
    double penentration;
    int id;
    std::string name;
    double health;
    double basicATK;
    double atk;
    double critical_C;
    double critical_D;
    double strengthen;
    double resistance;
    int sp;
    double cureStrength;
    long long epochDamage = 0;
    int nengjinshou;
    double real_D(){
        return(atk * (1 + strengthen + isCritical() * critical_D));
    }
    
    bool isSkillAvailable(){
        if(this->sp >= 10) return true;
        return false;
    }

    void reward(int seed){
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_int_distribution<> dis(1, 6);
            int in = dis(gen);
            int reward = dis(gen);
            reward += int(sqrt(seed));
            if(seed>=10) {
                std::cout << "| get std upgrade" << std::endl;
                this->health += 3000 * reward;
                this->atk += basicATK * 0.1 * reward;
                this->critical_C += 0.1 * reward;
                this->critical_D += 0.25 * reward;
                this->resistance += 0.05 * reward;
                this->strengthen += 0.1 * reward;
            } else {
                std::cout << "| get health" << std::endl;
                this->health += 3000 * reward;
            }
            switch(in){
                case 1:
                    std::cout << "| get penentration" << std::endl;
                    this->penentration += 0.025 * reward;
                    break;
                case 2:
                    std::cout << "| get atk" << std::endl;
                    this->atk += basicATK * 0.2 * reward;
                    break;
                case 3:
                    std::cout << "| get critical" << std::endl;
                    this->critical_C += 0.15 * reward;
                    this->critical_D += 0.3 * reward;
                    break;
                case 4:
                    std::cout << "| get strengthen and resisitance" << std::endl;
                    this->strengthen += 0.2 * reward;
                    this->resistance += 0.1 * reward;
                    break;
                case 5:
                    std::cout << "| get all upgrade" << std::endl;
                    this->health += 2500 * reward;
                    this->atk += basicATK * 0.2 * reward;
                    this->critical_C += 0.1 * reward;
                    this->critical_D += 0.25 * reward;
                    this->strengthen += 0.2 * reward;
                    this->resistance += 0.05 * reward;
                    this->cureStrength += 0.2 * reward;
                    this->penentration += 0.025 * reward;
                    break;
                case 6:
                    std::cout << "| get all upgrade" << std::endl;
                    this->health += 2500 * reward;
                    this->atk += basicATK * 0.2 * reward;
                    this->critical_C += 0.1 * reward;
                    this->critical_D += 0.25 * reward;
                    this->strengthen += 0.2 * reward;
                    this->resistance += 0.05 * reward;
                    this->cureStrength += 0.2 * reward;
                    this->penentration += 0.025 * reward;
                    break;
            }
            estimateRew(reward);
        }
private:
    int isCritical(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,100);
        return (dis(gen) <= (this->critical_C * 100));
    }

    void estimateRew(int reward){
        if(this->critical_C > 1){
            double temp = this->critical_C - 1;
            this->critical_C = 1;
            this->critical_D += 2 * temp;
        }
        if(this->resistance > 0.99){
            double temp = this->resistance - 0.99;
            this->resistance = 0.99;
            this->strengthen += 2 * temp;
        }
        if(this->penentration > 0.95){
            this->penentration = 0.95;
            std::cout << "| get little upgrade" << std::endl;
            this->health += 1500 * reward;
            this->atk += 150 * reward;
            this->critical_D += 0.1 * reward;
            this->strengthen += 0.1 * reward;
        }
    }
};
