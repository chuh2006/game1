#include <iostream>
#include <string>
#include <random>

class Enemy{
public:
    Enemy(int seed) {
        this->seed = seed;
        this->health = seed * 250;
        this->maxHealth = health;
        this->atk = seed * 10;
        this->strengthen = 0;
        this->resistance = 0.1;
    }

    bool isBust;

    int seed;
    double health;
    double maxHealth;
    double atk;
    double strengthen;
    double resistance;
    double tempResist;
    double bustProgress;

    void force(int floor){
        this->health *= floor/2 + 1;
        this->atk *= floor/2 + 1;
        this->strengthen += 0.1 * (floor/3+1);
        this->resistance += 0.1;
    }

    void final(){
        this->health *= 10;
        this->maxHealth = this->health;
        this->atk *= 4;
        this->strengthen += 0.2;
        this->resistance += 0.2;
    }

    double bust(int seed){
        this->bustProgress += 0.1*seed;
        if(this->bustProgress >= 1){
            this->bustProgress = 0;
            isBust = true;
            std::cout << "| enemy is busted\n| damage:" << 0.1 * this->maxHealth << std::endl;
            return processBust(seed);
        }
        return 0.0;
    }

private:
    double processBust(int seed) {
        this->tempResist = this->resistance;
        this->resistance = -0.5;
        this->health -= this->maxHealth * 0.1;
        return 0.1 * this->maxHealth;
    }
};
