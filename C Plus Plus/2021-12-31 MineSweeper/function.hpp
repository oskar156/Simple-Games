//function.hpp

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <cstdlib>
#include <chrono>

#include "class.hpp"

std::string trim(std::string str);

std::string getFileContent(std::string fileName);
std::vector< std::vector< std::string >> parseFileContents(std::string fileContents);
std::vector< std::vector< std::string >> parseVectorFromString(std::string str);
void appendLineToFile(std::string fileName, std::string line);
void appendLinesToFile(std::string fileName, std::vector< std::string > lines);
void deleteLineFromFile(std::string fileName, int indexToDelete);
void deleteLinesFromFile(std::string fileName, std::vector< int > indicesToDelete);

std::vector< std::vector< std::string >> showGameTypesFromFile(std::string fileName);
std::vector< std::vector< std::string >> showSavedGamesFromFile(std::string fileName);
int askUserForGameTypeChoice_int(std::vector< std::vector< std::string >> &gameTypes, std::string action);
int askUserForSavedGameChoice_int(std::vector< std::vector< std::string >> &savedGames, std::string action);
std::string askUserForGameTypeChoice_string(std::vector< std::vector< std::string >> &gameTypes, std::string action);
std::string askUserForSavedGameChoice_string(std::vector< std::vector< std::string >> &savedGames, std::string action);
bool fileWritingInputValidation(std::string input, std::vector< std::string > &names);
#endif


