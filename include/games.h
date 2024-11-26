/*
Create a game creator class - which has a factory method to create a specific type of game


*/
#ifndef _GAMES_H
#define _GAMES_H

#include <memory>
#include <iostream>
#include <vector>
#include <ncurses.h>
#include <list>

using namespace std;

const int kBoardSize = 3;
const int kWidth = 20;
const int kHeight = 20;
const int kScoreIncrement = 10;
const int kLargestTimer = 250000;


enum class GameName{
    TIC_TAC_TOE = 0,
    SNAKE = 1
};

enum class Direction{
    STOP = 0,
    UP = 1,
    DOWN = 2,
    LEFT = 3,
    RIGHT = 4
};

struct Pos{
    int x;
    int y;

    Pos() {}

    Pos(int x, int y) : x(x),y(y) { }

    friend Pos operator+(const Pos& a, const Pos& b) {
        Pos result;
        result.x  = a.x + b.x;
        result.y = a.x + b.y;
        return result;
    }

    Pos& operator=(const Pos& a) {
        x = a.x;
        y = a.y;
        return *this;
    }

    bool operator==(const Pos& a) const{
        return (this->x == a.x && this->y == a.y);
    }

    void operator()(int x, int y) {
        this->x = x;
        this->y = y;
    }
};

class GameType{
    private:

    public:
        GameType() {}
        virtual ~GameType() {}

        virtual void setup() = 0;
        virtual void input() = 0;
        virtual void draw() = 0;
        virtual void run() = 0;
        virtual bool gameComplete() = 0;
};

class GameTicTakToe : public GameType {
    private:
        vector<vector<char>> board;
        bool isTie; 
        pair<char, char> players;
        char currentPlayer;

        // to win the game
        

    public:
        GameTicTakToe();
        ~GameTicTakToe();

        void setup();
        void input();
        void draw();
        void run();

        /*
        Check for game completion. If its a tie or if there is a winnder
        */
        bool gameComplete();
};

class GameSnake: public GameType {
    private:
        vector<vector<char>> grid;
        bool gameOver;
        int score;
        Direction direction;
        Pos fruitPos; 
        //Pos currentPos;
        std::list<Pos> currentPos; // contains the pos and the tails.
        int width;
        int height;
        int largestTimer;
        bool grow;

        void _generate();
        bool _checkPosInList(const Pos& pos);

    public:
        GameSnake();
        ~GameSnake();
        void setup();
        void input();
        void draw();
        void run();
        bool gameComplete();

        void updatePosition();
        void updateGame();
};

class GameCreator {
    private:

    public:
        GameCreator() {}
        virtual ~GameCreator() {}
        virtual std::unique_ptr<GameType> createGame(GameName gameName) = 0; // just creates different types of games. 
};

class ConsoleGameCreator : public GameCreator{
    public:
        ConsoleGameCreator();
        ~ConsoleGameCreator();

        std::unique_ptr<GameType> createGame(GameName gameName); 
};

#endif