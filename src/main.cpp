#include <memory>
#include <games.h>

using namespace std;

int main(int argc, char** argv) {
    initscr();
    //cbreak();
    raw();
    noecho();
    

    unique_ptr<GameCreator> gameCreator = std::make_unique<ConsoleGameCreator>();

    // create the tic tak toe game
    unique_ptr<GameType> game = gameCreator->createGame(GameName::SNAKE);

    if (game == nullptr) {
        std::cout << "Unknown game : " << std::endl;
    }

    // draw the game board
    //game->draw(); 

    // run the game
    nodelay(stdscr, TRUE); // non blocking keybo
    game->run();
    
    endwin(); // End ncurses mode
}