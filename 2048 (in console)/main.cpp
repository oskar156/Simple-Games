/* 2048 (in console)

Notes

TOC
STRUCT/CLASS PROTOTYPES
FUNCTION PROTOTYPES
STRUCT/CLASS DEFINITIONS
FUNCTION DEFINITIONS
MAIN
*/

#include <iostream> //cout, cin
#include <cstdlib> //rand, srand
#include <vector> //vector
using namespace std;

//STRUCT/CLASS PROTOTYPES
struct Game;

//FUNCTION PROTOTYPES
int askUserForMenuChoice();
void createEmptyBoard(vector< vector< int >> &board, int BoardSizeX, int BoardSizeY);
void fillBoard(vector< vector< int >> &board, Game game, int moves);
int weightedRandomChoice_twoInts(int num1, int num2, int num1Weight, int num2Weight);
int randomNumberInRange(int low, int high);
int power(int num, int exponent);
void showBoard(vector< vector< int >> &board, Game game);
char askUserForMove(vector< vector< int >> &board, Game game, vector< bool > &moveValidity);
void executeMove(vector< vector< int >> &board, char move, Game &game, bool countScore);
void findValidMoves(vector< vector< int >> &board, Game game, vector< bool > &moveValidity);
int updateGameStatus(vector< vector< int >> &board, Game game, vector< bool > &moveValidity);
int checkWinStatus(vector< vector< int >> &board, Game game);  

//////////////////////////////////////////////////////////////////////////////////////////////////////
// STRUCT/CLASS DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////
struct Game {
    
  public:

    int BaseNumber = 2;
    int WinningMultiple = 11; //2048
    int MinNewNumberQuantity = 1;
    int MinNewNumberMultiple = 1;   
    int MaxNewNumberQuantity = 2;
    int MaxNewNumberMultiple = 2;
    int BoardSizeX = 4;
    int BoardSizeY = 4;
    
    int BaseNumber_min = 2;
    int BaseNumber_max = 5;
    int WinningMultiple_min = 2;
    int WinningMultiple_max = 10;
    int BoardSizeX_min = 2;
    int BoardSizeX_max = 20;
    int BoardSizeY_min = 2;
    int BoardSizeY_max = 20;
    
    vector< char > moves = {'w', 'a', 's', 'd'};
    int score = 0;
    
    void updateGameScore(int score_input) {score += score_input;}
    int getGameScore() {return score;}
    void showGameScore() {cout << "Score: " << getGameScore() << endl;}
    
    void showGameSettings() {
        
        cout << "Combine the numbers until you reach " << power(BaseNumber, WinningMultiple) << ", then keep playing to increase your score!" << endl
             << "Enter w a s d (UP DOWN LEFT RIGHT,respectively) to move the numbers." << endl
             << "Enter q to quit to the menu." << endl;
    }
    
    void customGameWizard() {
        
        cout << endl << "Custom Game Wizard" << endl << endl;
        
        int input;
        
        do {
            
            cout << "Choose the Base Number (current value is " << BaseNumber << "): ";
            cin >> input;
            BaseNumber = input;
            
            if(!(BaseNumber >= BaseNumber_min && BaseNumber <= BaseNumber_max))
                cout <<"Invalid! Enter an integer between " << BaseNumber_min << " and " << BaseNumber_max << endl;
                
        } while(!(BaseNumber >= BaseNumber_min && BaseNumber <= BaseNumber_max));
        
        do {
            
            cout << "Choose the Winning Multiple Number (current value is " << WinningMultiple << " - which will be displayed as " << power(BaseNumber, WinningMultiple) << " in game): ";
            cin >> input;
            WinningMultiple = input;

            if(!(WinningMultiple >= WinningMultiple_min && WinningMultiple <= WinningMultiple_max))
                cout <<"Invalid! Enter an integer between " << WinningMultiple_min << " and " << WinningMultiple_max << endl;
                
        } while(!(WinningMultiple >= WinningMultiple_min && WinningMultiple <= WinningMultiple_max));
        cout << "(Will be displayed as " << power(BaseNumber, WinningMultiple) <<" in game)" << endl;
        
        do {
            cout << "Choose the Board's Size (X) (current value is " << BoardSizeX << "): ";
            cin >> input;
            BoardSizeX = input;
            
            if(!(BoardSizeX >= BoardSizeX_min && BoardSizeX <= BoardSizeX_max))
                cout <<"Invalid! Enter an integer between " << BoardSizeX_min << " and " << BoardSizeX_max << endl;
                
        } while(!(BoardSizeX >= BoardSizeX_min && BoardSizeX <= BoardSizeX_max));
        
        do {
            cout << "Choose the Board's Size (Y) (current value is " << BoardSizeY << "): ";
            cin >> input;
            BoardSizeY = input;
            
            if(!(BoardSizeY >= BoardSizeY_min && BoardSizeY <= BoardSizeY_max))
                cout <<"Invalid! Enter an integer between " << BoardSizeY_min << " and " << BoardSizeY_max << endl;
                
        } while(!(BoardSizeY >= BoardSizeY_min && BoardSizeY <= BoardSizeY_max));
        
        cout << "Wizard complete, custom game starting... " << endl << endl;
    }

};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// FUNCTION DEFINITIONS
//////////////////////////////////////////////////////////////////////////////////////////////////////

int askUserForMenuChoice() {

    cout << "Enter 1 to start a normal game." << endl
         << "Enter 2 to define then start a custom game." << endl
         << "Enter 0 to quit." << endl;
         
    int menuChoice;
    cin >> menuChoice;
    return menuChoice;
}

void createEmptyBoard(vector< vector< int >> &board, int BoardSizeX, int BoardSizeY) {

    for(int x = 0; x < BoardSizeX; x++) {
        
        board.push_back({});
        
        for(int y = 0; y < BoardSizeY; y++) {
            
            board[x].push_back(0);
        }
    }
}

void fillBoard(vector< vector< int >> &board, Game game, int moves) {
    
    int newNumberQuantity = 2;
    if(moves >= 1)
        newNumberQuantity = weightedRandomChoice_twoInts(game.MinNewNumberQuantity, game.MaxNewNumberQuantity, 9, 1);
    //there needs to be an empty spot counter and the below loop needs to take into account
    for(int i = 0; i < newNumberQuantity /*&& i <= emptyCells*/; i++) {
        
        int newNumberMultiple = weightedRandomChoice_twoInts(game.MinNewNumberMultiple, game.MaxNewNumberMultiple, 9, 1);
        
        int xCoord, yCoord;

        do {
        
            xCoord = randomNumberInRange(0, board.size() - 1);
            yCoord = randomNumberInRange(0, board[0].size() - 1);   

        } while(board[xCoord][yCoord] != 0);
        
        board[xCoord][yCoord] = newNumberMultiple;
    }
}

int weightedRandomChoice_twoInts(int num1, int num2, int num1Weight, int num2Weight) {

    int random = (rand() % (num1Weight + num2Weight + 1)) + 0;
    int choice = num1;
    if(random > num1Weight)
        choice = num2;
        
    return choice;
}

int randomNumberInRange(int low, int high) {
    
    return (rand() % (low + high+ 1)) + 0;
}

int power(int num, int exponent) {
    
    int result = num;
    int i = 1;
    while(i < exponent) {
        
        result *= num;
        i++;
    }
    
    return result;
}

void showBoard(vector< vector< int >> &board, Game game) {

    cout << "-------------------------------------" << endl;
    for(int x = board.size() - 1; x >= 0; x--) {
        for(int y = 0; y < board[x].size(); y++) {

            int multiple = board[x][y];
            int value = 0;

            if(multiple > 0)
                value = power(game.BaseNumber, multiple);
                
            cout << value;
            
            if(value < 10) cout << "    ";
            else if(value < 100) cout << "   ";  
            else if(value < 1000) cout << "  ";  
            else if(value < 10000) cout << " ";
        }
        
        cout << endl << endl;
    }
}

char askUserForMove(vector< vector< int >> &board, Game game, vector< bool > &moveValidity) {
   
    char move = ' ';
    bool again = true;
    
    cout << "Make a move: ";

    do {

        cin >> move;

        if(move == 'W' || move == 'A' || move == 'S' || move == 'D' || 
           move == 'w' || move == 'a' || move == 's' || move == 'd')
            again = false;
        
        if(move == 'Q' || move == 'q') {
            
            cout << "Quitting to Menu... " << endl;
            return move;
        }
        
        int index;
        for(int m = 0; m < moveValidity.size(); m++) {
            
            if(game.moves[m] == move)
                index = m;
        }

        if(moveValidity[index] == false)
            again = true;

        if(again == true)
            cout << "Invalid move, please try again: ";

        
    } while(again == true);

   return move;
}

void executeMove(vector< vector< int >> &board, char move, Game &game, bool countScore) { //game is temp
    
    int directionX = 0;
    int directionY = 0;
    int iterationX = 1;
    int iterationY = 1;
    int edgeStartX = 0;
    int edgeStartY = 0;
    int edgeEndX = board.size();
    int edgeEndY = board[0].size();
    
    if(move == 'W' || move == 'w') {
        
        directionX = 1;
        iterationX = -1;
        edgeStartX = board.size() - 1;
        edgeEndX = -1;
    }
    else if(move == 'S' || move == 's') {
        
        directionX = -1;
        iterationX = 1;
        edgeStartX = 0;
        edgeEndX = board.size();
    }
    else if(move == 'D' || move == 'd') {
    
        directionY = 1;
        iterationY = -1;
        edgeStartY = board[0].size(); - 1;
        edgeEndY = -1;
    }
    else if(move == 'A' || move == 'a') {

        directionY = -1;
        iterationY = 1;
        edgeStartY = 0;
        edgeEndY = board[0].size();;
    }
    
    for(int x = edgeStartX; x != edgeEndX; x += iterationX) {
        for(int y = edgeStartY; y != edgeEndY; y += iterationY) {

            bool blocked = false;
            
            int currX = x;
            int currY = y;
            int ix = 1;
            int iy = 1;

            int combos = 0;
            while(currX + (ix * directionX) >= 0 && currX + (ix * directionX) < board.size() && 
                  currY + (iy * directionY) >= 0 && currY + (iy * directionY) < board[x].size() &&
                  (board[currX + (ix * directionX)][currY + (iy * directionY)] == 0 || blocked == false)) {

                if(board[currX + (ix * directionX)][currY + (iy * directionY)] == 0) {

                    board[currX + (ix * directionX)][currY + (iy * directionY)] = board[currX][currY];
                    board[currX][currY] = 0;
                }
                else if(blocked == false &&
                        board[currX + (ix * directionX)][currY + (iy * directionY)] == board[currX][currY]) {
                            
                    board[currX + (ix * directionX)][currY + (iy * directionY)] += 1;
                    board[currX][currY] = 0; 

                    if(countScore == true)
                        game.updateGameScore(100 * board[currX + (ix * directionX)][currY + (iy * directionY)]);

                    combos++;
                }

                currX += (ix * directionX);
                currY += (iy * directionY);             

                if(board[currX][currY] != 0)
                    blocked = true; 

                if(currX + (ix * directionX) >= 0 && currX + (ix * directionX) < board.size() &&
                   currY + (iy * directionY) >= 0 && currY + (iy * directionY) < board[0].size() &&
                   board[currX][currY] == board[currX + (ix * directionX)][currY + (iy * directionY)] && 
                   combos == 0)
                    blocked = false;
                else
                    blocked = true;
            }
        }
    }
}

void findValidMoves(vector< vector< int >> &board, Game game, vector< bool > &moveValidity) {

    for(int m = 0; m < moveValidity.size(); m++) {

        char move = game.moves[m];
        vector< vector< int >> hypoBoard = board;
        executeMove(hypoBoard, move, game, false);
            
        bool noChange = true;
        for(int x = 0; x < board.size(); x++) {
            for(int y = 0; y < board[x].size(); y++) {  
                    
                if(hypoBoard[x][y] != board[x][y]) {
                        
                    noChange = false;
                    break;
                }
            }
                
            if(noChange == false)
                break;
        }

        if(noChange == true)
            moveValidity[m] = false;
    }
}

int updateGameStatus(vector< vector< int >> &board, Game game, vector< bool > &moveValidity) {
    int gameStatus = 0;
    bool noValidMoves = true;
    for(int m = 0; m < moveValidity.size(); m++) {
        if(moveValidity[m] == true)
            noValidMoves = false;
    }
    if(noValidMoves == false)
        gameStatus = 0;
    else if(noValidMoves == true)
        gameStatus = -1;

    return gameStatus;
}

int checkWinStatus(vector< vector< int >> &board, Game game) {
 
    int winStatus = 0;
    for(int x = 0; x < board.size(); x++) {
        for(int y = 0; y < board[x].size(); y++) {    
            
            if(board[x][y] == game.WinningMultiple) {
                
                winStatus = 1;
                break;
            }
        }
        if(winStatus != 0)
            break;
    }   
    
    return winStatus;
}


//////////////////////////////////////////////////////////////////////////////////////////////////////
// MAIN
//////////////////////////////////////////////////////////////////////////////////////////////////////
int main() {
    
    cout << "2048 Copy" << endl << endl;
    srand((unsigned)time(0)); //seed random number generator
    int menuChoice = askUserForMenuChoice(); //1 means start, 0 means quit

    while(menuChoice != 0) {

        Game game; //game parameters
        
        if(menuChoice == 2) //define a custom game if the user wants to
            game.customGameWizard();
            
        int gameStatus = 0; //-1 means loss, 0 means in-progresss
        vector< vector< int >> board;
        createEmptyBoard(board, game.BoardSizeX, game.BoardSizeY);
        int moves = 0; //keeps track of the # of moves
        int movesPostWin = 0;
        int keepPlaying = 1;
        
        while(gameStatus >= 0 && keepPlaying == 1) { //while the user hasn't lost
            
            game.showGameSettings();
            fillBoard(board, game, moves); //add a new values to the board
            showBoard(board, game);
            game.showGameScore();
            
            vector< bool > moveValidity = {true, true, true, true};
            findValidMoves(board, game, moveValidity); //updates moveValidity by reference and checks if a move in each direction {UP LEFT DOWN RIGHT} is valid
            int gameStatus = updateGameStatus(board, game, moveValidity);
            int winStatus = checkWinStatus(board, game);
            
            if(movesPostWin == 0 && winStatus == 1) {
                
                if(movesPostWin == 0) {
                    
                    cout << "You won!" << endl;
                    cout << "Enter 1 to Keep Playing." << endl
                         << "Enter 0 to Return to the Menu." << endl;
                         
                    cin >> keepPlaying;
                    
                    if(keepPlaying == 1)
                        showBoard(board, game);
                }
                
                movesPostWin++;
            }
            
            if(gameStatus == 0 && keepPlaying != 0) {
                
                char move = askUserForMove(board, game, moveValidity);
                
                if(move != 'q' && move != 'Q') {
                    executeMove(board, move, game, true);
                    moves++;
                }
                else
                    keepPlaying = 0;
            }
        }

        menuChoice = askUserForMenuChoice();
    }
}
