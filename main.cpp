#include "game.cpp"

int main(){
    std::cout << "| input seed:";
    int seed;
    std::cin >> seed;
    Game game(seed);
    game.processGame();
    return 0;
}