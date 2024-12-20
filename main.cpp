#include "game.cpp"

void menu(){
    std::cout << "\n-----------------------" << std::endl;
    std::cout << "| 1. play" << std::endl;
    std::cout << "| 2. exit" << std::endl;
    std::cout << "-----------------------\n" << std::endl;
}

int main(){
    while(true){
        std::cout << "| input seed:";
        int seed;
        std::cin >> seed;
        Game *game = new Game(seed);
        game->processGame();
        delete game;
        menu();
        int in;
        std::cin >> in;
        switch(in){
            case 1:
                continue;
            case 2:
                break;
        }
        break;
    }
    return 0;
}
