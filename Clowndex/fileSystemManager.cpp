#include <fstream>
#include <string>
#include <unistd.h>
#include <iostream>

#include "fileSystemManager.hpp"

std::string getRootPath () {
    int MAXDIR = 1000;
    char dir[MAXDIR];
    getcwd(dir, MAXDIR);
    std::string direction(dir);
    return direction;
}
//Копировать данные по указанному пути в рут дикерторию
void copyOneFileToRoot (std::string filePath) {
    //получаем имя файла
    std::string fileName = "";
    auto i = filePath.size()-1;
    while (filePath[i] != '/' && i >= 0) {
        fileName = filePath[i] + fileName;
        i--;
    }
    if (i == 0) {std::cout << "Wrong Path" << std::endl; return;}
    //что копируем
    std::ifstream inputFile(filePath);
    //куда копируем
    std::ofstream newData(getRootPath() + "/data/" + fileName);
    //буффер
    std::string buffer;
    while (getline(inputFile, buffer)) {
        newData << buffer << std::endl;
    }
    std::cout << "File " << fileName << " was succesfully copied" << std::endl;
}
//скопировать все файлы с названием doc в рут директорию
void copyDirectoryFilesToRoot (std::string filePath) {
    //количество файлов
    auto filesCounter = 1;
    //если нет файлов
    std::ifstream inputFile(filePath + "doc" + std::to_string(filesCounter) + ".txt");
    if (!inputFile.is_open()) {std::cout << "Wrong directory" << std::endl; return;}
    
    while (inputFile.is_open()) {
        //Проверка существования файлов
        if (std::ifstream(getRootPath() + "/data/" + "doc" + std::to_string(filesCounter) + ".txt").is_open()) {
            std::cout << "File doc " + std::to_string(filesCounter) + ".txt already exist, rewrite?(y/n)" << std::endl;
            char answer;
            std::cin >> answer;
            tolower(answer);
            if (answer != 'y')
                continue;
        }
        //куда копируем
        std::ofstream newData(getRootPath() + "/data/" + "doc" + std::to_string(filesCounter) + ".txt", std::ofstream::out | std::ofstream::trunc);
        //буффер
        std::string buffer;
        while (getline(inputFile, buffer)) {
            newData << buffer << std::endl;
        }
        filesCounter++;
        inputFile = std::ifstream(filePath + "doc" + std::to_string(filesCounter) + ".txt");
    }
    std::cout << filesCounter - 1 << " files was succesfully copied" << std::endl;
}
