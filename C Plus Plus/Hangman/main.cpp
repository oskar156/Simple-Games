//hangman main.cpp
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

#include "function.hpp"
#include "class.hpp"

int main() {

    srand((unsigned)time(0)); //seed rand
    Files files;

    std::cout << "Hangman" << std::endl <<std::endl;

    Menu mainMenu("mainMenu");
    
    while(mainMenu.choice != 0) {

        mainMenu.askUserForMenuChoice();
        std::cout << "gamegmae" <<std::endl;
        Game game;
        
        if(mainMenu.choice == 1) { //QUICKPLAY
        std::cout << "mainMenu.choice1" <<std::endl;
            game.chooseAWord();
        }
        /*else if(mainMenu.choice == 2) { //CHOOSE

            Menu chooseMenu("playMenu");
            while(chooseMenu.choice != 0) {

                chooseMenu.askUserForMenuChoice();

                if(chooseMenu.choice == 1) { //choose by theme

                    Menu themeMenu("themeMenu");
                    while(themeMenu.choice != 0) {

                        themeMenu.askUserForMenuChoice();
                        if(themeMenu.choice != 0) {

                            //choose theme by index
                        }
                    }
                }
                else if(chooseMenu.choice == 2) { //choose wizard
                }
            }
        }*/

        if(mainMenu.choice != 0 && game.status != 0) {
        std::cout << "game.create();" <<std::endl;

            game.gameLoop();
        }
        game.~Game();
    }

    std::cout << "GoodBye" << std::endl;

    return 0;
}
