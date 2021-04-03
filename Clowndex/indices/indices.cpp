#include "indices.hpp"
#include <fstream>
#include <iostream>
#include <unistd.h>
#include <vector>
#include <map>

std::string getRootPath () {
    int MAXDIR = 1000;
    char dir[MAXDIR];
    getcwd(dir, MAXDIR);
    std::string direction(dir);
    return direction;
}

//создать файс с прямыми индексами для 1 текстового файла
void createDirectIndices (std::ifstream fin, std::string name) {
    //слова из файла
    std::map<std::string, int> words;
    //std::vector<std::string> words;
    //количество слов
    int numOfWords = 0;
    //считыываем по словам
    std::string word;
    //проверка существования файла с данными
    if (!fin.is_open()) {
        std::cout << "This document doesnt exist" << std::endl;
        return;
    }
    //Проверка существования файла с индексами
    if (std::ifstream(getRootPath() + "/directIndices/" + name + ".txt").is_open()) {
        std::cout << "File " + name + ".txt" + " already exist, rewrite?(y/n)" << std::endl;
        char answer;
        std::cin >> answer;
        tolower(answer);
        if (answer != 'y')
            return;
    }
    
    while (fin >> word) {
        //считаем общее кол=во слов
        numOfWords++;
        //to lower case
        std::transform(word.begin(), word.end(), word.begin(),
                        [](unsigned char c){ return std::tolower(c); });
        //убираем точки, запятые и пр
        word.erase (std::remove_if (word.begin (), word.end (), ispunct), word.end ());
        //вставляем в вектор, если такого слова еще нет
        if (words.find(word) == words.end() ) 
            words[word] = 1;
        else
            words[word]++;
        
    }
    //создаем файл + записываем кол-во слов
    std::ofstream indexFile(getRootPath() + "/directIndices/" + name + ".txt", std::ofstream::out | std::ofstream::trunc);
    indexFile << numOfWords << std::endl;
    //записываем сами слова
    for (auto it = words.begin(); it != words.end(); it++) {
        indexFile << it->first << " " << it->second << std::endl;
    }
    //оповещаем
    std::cout << "Index file " + name + ".txt" + " was succesfully created" << std::endl;
}

void createInvertIndices (std::string directory) {
    //контейнер с данными
    std::map<std::string, std::vector<int>> words;
    //сюда пишем слова из файлов с прямыми индексами
    std::string word;
    //строим из прямых индексов обратные
    //открываем файл с прямыми индексами
    int count = 1;
    std::ifstream index(directory + "/index" + std::to_string(count) + ".txt");
    //смотрим существует ли хотя бы 1 файл
    if (!index.is_open()) {
        std::cout << "Index files doesnt exist in this directory" << std::endl;
        return;
    }
    //Проверка существования файла с индексами
    if (std::ifstream(getRootPath() + "/invertedIndices/" + "invertedIndex.txt").is_open()) {
        std::cout << "File nvertedIndex.txt already exist, rewrite?(y/n)" << std::endl;
        char answer;
        std::cin >> answer;
        tolower(answer);
        if (answer != 'y')
            return;
    }
    //пропускаем число слов док-та
    index >> word;
    //строим
    while (index.is_open()) {
        //инициализируем
        while(index >> word) {
            if (words.find(word) == words.end() ) {
                //если такого слова нет, то добавляем ему вектор и пишем номер файла
                words[word] = std::vector<int>(0);
                words[word].push_back(count);
            }
            else
                //если есть, то добавляем новый номер док-та
                words[word].push_back(count);
            //пропускаем цифры
            index >> word;
        }
        //увеличиваем
        count++;
        index.close();
        //читаем следующий файл
        index = std::ifstream(directory + "/index" + std::to_string(count) + ".txt");
        //пропускаем число слов док-та
        index >> word;
    }
    //инициализируем документ с обратными индексами
    std::ofstream invIndex(getRootPath() + "/invertedIndices/" + "invertedIndex.txt", std::ofstream::out | std::ofstream::trunc);
    //записываем
    for (auto it = words.begin(); it != words.end(); it++) {
        invIndex << it->first << " ";
        for (auto vecIt = it->second.begin(); vecIt != it->second.end(); vecIt++) {
            invIndex << *vecIt << " ";
        }
        invIndex << std::endl;
    }
    
    std::cout << "Inverted Index file was succesfully created" << std::endl;
}


