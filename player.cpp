#include <iostream>
#include <string>
#include <random>

class Player{
public:
    Player(int id){
        this->id = id;
        std::cout << "Enter your name: ";
        std::cin >> this->name;
        this->health = 12500;
        this->atk = 2500;
        this->critical_C = 0.5;
        this->critical_D = 1.5;
        this->strengthen = 0.1;
        this->resistance = 0.2;
        this->sp = 0;
        this->cureStrength = 1;
        this->nengjinshou = 3;
    }

    int id;
    std::string name;
    double health;
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
private:
    int isCritical(){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0,100);
        return (dis(gen) <= (this->critical_C * 100));
    }
};
