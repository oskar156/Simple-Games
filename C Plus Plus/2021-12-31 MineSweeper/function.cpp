//function.cpp

#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <cstdlib>
#include <chrono>

#include "function.hpp"
#include "class.hpp"

std::string trim(std::string str) {

    int a = 0;
    while(str[a] == ' ' || str[a] == '\n')
        a++;
    str = str.substr(a);

    a = str.size();
    while(str[a] == ' ' || str[a] == '\n')
        a--;
    str = str.substr(0, a);

    return str;
}

std::string getFileContent(std::string fileName) {

    std::ifstream file(fileName);
    std::string fileContents;

    if(file.is_open()) {

        std::string line;
        while(getline(file, line)) {

            if(line.substr(0, 2) != "//" && line.substr(0, 2) != "/*") {
                   
                fileContents += line;
                fileContents += '\n';
            }
        }

        file.close();
    }
    
    std::string fileContentsTrimmed = fileContents.substr(0, fileContents.size() - 1);

    return fileContentsTrimmed;
}

std::vector< std::vector< std::string >> parseFileContents(std::string fileContents) {

    std::vector< std::vector< std::string >> parsedFileContents = {{}};
    int c = 0;

    while(c < fileContents.size()) {
        
        std::string newStr = "";
        while(fileContents[c] != ',' && fileContents[c] != '{' && fileContents[c] != '}' && fileContents[c] != '[' && fileContents[c] != ']' && 
              fileContents[c] != '\n' && fileContents[c] != '\0') {
            newStr += fileContents[c];
            c++;
        }

        parsedFileContents[parsedFileContents.size() - 1].push_back(trim(newStr));

        if(fileContents[c] == ',') {
            c++;
        }
        else if(fileContents[c] == '\n') {

            parsedFileContents.push_back({});
            c++;
        }
        else if(fileContents[c] == '[') {
            
            int startIndex = c;
            while(fileContents[c] != ']')
                c++;
            int endIndex = c;
            
            std::string vctr = fileContents.substr(startIndex + 1, endIndex - startIndex - 1);
            parsedFileContents.push_back({});
            parsedFileContents[parsedFileContents.size() - 1].push_back(trim(vctr));
        }
        else
            c++;
    }

    //loop through and remove all empty vectors
    for(int a = parsedFileContents.size() - 1; a >= 0; a--)
        if(parsedFileContents[a].size() < 1)
            parsedFileContents.pop_back();

    return parsedFileContents;
}

std::vector< std::vector< std::string >> parseVectorFromString(std::string str) {
    
    std::vector< std::vector< std::string >> vctr;

    int c = 0;
    while(c < str.size()) {
        
        if(str[c] == '{') {
            
            vctr.push_back({});
            c++;
        }
        else if(str[c] != '}' && str[c] != ',') {
            
            std::string element;
            while(str[c] != '}' && str[c] != ',') {
                
                element += str[c];
                c++;
            }
            
            vctr[vctr.size() - 1].push_back(element);
        }
        else
            c++;
    }
    
    return vctr;
}

void appendLineToFile(std::string fileName, std::string line) {

    std::string fileContent = getFileContent(fileName);

    std::ofstream file(fileName, std::ios::app);
    if(fileContent.size() > 0)
        file << "\n";
    file << line;
    file.flush();
    file.close();
}

void appendLinesToFile(std::string fileName, std::vector< std::string > lines) {


    for(int a = 0; a < lines.size(); a++) {
        std::string fileContent = getFileContent(fileName);
    
        std::ofstream file(fileName, std::ios::app);
        if(fileContent.size() > 0 && a != 0)
            file << "\n";
        
        file << lines[a];
        file.flush();
        file.close();
    }

}

void deleteLineFromFile(std::string fileName, int indexToDelete) {

    std::ifstream file(fileName); //open file in read mode
    std::vector< std::string > fileContentsByLine; 
    std::string line;
    int newLines = 0;
    
    while(getline(file, line)) {//read everything into a vector
    
        if(newLines != indexToDelete)
            fileContentsByLine.push_back(line);

        newLines++;
    }
        
    file.close(); //close file
    std::ofstream fileAgain(fileName); //open file in write mode without append to delete everything
    fileAgain.close();
    
    appendLinesToFile(fileName, fileContentsByLine);
}

void deleteLinesFromFile(std::string fileName, std::vector< int > indicesToDelete) {

    for(int a = 0; a < indicesToDelete.size(); a++) {
        std::ifstream file(fileName); //open file in read mode
        std::vector< std::string > fileContentsByLine; 
        std::string line;
        int newLines = 0;
    
        while(getline(file, line)) {//read everything into a vector
    
            if(newLines != indicesToDelete[a])
                fileContentsByLine.push_back(line);

            newLines++;
        }
         
        file.close(); //close file
        std::ofstream fileAgain(fileName); //open file in write mode without append to delete everything
        fileAgain.close();
    
        appendLinesToFile(fileName, fileContentsByLine);
    }
}

std::vector< std::vector< std::string >> showGameTypesFromFile(std::string fileName) {

    std::string fileContents = getFileContent(fileName);
    std::vector< std::vector< std::string >> gameTypes = parseFileContents(fileContents);

    std::cout << "Game Types: " << std::endl;
    for(int a = 0; a < gameTypes.size(); a++) {
        
        std::cout << "- " << gameTypes[a][1] << " x " << gameTypes[a][2] << " (" << gameTypes[a][3] << " mines) // name: " << gameTypes[a][0] << std::endl;
    }
    std::cout << std::endl;
    return gameTypes;
}

std::vector< std::vector< std::string >> showSavedGamesFromFile(std::string fileName) {

    std::string fileContents = getFileContent(fileName);
    std::vector< std::vector< std::string >> savedGames = parseFileContents(fileContents);

    std::cout << "Game Types: " << std::endl;
    for(int a = 0; a < savedGames.size(); a += 3) {
        
        std::cout << "- " << savedGames[a][3] << " x " << savedGames[a][4] << " (" << savedGames[a][5] << " mines) // moves made: " << savedGames[a][6] 
                  << " // name: " << savedGames[a][0] << " (game type: " << savedGames[a][1] << ")" << std::endl;
    }
    std::cout << std::endl;
    return savedGames;
}

int askUserForGameTypeChoice_int(std::vector< std::vector< std::string >> &gameTypes, std::string action) {
    
    std::string gameTypeChoice;
    bool validChoice = false;
    int indexToActOn;
    
    do {
        std::cout << "Please enter the name of the Game Type from the list above to " << action << " it: (Type cancel to return to the menu): " << std::endl;
        std::cin >> gameTypeChoice;
        
        for(int a = 0; a < gameTypes.size(); a++) {
            
            if(gameTypeChoice == gameTypes[a][0] && gameTypeChoice != "cancel") {
                
                validChoice = true;
                indexToActOn = a;
                std::cout << "You have chosen to " << action << " Game Type " << gameTypeChoice << "." << std::endl 
                          << std::endl;
                break;
            }
        }
        if(validChoice == false && gameTypeChoice != "cancel") {
        
            std::cout << "Invalid entry, please try again." << std::endl;   
        }
        
        
    } while(validChoice == false && gameTypeChoice != "cancel");
    
    if(gameTypeChoice == "cancel") {
        
        indexToActOn = -1;
        std::cout << "Action canceled. Returning to the menu." << std::endl
                  << std::endl;
    }
    
    return indexToActOn;
}

int askUserForSavedGameChoice_int(std::vector< std::vector< std::string >> &savedGames, std::string action) {
    
    std::string saveGameChoice;
    bool validChoice = false;
    int indexToActOn;
    
    do {
        std::cout << "Please enter the name of the Game Type from the list above to " << action << " it: (Type cancel to return to the menu): " << std::endl;
        std::cin >> saveGameChoice;
        
        for(int a = 0; a < savedGames.size(); a++) {
            
            if(saveGameChoice == savedGames[a][0] && saveGameChoice != "cancel") {
                
                validChoice = true;
                indexToActOn = a;
                std::cout << "You have chosen to " << action << " Game Type " << saveGameChoice << "." << std::endl 
                          << std::endl;
                break;
            }
        }
        if(validChoice == false && saveGameChoice != "cancel") {
        
            std::cout << "Invalid entry, please try again." << std::endl;   
        }
        
        
    } while(validChoice == false && saveGameChoice != "cancel");
    
    if(saveGameChoice == "cancel") {
        
        indexToActOn = -1;
        std::cout << "Action canceled. Returning to the menu." << std::endl
                  << std::endl;
    }
    
    return indexToActOn;
}

std::string askUserForGameTypeChoice_string(std::vector< std::vector< std::string >> &gameTypes, std::string action) {
    
    std::string gameTypeChoice;
    bool validChoice = false;
    
    do {
        std::cout << "Please enter the name of the Game Type from the list above to " << action << " it: (Type cancel to return to the menu): "  << std::endl;
        std::cin >> gameTypeChoice;
        
        for(int a = 0; a < gameTypes.size(); a++) {
            
            if(gameTypeChoice == gameTypes[a][0] && gameTypeChoice != "cancel") {
                
                validChoice = true;
                std::cout << "You have chosen to " << action << " Game Type " << std::endl 
                          << std::endl;
                break;
            }
        }
        if(validChoice == false && gameTypeChoice != "cancel") {
        
            std::cout << "Invalid entry, please try again." << std::endl;   
        }
        
        
    } while(validChoice == false && gameTypeChoice != "cancel");
    
    if(gameTypeChoice == "cancel")
        std::cout << "Action canceled. Returning to the menu." << std::endl
                  << std::endl;
                  
    return gameTypeChoice;
}

std::string askUserForSavedGameChoice_string(std::vector< std::vector< std::string >> &savedGames, std::string action) {
    
    std::string savedGameChoice;
    bool validChoice = false;

    do {
        std::cout << "Please enter the name of the Saved Game from the list above to " << action << " it: (Type cancel to return to the menu): "  << std::endl;
        std::cin >> savedGameChoice;

        for(int a = 0; a < savedGames.size(); a++) {

            if(savedGameChoice == savedGames[a][0] && savedGameChoice != "cancel") {

                validChoice = true;
                std::cout << "You have chosen to " << action << " a saved game " << std::endl 
                          << std::endl;
                break;
            }
        }
        if(validChoice == false && savedGameChoice != "cancel") {
        
            std::cout << "Invalid entry, please try again." << std::endl;   
        }

    } while(validChoice == false && savedGameChoice != "cancel");
    
    if(savedGameChoice == "cancel")
        std::cout << "Action canceled. Returning to the menu." << std::endl
                  << std::endl;
                  
    return savedGameChoice;
}

bool fileWritingInputValidation(std::string input, std::vector< std::string > &names) {

    bool isValid = true;
    std::vector< char > forbiddenChars = {',', '[', ']', '{', '}', ' ', '\0', '\n', '\t'};
    
    for(int c = 0; c < input.size(); c++) {
        
        char currentChar = input[c];
        
        for(int f = 0; f < forbiddenChars.size(); f++) {
            
            if(currentChar == forbiddenChars[f]) {
                
                isValid = false;
                break;
            }
        }
    }
    
    for(int n = 0; n < names.size(); n++) {
        
        if(names[n] == input) {
            
            isValid = false;
            break;
        }
    }
    
    return isValid;
}


