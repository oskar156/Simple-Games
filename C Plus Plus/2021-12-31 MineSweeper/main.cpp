//MineSweeper
#include <iostream> //cout, cin, endl
#include <vector> //vector
#include <string> //string
#include <fstream> //ifstream, ofstream
#include <cstdlib> //rand, srand, time
#include <chrono>

#include "function.hpp"
#include "class.hpp"

//add saved game and game type input validation (no duplicates allowed, no spaces and special chars allowed {}[],)

int main() {

    srand((unsigned)time(0)); //seed rand
    Files files;
    
    std::cout << "MineSweeper" << std::endl
              << std::endl;
              
    Menu mainMenu("mainMenu"); //constructor uses a string parameter to find a file and use its contents to fill vector< string > options

    while(mainMenu.choice != 0) { //MAIN MENU

        mainMenu.askUserForMenuChoice();

        if(mainMenu.choice == 1) { //PLAY

            Menu subMenuPlay("playMenu");
            while(subMenuPlay.choice != 0) {

                subMenuPlay.askUserForMenuChoice();
                Game game; //create game object, then...

                //...DETERMINE GAME TYPE
                if(subMenuPlay.choice == 1) { //STANDARD GAME

                    game.createFromFile("easy"); //need to add a not found exception handle
                }
                else if(subMenuPlay.choice == 2) { //CREATE CUSTOM GAME
                
                    bool saveGameType = false;
                    game.createFromWizard(saveGameType, "create");
                }
                else if(subMenuPlay.choice == 3) { //LOAD CUSTOM GAME

                    std::string gameType;
                    std::vector< std::vector< std::string >> gameTypes = showGameTypesFromFile(files.GAME);
                    gameType = askUserForGameTypeChoice_string(gameTypes, "load");
                    
                    if(gameType != "cancel")
                        game.createFromFile(gameType);
                    else if(gameType == "cancel")
                        game.status = 0;   
                }
                else if(subMenuPlay.choice == 4) { //LOAD SAVED GAME

                    std::string saveGameName;
                    std::vector< std::vector< std::string >> savedGames = showSavedGamesFromFile(files.SAVE);
                    saveGameName = askUserForSavedGameChoice_string(savedGames, "load");
                    
                    if(saveGameName != "cancel")
                        game.loadSavedGame(game, saveGameName);
                    else if(saveGameName == "cancel")
                        game.status = 0;
                }
                
                if(subMenuPlay.choice != 0 && game.status != 0) { //PLAY THE GAME

                    game.gameLoop();
                }
            }
        }
        else if(mainMenu.choice == 2) { //CREATE

            Menu subMenuCreate("createMenu");
            while(subMenuCreate.choice != 0) {

                subMenuCreate.askUserForMenuChoice();
                if(subMenuCreate.choice == 1) { //Create a Custom Game

                    Game game;
                    bool saveGameType = true;
                    game.createFromWizard(saveGameType, "create");
                }
                else if(subMenuCreate.choice == 2) { //Edit a Custom Game
                
                    std::string gameType;
                    std::vector< std::vector< std::string >> gameTypes = showGameTypesFromFile(files.GAME);
                    int indexToEdit = askUserForGameTypeChoice_int(gameTypes, "edit");
                    if(indexToEdit >= 0) {
                        
                        Game game;
                        game.createFromFile(gameTypes[indexToEdit][0]);
                        deleteLineFromFile(files.GAME, indexToEdit);
                        bool saveGameType = true;
                        game.createFromWizard(saveGameType, "edit");
                    }
                }
                else if(subMenuCreate.choice == 3) { //Delete a Custom Game Type

                    std::string gameType;
                    std::vector< std::vector< std::string >> gameTypes = showGameTypesFromFile(files.GAME);
                    int indexToDelete = askUserForGameTypeChoice_int(gameTypes, "delete");
                    if(indexToDelete >= 0)
                        deleteLineFromFile(files.GAME, indexToDelete);
                }
                else if(subMenuCreate.choice == 4) { //Delete a Saved Game

                    std::vector< std::vector< std::string >> savedGames = showSavedGamesFromFile(files.SAVE);
                    int indexToDelete = askUserForSavedGameChoice_int(savedGames, "delete");
                    if(indexToDelete >= 0) {
                        
                        std::vector< int > indicesToDelete = {indexToDelete};
                        deleteLinesFromFile(files.SAVE, indicesToDelete);   
                    }
                }
            }
        }
    }

    std::cout << "Thanks for Playing!";
    
    return 0;
}


