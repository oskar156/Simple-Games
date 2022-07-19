//class.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <cstdlib>

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
    std::string Game::chooseAWord(std::string theme_input, std::string word_input) {

        std::string chosenWord, chosenTheme;
        Files files;
        std::string fileContents = getFileContent(files.WORD);
        std::vector< std::vector< std::string >> parsedFileContents = parseFileContents(fileContents);
        int themeIndex, wordIndex;

        if(theme_input == "random") {
            
            themeIndex = rand() % (parsedFileContents.size()) + 0;
            chosenTheme = findThemeFromFileLine(parsedFileContents[themeIndex][0]);
        }
        else {
            
            //showListOfAllThemes();
            //std::string input = askUserForSelection(allThemes) input validation in here
        }

        if(theme_input != "cancel" && word_input != "cancel") {
            
            if(word_input == "random") {
                
                wordIndex = rand() % (parsedFileContents[themeIndex].size()) + files.FIRST_WORD_INDEX;
                chosenWord = parsedFileContents[themeIndex][wordIndex];
            }
            else {
                
                //showListOfAllWords(std::string theme);
                //std::string input = askUserForSelection(allWordsInATheme) input validation in here
            }
        }

        Game::word = chosenWord;
        Game::theme = chosenTheme;
        
        return chosenWord;
    }

    void Game::gameLoop() {

        std::string wordHidden = Game::word;
        for(int c = 0; c < word.size(); c++) {
            
            if(word[c] != '-' && word[c] != ' ')
                wordHidden[c] = '_';
        }
        
        Menu charGuess("charGuess");
        
        while(Game::status != 0) {
            
            std::cout << wordHidden << " " << Game::word << std::endl;

            if(wordHidden == Game::word) {
                Game::status = 0;
            }
            else if(Game::tries <= 0) {
                Game::status = 0;
            }
            
            if(Game::status != 0) {
                
                //SHOW THE THEME as a hint
                showHiddenWord(wordHidden); //showWordHIdden
                char userGuess = askUserForChar(charGuess.title);//askuserforinput
                //add userGuess to a vector of already guessed chars and prevent user from slectig these again
                bool correctGuess = updateHiddenWord(wordHidden, Game::word, userGuess); //passs wordHidden by reference
            
                if(correctGuess == false)
                    Game::tries -= 1;
                std::cout << correctGuess << "TRIES LEFT " << Game::tries;
            }
        }
    }
    Game::Game() {

        Game::status = 1;
        Game::tries = 3;
        //Game::word = "random";
        //Game::theme = "random";
        
    }
    
    Game::~Game() {};




