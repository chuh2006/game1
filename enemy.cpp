#include <iostream>
#include <string>
#include <random>

class Enemy{
public:
    Enemy(int seed, int floors, int gameSeed) {
        this->seed = seed;
        this->health = seed * 250;
        this->maxHealth = health;
        this->atk = seed * 10;
        this->strengthen = 0;
        this->resistance = 0.1;
        force(floors);
        if(floors == 12){
            final();
            this->health *= gameSeed/10 + 1;
            this->maxHealth = this->health;
        }
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

    double bust(int seed){
        int ret = getSeed(seed);
        this->bustProgress += 0.1*ret;
        if(this->bustProgress >= 1){
            this->bustProgress = 0;
            isBust = true;
            std::cout << "| enemy is busted\n| damage:" << 0.1 * this->maxHealth << std::endl;
            return processBust();
        }
        return 0.0;
    }

private:
    double processBust() {
        this->tempResist = this->resistance;
        this->resistance = -0.5;
        this->health -= this->maxHealth * 0.1;
        return 0.1 * this->maxHealth;
    }

    int getSeed(int seed){
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, seed);
        return dis(gen);
    }
    
    void force(int floor){
        this->health *= floor;
        this->maxHealth = this->health;
        this->atk *= floor/2 + 1;
        this->strengthen += 0.1 * (floor/3+1);
        this->resistance += 0.1 * (floor/3+1);
    }

    void final(){
        this->health *= 5;
        this->maxHealth = this->health;
        this->atk *= 2;
        this->strengthen += 0.2;
    }
};
