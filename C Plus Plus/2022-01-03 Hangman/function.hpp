//function.hpp

#ifndef FUNCTION_HPP
#define FUNCTION_HPP

#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <cstdlib>

std::string trim(std::string str);

std::string getFileContent(std::string fileName);
std::vector< std::vector< std::string >> parseFileContents(std::string fileContents);
std::string findThemeFromFileLine(std::string fileLine);
char askUserForChar(std::string message = " ");
void showHiddenWord(std::string str);
bool updateHiddenWord(std::string &wordHidden, std::string word, char userGuess);
#endif




