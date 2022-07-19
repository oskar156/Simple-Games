//class.hpp

#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <cstdlib>
#include <chrono>

#include "function.hpp"

//File
//Menu
//Game

//--------------------------------------------
// File 
//--------------------------------------------
struct Files {
    
  public:
        const std::string MENU = "Menu.txt";
        const std::string GAME = "Game.txt";
        const std::string SAVE = "Save.txt";
};

//--------------------------------------------
// Menu 
//--------------------------------------------
struct Menu {

  public:
    int choice, lowerBound;
    std::string title;
    std::vector< std::string > options;

    Menu(std::string fileName, int lowerBound_input = 0);
    void askUserForMenuChoice();
};

//--------------------------------------------
// Game 
//--------------------------------------------
struct Game {

  public:
    const int MIN_BOARD_SIZE_X = 1;
    const int MAX_BOARD_SIZE_X = 102;
    const int MIN_BOARD_SIZE_Y = 1;
    const int MAX_BOARD_SIZE_Y = 51;
    const int MIN_MINE_AMOUNT = 1;
    const int DEFAULT_SAFE_RADIUS_X = 1;
    const int DEFAULT_SAFE_RADIUS_Y = 1;
    const int DEFAULT_LENIENCY = 3;

    int sizeX, sizeY, maxMines, status, moves, moveX, moveY, safeRadiusX, safeRadiusY, maxFlagCount, remainingMines;
    std::string gameTypeName;
    std::vector< std::vector< int >> board; //0 is empty, 2 is a mine
    std::vector< std::vector< int >> boardFog; //0 is hidden, 1 is exposed, 2 is exposed mine

    Game();
    
    void gameLoop();
    
    void createFromFile(std::string gameTypeName_input);
    void createFromWizard(bool saveGameType, std::string action);
    
    void createEmptyBoard();
    void fillBoardWithMines(bool safeZone = true);
    void fillBoardWithWarningNumbers();
    void resetBoardFog();
    void updateBoardFog(int x, int y, int leniency);
    
    void showBoardSimple();
    void showBoard();
    void showBoardFog();

    std::vector< std::vector< int >> findSafeCoordinates();
    void askUserForMoveX();
    void askUserForMoveY();
    void askUserForFlagCoordinates();

    int countRemainingMines();
    int checkMoveDestinationStatus();
    bool checkIfMoveIsInBound();
    void checkIfGameIsOver();

    void saveGameTypeToFile();
    void saveGame();
    void loadSavedGame(Game &game, std::string saveGameName);
};
#endif


