#include <games.h>
#include <unistd.h>

ConsoleGameCreator::ConsoleGameCreator() {}

ConsoleGameCreator::~ConsoleGameCreator() {}

std::unique_ptr<GameType> ConsoleGameCreator::createGame(GameName gameName) {
    if (gameName == GameName::TIC_TAC_TOE) {
        return std::make_unique<GameTicTakToe>();
    } else if (gameName == GameName::SNAKE) {
        return std::make_unique<GameSnake>();
    } else {
        return nullptr;
    }
}

GameTicTakToe::GameTicTakToe() : isTie(false) {
    this->setup();
}

GameTicTakToe::~GameTicTakToe() {

}

void GameTicTakToe::setup() {
    board = vector<vector<char>>(kBoardSize, vector<char>(kBoardSize, '_'));
    players.first = 'X';
    players.second = 'O'; 
    currentPlayer = players.second;
}

void GameTicTakToe::input() {
    int row, col;
    currentPlayer = (currentPlayer == players.second)? players.first : players.second;
    std::cout << "Current player : " << currentPlayer << " Please enter row, col : ";
    while (true) {        
        std::cin >> row >> col;
        if (row >= 0 && col >= 0 && row < kBoardSize && col < kBoardSize && board[row][col] != players.first && board[row][col] != players.second) {
            break;
        }

        std::cout << "Invalid input - Current player : " << currentPlayer << " Please enter row, col : ";
    }
    board[row][col] = currentPlayer;
}

void GameTicTakToe::draw() {
    // printing the board
    for(int i = 0 ; i < board.size() ; i++) {
        for (int j = 0 ; j < board[0].size(); j++) {
            std::cout << "| " << board[i][j] << " ";
        }
        std::cout << "|" << std::endl;
    }
}

bool GameTicTakToe::gameComplete(){
    // check row to see if its the same value or not
    for(int i = 0; i < kBoardSize; i++) {
        bool winnerFound = true;
        for(int j = 1; j < kBoardSize; j++) {
            winnerFound = winnerFound && (board[i][j-1] == board[i][j] && board[i][j] == currentPlayer);
        }
        if (!winnerFound) continue;
        else {
            this->isTie = ~winnerFound;
            return true; 
        }
    }

    // check evyer column
    for(int j = 0; j < kBoardSize; j++) {
        bool winnerFound = true;
        for(int i = 1; i < kBoardSize; i++) {
            winnerFound = winnerFound && (board[i-1][j] == board[i][j] && board[i][j] == currentPlayer);
        }
        if (!winnerFound) continue;
        else {
            this->isTie = !winnerFound;
            return true; 
        }
    }

    // check the diagonal
    bool winnerFound = true;
    for(int i = 1; i < kBoardSize; i++) {
        winnerFound = winnerFound && (board[i][i] == board[i-1][i-1] && board[i][i] == currentPlayer);
    }
    if (winnerFound) {
        this->isTie = !winnerFound;
        return true;
    }

    // check the reverse diagonal
    winnerFound = true;
    for(int i = 1; i < kBoardSize; i++) {
        winnerFound = winnerFound && (board[i][kBoardSize-i-1] == board[i-1][kBoardSize-i] && board[i-1][kBoardSize-i] == currentPlayer);
    }
    if (winnerFound) {
        this->isTie = !winnerFound;
        return true;
    }

    // check if there any empty spaces in the board
    for(int i = 0 ; i < kBoardSize ; i++) {
        for(int j = 0 ; j < kBoardSize ; j++) {
            if (board[i][j] == '_') {
                this->isTie = false;
                return false;
            }
        }
    }

    this->isTie = true;
    return true;
}

void GameTicTakToe::run() {
    while (true) {
        // draw the board
        this->draw();
        
        // process the input
        this->input();

        // if game is complete
        if (this->gameComplete()) {
            break;
        }
    }

    std::cout << "Game complete.  " << std::endl;
    if (this->isTie) {
        std::cout << "Game tied" <<std::endl;
    } else {
        std::cout << "Winner : " << currentPlayer << std::endl;
    }
}

GameSnake::GameSnake() : width(kWidth), height(kHeight) {
    this->setup();
}

GameSnake::~GameSnake() {

}

void GameSnake::_generate() {
    this->fruitPos.x = rand() % width;
    this->fruitPos.y = rand() % height;
}

void GameSnake::setup() {
    this->gameOver = false;
    direction = Direction::STOP;
    this->largestTimer = kLargestTimer;
    Pos tempPos(width/2, height/2);
    this->currentPos.push_back(tempPos);
    this->_generate();
    score = 0;
    grow = false;
}

bool GameSnake::_checkPosInList(const Pos& pos) {
    auto it = std::find(this->currentPos.begin(), this->currentPos.end(), pos);
    return (it != this->currentPos.end());
}

void GameSnake::draw() {
    //system("clear");
    clear();

    // draw the boundaries - draw the first row
    for(int i = -1 ; i <= this->height; i++) {
        for(int j = -1 ; j <= this->width ;  j++) {
            // for boundary, we set 
            if (i == -1 || j == -1 || i == this->height || j == this->width) {
                //std::cout << "#";
                printw("#");
            } else if (this->_checkPosInList(Pos(j,i))) {
                //std::cout << "O";
                printw("O");
            } else if (i == this->fruitPos.y && j == this->fruitPos.x) {
                //std::cout << "F";
                printw("F");
            } else {
                //std::cout << " ";
                printw(" ");
            }
        }
        //std::cout << "\n";
        printw("\n");
    }

    printw("Score : %d", this->score);
    if (this->gameOver) printw("Game Over");

    refresh();
}

void GameSnake::input() {
    switch(getch()) {
        case 'a':
            this->direction = (this->direction != Direction::RIGHT)? Direction::LEFT : this->direction;
            break;
        case 'd':
            this->direction = (this->direction != Direction::LEFT)? Direction::RIGHT : this->direction;
            break;
        case 'w':
            this->direction = (this->direction != Direction::DOWN)? Direction::UP : this->direction;
            break;
        case 's':
            this->direction = (this->direction != Direction::UP)? Direction::DOWN : this->direction;
            break;
        case 'x':
            this->direction = Direction::STOP;
            this->gameOver = true;
            break;
    }
}

void GameSnake::updatePosition() {
    Pos tempPos = (*this->currentPos.begin()); // get the head

    // update the head to get the new head
    switch (this->direction) {
        case Direction::UP:
            tempPos.y--;
            break;
        case Direction::DOWN:
            tempPos.y++;
            break;
        case Direction::LEFT:
            tempPos.x--;
            break;
        case Direction::RIGHT:
            tempPos.x++;
            break;
        case Direction::STOP:
            break;
        default:
            break;
    }

    // check if this new position is already part of the list
    if (!this->grow) currentPos.pop_back();
    else this->grow = false;
    currentPos.push_front(tempPos);
}

void GameSnake::updateGame() {
    // check boundary condition
    Pos tempPos = (*this->currentPos.begin()); // get the head
    if (tempPos.x >= this->width || tempPos.x < 0 
    || tempPos.y >= this->height || tempPos.y < 0) {
        this->gameOver = true;
        return;
    }

    // check if the head hits any of the body of the snake
    this->currentPos.pop_front();
    if (this->_checkPosInList(tempPos)) {
        this->gameOver = true;
        return;
    }
    this->currentPos.push_front(tempPos);

    // update score
    if (tempPos == this->fruitPos) {
        this->score += kScoreIncrement;
        this->_generate();
        this->largestTimer -= 5000;
        this->grow = true;
    }
}

void GameSnake::run() {
    while (true) {
        this->draw();
        if (this->gameOver) break;
        this->input();
        this->updateGame();
        this->updatePosition();
        usleep(this->largestTimer);
    }
}

bool GameSnake::gameComplete() {
    return false;
}