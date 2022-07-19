//function.cpp

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

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

std::string findThemeFromFileLine(std::string fileLine) {
    
    int i = 0;
    while(fileLine[i] != '{')
        i++;
        
    std::string theme = fileLine.substr(0, i);
    return theme;
}

char askUserForChar(std::string message) {
    
    std::string str;
    if(message != " ")
        std::cout << message << std::endl;
    std::cin >> str;
    char character = str[0];
    return character;
}

void showHiddenWord(std::string str) {
    
    for(int c = 0; c < str.size(); c++)
        std::cout << str[c] << " ";
    std::cout << std::endl;
}

bool updateHiddenWord(std::string &wordHidden, std::string word, char userGuess) { //passs wordHidden by reference

    bool correctGuess = false;
    for(int c = 0; c < wordHidden.size(); c++) {
        
        if(word[c] == userGuess) {
            
            //std::cout << "CHANGE";
            wordHidden[c] = word[c];
            correctGuess = true;
        }
    }
    
    return correctGuess;
}


