#include <iostream>
#include <string>

class Enemy{
public:
    Enemy(int seed) {
        this->seed = seed;
        this->health = seed * 250;
        this->atk = seed * 10;
        this->strengthen = 0;
        this->resistance = 0.1;
    }

    int seed;
    double health;
    double atk;
    double strengthen;
    double resistance;

    void force(int floor){
        this->health *= floor/2 + 1;
        this->atk *= floor/2 + 1;
        this->strengthen += 0.1*(floor/3 + 1);
        this->resistance += 0.1;
    }

    void final(){
        this->health *= 10;
        this->atk *= 4;
        this->strengthen += 0.2;
        this->resistance += 0.2;
    }
};
