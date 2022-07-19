//class.hpp

#ifndef CLASS_HPP
#define CLASS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

//File
//Menu
//Game

//--------------------------------------------
// File 
//--------------------------------------------
struct Files {
    
  public:
    const std::string MENU = "Menu.txt";
    const std::string WORD = "Word.txt";
    const std::string THEME = "Theme.txt";
    const int FIRST_WORD_INDEX = 1;
    
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
    int status, tries;
    std::string theme, word;

    Game();
    void gameLoop();
    std::string chooseAWord(std::string theme = "random", std::string word = "random");
    ~Game();
};

#endif



