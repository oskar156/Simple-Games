//class.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <cstdlib>
#include <ctime>

#include "function.hpp"
#include "class.hpp"

//File
//Menu
//Game

//--------------------------------------------
// File
//--------------------------------------------

//--------------------------------------------
// Menu 
//--------------------------------------------
    Menu::Menu(std::string menuName, int lowerBound_input) {

        Menu::choice = -100;
        Menu::lowerBound = lowerBound_input;
        
        Files files;
        std::string fileContents = getFileContent(files.MENU);
        std::vector< std::vector< std::string >> parsedFileContents = parseFileContents(fileContents);

        for(int a = 0; a < parsedFileContents.size(); a++) {
            
            if(parsedFileContents[a][0] == menuName) {
                
                for(int b = 1; b < parsedFileContents[a].size(); b++) { //b = 1 so we skip the menu name
                    
                    if(parsedFileContents[a][b].substr(0, 8) == "title - ")
                        Menu::title = parsedFileContents[a][b].substr(8);
                    else
                        Menu::options.push_back(parsedFileContents[a][b]);
                }
            }
        }
    }

    void Menu::askUserForMenuChoice() {

        std::cout << title << std::endl;

        for(int i = 0; i < Menu::options.size(); i++)
            std::cout << Menu::options[i] << std::endl;

        int menuChoice = -1;
        while(menuChoice < Menu::lowerBound || menuChoice > Menu::options.size() - 1) {

            std::cin >> menuChoice;

            if(menuChoice < Menu::lowerBound  || menuChoice > Menu::options.size() - 1)
                std::cout << "Invalid Entry. Please enter an integer between " << lowerBound << " and " << options.size() - 1 << " (inclusive)." << std::endl;
        }

        Menu::choice = menuChoice;
        std::cout << std::endl;
    }

//--------------------------------------------
// Game 
//--------------------------------------------
    Game::Game() {

        Game::status = 1; //0 means gameover loss, 1 is ongoing, 2 is gameover win 
        Game::moves = 0;
    }
    
    void Game::gameLoop() {

        Menu gameMenu("gameMenu");
        std::cout << "Remaining Mines: " << Game::countRemainingMines() << std::endl;
        Game::showBoardFog();
        std::cout << "Play Controls: " << std::endl 
                  << "Enter 0 to Open the Menu." << std::endl
                  << "Enter -1 to Place a Flag." << std::endl            
                  << "Enter Valid Coordinates to reveal a cell." << std::endl
                  << std::endl;
            
        while(gameMenu.choice != 2 && Game::status != 0) { //while the user doesn't want to quit and the game isnt over...

            bool moveIsValid = true;

            do {
                
                moveIsValid = true;

                Game::askUserForMoveX();
                if(Game::moveY >= 0) //user enters 0 to get -1. this is to account for 1/0-index translation
                    Game::askUserForMoveY();

                if(Game::moveY == -2) { //if the user enters -1, then we run the flag function

                    Game::askUserForFlagCoordinates();
                    moveIsValid = false;
                }
                else if(Game::moveX == -1 || Game::moveY == -1) { //if the user enters 0 in either x or y...
                    gameMenu.askUserForMenuChoice(); //then it will pull up the menu
                    
                    if(gameMenu.choice == 1) { //SAVE GAME
                        
                        std::cout << "SAVE GAME" << std::endl;
                        Game::saveGame();
                    }
                    moveIsValid = false;
                }

                else {

                    if(Game::moves == 0) {//if it's the first move

                        Game::fillBoardWithMines(); //then fill the board with mines with a safe-zone around the move
                        Game::fillBoardWithWarningNumbers();
                    }

                    //checkIfMoveIsValid
                    bool moveIsInBound = Game::checkIfMoveIsInBound(); //check if greater 0 or greater and less than sizeX and SizeY
                    if(moveIsInBound == false) {

                        std::cout << "Invalid move (out-of-bounds). Please try again." << std::endl;
                        moveIsValid = false;
                    }

                    if(moveIsInBound == true) {
                    
                        if(Game::boardFog[moveX][moveY] == 1) { //if it's already been revealed

                            std::cout << "Invalid move (already revealed). Please try again." << std::endl;
                            moveIsValid = false;
                        }
                        else if(Game::boardFog[moveX][moveY] == -2) { //selecting a flagged cell

                            std::cout << "You've flagged this cell. To remove the flag, please enter -1 in the next input and enter the coordinates again." << std::endl;
                            moveIsValid = false;
                        }                      
                        else if(Game::board[moveX][moveY] == -1) { //if you hit a mine

                            std::cout << "You hit a mine! You Lost." << std::endl
                                      << std::endl;
                            Game::status = 0; //game over, you lost
                        }
                    }
                }
            } while(moveIsValid == false && gameMenu.choice != 2 && Game::status != 0);

            Game::updateBoardFog(Game::moveX, Game::moveY, 0);
            std::cout << "Remaining Mines: " << Game::countRemainingMines() << std::endl;
            Game::showBoardFog();
            Game::checkIfGameIsOver();
            Game::moves += 1;
        }
    }
    
    void Game::createFromFile(std::string gameTypeName_input) {
        
        Files files;
        int gameTypeIndex = 0;
        std::string fileName = files.GAME;

        std::string fileContents = getFileContent(fileName);
        std::vector< std::vector< std::string >> parsedFileContents = parseFileContents(fileContents);

        int a = 0;
        while(parsedFileContents[a][0] != gameTypeName_input)
            a++;
        gameTypeIndex = a;
        
        Game::gameTypeName = parsedFileContents[a][0];
        Game::sizeX = stoi(parsedFileContents[gameTypeIndex][1]);
        Game::sizeY = stoi(parsedFileContents[gameTypeIndex][2]);
        Game::maxMines = stoi(parsedFileContents[gameTypeIndex][3]);
        Game::safeRadiusX = stoi(parsedFileContents[gameTypeIndex][4]);
        Game::safeRadiusY = stoi(parsedFileContents[gameTypeIndex][5]);
        Game::remainingMines = maxMines;
        
        Game::createEmptyBoard();
        Game::resetBoardFog();
    }

    void Game::createFromWizard(bool saveGameType, std::string action) {
        
        if(action == "create")
            std::cout << "Create a Game Type Controls: " << std::endl;
        else if(action == "edit")
            std::cout << "Edit a Game Type Controls: " << std::endl;
            
        std::cout << "Enter -1 at anytime to Cancel." << std::endl
                  << "Follow the instructions." << std::endl
                  << std::endl;

        int input;
        
        do { 

            std::cout << "Enter Size (X)";
            if(action == "edit")
                std::cout << " (current value is " << Game::sizeX << ")";
            std::cout << ": ";
            std::cin >> input;

            if((input < Game::MIN_BOARD_SIZE_X || input > Game::MAX_BOARD_SIZE_X) && (input != -1))
                std::cout << "Invalid entry. Size (X) must be between " << Game::MIN_BOARD_SIZE_X << " and " << Game::MAX_BOARD_SIZE_X << " (inclusive)." << std::endl;
            else if(input != -1)
                Game::sizeX = input;

        } while((input < Game::MIN_BOARD_SIZE_X || input > Game::MAX_BOARD_SIZE_X) && (input != -1));

        do { 

            if(input != -1) {
                
                std::cout << "Enter Size (Y)";
                if(action == "edit")
                    std::cout << " (current value is " << Game::sizeY << ")";
                std::cout << ": ";
                std::cin >> input;
            
                if((input < Game::MIN_BOARD_SIZE_Y || input > Game::MAX_BOARD_SIZE_Y) && (input != -1))
                    std::cout << "Invalid entry. Size (Y) must be between " << Game::MIN_BOARD_SIZE_Y << " and " << Game::MAX_BOARD_SIZE_Y << " (inclusive)." << std::endl;
                else if(input != -1)
                    Game::sizeY = input;
            }
            
        } while((input < Game::MIN_BOARD_SIZE_Y || input > Game::MAX_BOARD_SIZE_Y) && (input != -1));
        
        const int MAX_MINE_AMOUNT = sizeX * sizeY;

        do { 

            if(input != -1) {
                
                std::cout << "Enter Amount of Mines";
                if(action == "edit")
                    std::cout << " (current value is " << Game::maxMines << ")";
                std::cout << ": ";
                std::cin >> input;

                if((input < Game::MIN_MINE_AMOUNT || input > MAX_MINE_AMOUNT) && (input != -1))
                    std::cout << "Invalid entry. There must be between " << Game::MIN_MINE_AMOUNT << " and " << MAX_MINE_AMOUNT << " mines (inclusive)." << std::endl;
                else if(input != -1)
                    Game::maxMines = input;
            }
            
        } while((input < Game::MIN_MINE_AMOUNT || input > MAX_MINE_AMOUNT) && (input != -1));

        Game::safeRadiusX = Game::DEFAULT_SAFE_RADIUS_X;
        Game::safeRadiusY = Game::DEFAULT_SAFE_RADIUS_Y;
        Game::remainingMines = Game::maxMines;

        if(input == -1) {
            
            if(action == "create")
                std::cout << "Game Type Creation canceled. Returned to Menu." << std::endl 
                          << std::endl;
            else if(action == "edit")
                std::cout << "Game Type Edit canceled. Returned to Menu." << std::endl 
                          << std::endl;
            Game::status = 0;
        }
        else if(input != -1) {
            
            Game::createEmptyBoard();
            Game::resetBoardFog();
        }
        if(input != -1 && saveGameType == true) {

            Game::saveGameTypeToFile();
        }
    }
    
    void Game::createEmptyBoard() {

        for(int x = 0; x < Game::sizeX; x++) {

            Game::board.push_back({});

            for(int y = 0; y < Game::sizeY; y++) {

                Game::board[x].push_back(0);
            }
        }
    }
    
    void Game::fillBoardWithMines(bool safeZone) {

        std::vector< std::vector< int >> safeCoordinates;
        if(safeZone == true)
            safeCoordinates = findSafeCoordinates();

        int mines = 0;

        while(mines <= Game::maxMines) {

            for(int x = 0; x < Game::sizeX; x++) {
               for(int y = 0; y < Game::sizeY; y++) {

                    bool skipBecauseSafeZone = false;
                    for(int s = 0; s < safeCoordinates.size(); s++) {
                        
                        if(x == safeCoordinates[s][0] && y == safeCoordinates[s][1]) {
                            
                            skipBecauseSafeZone = true;
                            break;
                        }
                    }

                    if(skipBecauseSafeZone == false) {
                        
                        int random = rand() % (Game::sizeX * Game::sizeY - 1) + 0;
                      
                        if(mines <= Game::maxMines && random < Game::maxMines && Game::board[x][y] != -1) {
             
                            Game::board[x][y] = -1;
                            mines++;
                        }
                    }
                }
            }
        }
    }

    void Game::fillBoardWithWarningNumbers() {

        for(int x = 0; x < Game::sizeX; x++) {
            for(int y = 0; y < Game::sizeY; y++) {

                int mines = 0;
                for(int xx = -1; xx <= 1; xx++) {
                    for(int yy = -1; yy <= 1; yy++) {
                        
                        if(x + xx >= 0 && y + yy >= 0 && x + xx < Game::sizeX && y + yy < Game::sizeY && //if it's inbounds
                           Game::board[x][y] != -1 && //if it's not a mine
                           Game::board[x + xx][y + yy] == -1) //and if it's adjacent to a mine, 
                            mines += 1; //then count it
                    }
                }
                
                if(Game::board[x][y] != -1)
                    Game::board[x][y] = mines;
            }
        }
    }
    
    void Game::resetBoardFog() {
        
        for(int x = 0; x < Game::sizeX; x++) {

            Game:: boardFog.push_back({});

            for(int y = 0; y < Game::sizeY; y++) {

                Game::boardFog[x].push_back(0);
            }
        }
    }

    void Game::updateBoardFog(int x, int y, int leniency) {

        //Base cases 
        if(x < 0 || x >= Game::sizeX || y < 0 || y >= Game::sizeY) //if out of bounds
            return; 
        if(Game::board[x][y] == -1) //if we encounter a mine
            return; 
        if(Game::boardFog[x][y] == 1) //if it's already been revealed
            return; 
        if(leniency == Game::DEFAULT_LENIENCY) //if we've come across 3 mine-adjacent cells
            return;
        
        if(Game::board[x][y] != 0)
            leniency += 1;
        
        Game::boardFog[x][y] = 1; 

        //Recursively call for north, east, south and west 
        Game::updateBoardFog(x + 1, y, leniency); 
        Game::updateBoardFog(x - 1, y, leniency); 
        Game::updateBoardFog(x, y + 1, leniency); 
        Game::updateBoardFog(x, y - 1, leniency);
        Game::updateBoardFog(x + 1, y + 1, leniency); 
        Game::updateBoardFog(x - 1, y + 1, leniency); 
        Game::updateBoardFog(x + 1, y - 1, leniency); 
        Game::updateBoardFog(x - 1, y - 1, leniency);
    } 
    
    void Game::showBoardSimple() {

        for(int x = 0; x < Game::sizeX; x++) {
            for(int y = 0; y < Game::sizeY; y++) {

                std::cout << Game::board[x][y] << " ";
            }

            std::cout << std::endl;
        }
        std::cout << std::endl;
    }

    void Game::showBoard() {

        for(int x = Game::sizeX - 1; x >= 0 ; x--) {
            
            std::cout << x + 1;
            
            if(x + 1 >= 100) std::cout << "| ";           
            else if(x + 1 >= 10) std::cout << " | ";
            else if(x + 1 >= 0) std::cout << "  | ";           
            
            for(int y = 0; y < Game::sizeY; y++) {

                std::string symbol = ".";
                if(Game::board[x][y] == -1)
                    symbol = "M";
                else if(Game::board[x][y] >= 0)
                    symbol = std::to_string(Game::board[x][y]);
                std::cout << symbol << " ";
            }

            std::cout << std::endl;
        }
        
        std::cout << "     ";
        for(int y = 0; y < Game::sizeY; y++)
            std::cout << "- ";
        std::cout << std::endl;
        std::cout << "     ";
        for(int y = 0; y < Game::sizeY; y++) {
            
            std::cout << y + 1;
            if(y + 1 < 10)
                std::cout << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;     
    }

    void Game::showBoardFog() {

        for(int x = Game::sizeX - 1; x >= 0 ; x--) {
            
            std::cout << x + 1;
            
            if(x + 1 >= 100) std::cout << "| ";           
            else if(x + 1 >= 10) std::cout << " | ";
            else if(x + 1 >= 0) std::cout << "  | ";           
            
            for(int y = 0; y < Game::sizeY; y++) {

                std::string symbol = " ";
                if(Game::boardFog[x][y] == 0) symbol = "."; //hidden
                else if(Game::boardFog[x][y] == 1) { //exposed
                
                    symbol = " ";
                    int adjacentMines = Game::board[x][y];
                    if(adjacentMines == -1)
                        symbol = "M";
                    else if(adjacentMines >= 1)
                        symbol = std::to_string(adjacentMines);
                }
                else if(Game::boardFog[x][y] == -2) { //flagged
                
                    symbol = "F";
                }
                std::cout << symbol << " ";
            }

            std::cout << std::endl;
        }
        
        std::cout << " Y   ";
        for(int y = 0; y < Game::sizeY; y++)
            std::cout << "- ";
        std::cout << std::endl;
        std::cout << "   X ";
        for(int y = 0; y < Game::sizeY; y++) {

            if(y + 1 >= 100 && y % 3 == 0)
                std::cout << y + 1 << " ";
            else if(y + 1 >= 100 && y % 3 != 0)
                std::cout << " ";        
            else if(y + 1 >= 10 && (y + 1) % 2 == 0)
                std::cout << y + 1;
            else if(y + 1 >= 10 && (y + 1) % 2 != 0)
                std::cout << "  ";
            else if(y + 1 < 10)
                std::cout << y + 1 << " ";
        }
        std::cout << std::endl;
        std::cout << std::endl;
    }

    std::vector< std::vector< int >> Game::findSafeCoordinates() {

        std::vector< std::vector< int >> safeCoordinates;

        for(int x = moveX - safeRadiusX; x < moveX + Game::safeRadiusX; x++) {
            for(int y = moveY - safeRadiusY; y < moveY + Game::safeRadiusY; y++) {

                safeCoordinates.push_back({});
                safeCoordinates[safeCoordinates.size() - 1].push_back(x);
                safeCoordinates[safeCoordinates.size() - 1].push_back(y);
            }
        }

        return safeCoordinates;
    }

    void Game::askUserForMoveX() {
        
        std::cout << "Move (x): "; 
        std::cin >> Game::moveY;
        Game::moveY -= 1;
    }
    
    void Game::askUserForMoveY() {
        
        std::cout << "Move (y): ";
        std::cin >> Game::moveX;
        Game::moveX -= 1;
    }
    
    void Game::askUserForFlagCoordinates() {
        
        int flagX, flagY;
        do {
            
            std::cout << "Move Controls: " << std::endl 
                      << "Enter 0 to Cancel Flag Placement." << std::endl
                      << "Enter Valid Coordinates to place a Flag." << std::endl
                      << std::endl;
            
            std::cout << "Add/Remove a Flag: " << std::endl;
            std::cout << "Enter Flag Coordinates (x): ";
            std::cin >> flagY;
            if(flagY != 0) {

                std::cout << "Enter Flag Coordinates (y): ";
                std::cin >> flagX;
            }
        
            flagX -= 1; //0-index it
            flagY -= 1;
            
            if(flagX == -1 || flagY == -1) {
                
                std::cout << "Canceling Flag Placement..." << std::endl;
                return;
            }
            else if(flagX < 0 || flagX >= Game::sizeX || flagY < 0 || flagY >= Game::sizeY)
                std::cout << "Invalid flag placement (out of bounds), please try again." << std::endl;
            else if(Game::boardFog[flagX][flagY] == 1)
                std::cout << "Invalid flag placement (already exposed), please try again." << std::endl;   

        } while(flagX < 0 && flagX >= Game::sizeX && flagY < 0 && flagY >= Game::sizeY &&
                Game::boardFog[flagX][flagY] == 1);

        if(Game::boardFog[flagX][flagY] == -2) //if already flagged
            Game::boardFog[flagX][flagY] = 0;
        else if(Game::boardFog[flagX][flagY] != -2) //if not flagged
            Game::boardFog[flagX][flagY] = -2;
        std::cout << "Remaining Mines: " << Game::countRemainingMines() << std::endl;
        Game::showBoardFog();
    }
    
    int Game::countRemainingMines() {
        
        Game::remainingMines = Game::maxMines;
        for(int x = 0; x < Game::sizeX; x++) {
            for(int y = 0; y < Game::sizeY; y++) {
                
                if(Game::boardFog[x][y] == -2) { //if it's flagged then
                    
                    Game::remainingMines -= 1;
                }
            }
        }
        
        return remainingMines;
    }
    
    int Game::checkMoveDestinationStatus() {
        
        int moveDestinationStatus = 0;
        int revealStatus = Game::boardFog[moveX][moveY];
        int cellContents = Game::board[moveX][moveY];
        
        moveDestinationStatus = revealStatus;
        if(moveDestinationStatus != 1 && Game::board[moveX][moveY] == 2) //if it's not revealed
            moveDestinationStatus = cellContents; //then return 2 the fact that it's filled with a mine
            
        return moveDestinationStatus;
    }
    
    bool Game::checkIfMoveIsInBound() {
        
        bool moveIsInBound = true;
        if(moveX < 0 || moveX > Game::sizeX ||
           moveY < 0 || moveY > Game::sizeY)
            moveIsInBound = false;
            
        return moveIsInBound;
    }

    void Game::checkIfGameIsOver() {
        
        bool gameWon = true;
        for(int x = 0; x < Game::sizeX; x++) {
            for(int y = 0; y < Game::sizeY; y++) {
                
                if(Game::board[x][y] != -1 && Game::boardFog[x][y] == 0) {
                    
                    gameWon = false;
                    break;
                }
            }
        }
        
        if(gameWon == true) {
            
            status = 0;
            std::cout << "All mines found. You win!" << std::endl
                      << std::endl;
            Game::remainingMines = 0;
        }
    }
    
    void Game::saveGameTypeToFile() {
        
        Files files;
        std::string fileName = files.GAME;
        std::string gameTypeName_input;
        
        std::cout << "Name this custom game (no spaces): ";
        std::cin >> gameTypeName_input;
        //while nameIsValid = false or not canceled
        //bool fileWritingInputValidation(std::string input, std::vector< std::string > &names) 
        //get list of names
        
        Game::gameTypeName = gameTypeName_input;

        std::string line = Game::gameTypeName + ",";
        line += (std::to_string(Game::sizeX) + ",");
        line += (std::to_string(Game::sizeY) + ",");
        line += (std::to_string(Game::maxMines) + ",");
        line += (std::to_string(Game::safeRadiusX) + ",");
        line += (std::to_string(Game::safeRadiusY) + ",");

        appendLineToFile(fileName, line);
        std::cout << "Custom game " << Game::gameTypeName << " created." << std::endl;
    }

    void Game::saveGame() {

        Files files;
        std::string fileContent = getFileContent(files.SAVE);
        std::vector< std::string > lines;
        
        std::string saveGameName;
        std::cout << "Enter a name of saved game: ";
        std::cin >> saveGameName;
        //while nameIsValid = false or not canceled
        //bool fileWritingInputValidation(std::string input, std::vector< std::string > &names) 
        //get list of names
        
        auto start = std::chrono::system_clock::now();

        std::time_t thyme = std::time(0);   //FORMAT THE DATE AND TIME AND THEN DISPLAY IT!
        std::string dateAndTime = std::to_string(thyme);

        std::string line1 = saveGameName + ',';
        line1 += Game::gameTypeName + ',';       
        line1 += dateAndTime + ',';  
        line1 += std::to_string(Game::sizeX) + ',';
        line1 += std::to_string(Game::sizeY) + ',';
        line1 += std::to_string(Game::maxMines) + ',';
        line1 += std::to_string(Game::moves) + ',';

        std::string line2 = "[";
        for(int x = 0; x < Game::board.size(); x++) {
            
            line2 += "{";
            
            for(int y = 0; y < Game::board[x].size(); y++) {
                
                line2 += std::to_string(Game::board[x][y]);
                if(y < Game::board[x].size() - 1)
                    line2 += ",";
            }
            
            line2 += "}";
            if(x < Game::board.size() - 1)
                line2 += ",";
        }
        line2 += "]";
        
        std::string line3 = "[";
        for(int x = 0; x < Game::boardFog.size(); x++) {
            
            line3 += "{";
            
            for(int y = 0; y < Game::boardFog[x].size(); y++) {
                
                line3 += std::to_string(Game::boardFog[x][y]);
                if(y < Game::boardFog[x].size() - 1)
                    line3 += ",";
            }
            
            line3 += "}";
            if(x < Game::boardFog.size() - 1)
                line3 += ",";
        }
        line3 += "]";
        lines.push_back(line1);
        lines.push_back(line2);      
        lines.push_back(line3);     

        for(int a = 0; a < lines.size(); a++) {
            
            std::ofstream file(files.SAVE, std::ios::app);   
            if(fileContent.size() > 0)
                file << "\n";
            file << lines[a];
            file.flush();
            file.close();
        }
        
        std::cout << "Game " << saveGameName << " has been saved." << std::endl
                  << std::endl;
    }
    
    void Game::loadSavedGame(Game &game, std::string saveGameName) {

        Files files;
        std::string fileName = files.SAVE;
        std::string fileContents = getFileContent(fileName);
        std::vector< std::vector< std::string >> parsedFileContents = parseFileContents(fileContents);
        
        int i = 0;
        while(parsedFileContents[i][0] != saveGameName)
            i++;

        game.sizeX = std::stoi(parsedFileContents[i][3]);
        game.sizeY = std::stoi(parsedFileContents[i][4]);
        game.maxMines = std::stoi(parsedFileContents[i][5]);
        game.moves = std::stoi(parsedFileContents[i][6]);     

        std::vector< std::vector< std::string >> board_str = parseVectorFromString(parsedFileContents[i + 1][0]);
        std::vector< std::vector< std::string >> boardFog_str = parseVectorFromString(parsedFileContents[i + 2][0]);
        Game::createEmptyBoard();
        Game::resetBoardFog();

        for(int x = 0; x < board_str.size(); x++) {
            for(int y = 0; y < board_str[x].size(); y++) {   

                std::string board_strXY = board_str[x][y];
                std::string boardFog_strXY = boardFog_str[x][y];
                    
                if(board_strXY.size() > 0 && boardFog_strXY.size() > 0) {

                    Game::board[x][y] = std::stoi(board_strXY);
                    Game::boardFog[x][y] = std::stoi(boardFog_strXY);  
                }
            }
        } 
    }


