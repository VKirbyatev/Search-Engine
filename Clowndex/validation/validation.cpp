//File for check validation for files in repo
#include "validation.hpp"
#include <iostream>
#include <fstream>

#define validFileName "validation.txt"

void createValidationFile () {
    std::ifstream fin(validFileName);
    if (fin.is_open())
        std::cout << "This file already exist" << std::endl;
        
    else {
        std::ofstream oFile(validFileName);
        oFile.close();
        std::cout << "Validation file was succesfully created" << std::endl;
    }
}
